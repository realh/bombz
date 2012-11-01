/*
 * Copyright (c) 2012, Tony Houghton <h@realh.co.uk>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

// HGame - a simple cross-platform game framework

#include "hgame/Platform.h"

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#include "hgame/Log.h"
#include "hgame/Types.h"

namespace hgame {

using namespace std;

class CommonDirectoryListing : public DirectoryListing {
private:
    DIR *mDir;
public:
    CommonDirectoryListing(const char *dirname);
    ~CommonDirectoryListing();
    const char *getNext();
};

DirectoryListing::~DirectoryListing()
{
}

Platform::Endianness Platform::getEndianness()
{
    HUInt32 val = 0xff;
    if (*((HUInt8 *) &val) == 0xff)
        return HGAME_LITTLE_ENDIAN;
    return HGAME_BIG_ENDIAN;
}

Platform::~Platform()
{
}

char *Platform::joinPath(const char *first, ...)
{
    va_list ap;
    va_start(ap, first);
    char *result = joinPath(first, ap);
    va_end(ap);
    return result;
}

char *Platform::joinPath(const char *first, std::va_list ap)
{
    char *path = (char *) first;
    char sep = getDirectorySeparator();
    const char *element;

    while ((element = va_arg(ap, const char *)) != 0)
    {
        size_t l = strlen(path);
        char *tmp = path;
        path = new char[l + strlen(element) + 2];
        strcpy(path, tmp);
        path[l] = sep;
        strcpy(path + l + 1, element);
        if (tmp != first)
            delete[] tmp;
    }
    return path;
}

Image *Platform::loadPNG(const char *leafname, int size)
{
    char sz[8];
    sprintf(sz, "%d", size);
    char *name = joinPath("pngs", sz, leafname, NULL);
    try {
        Image *img = loadPNG(name);
        delete[] name;
        return img;
    }
    catch (exception e)
    {
        delete[] name;
        throw e;
    }
}

Platform::FileType Platform::getFileType(const char *filename)
{
    char *path = joinPath(getAssetsDirectory(), filename, NULL);
    struct stat info;
    FileType result = NOT_FOUND;
    if (stat(path, &info))
    {
        if (errno != ENOENT)
        {
            THROW(ErrnoException, errno, "Error accessing file '%s'", filename);
        }
    }
    else if (S_ISREG(info.st_mode))
    {
        result = PLAIN_FILE;
    }
    else if (S_ISDIR(info.st_mode))
    {
        result = DIRECTORY;
    }
    else
    {
        delete[] path;
        THROW(Throwable, "Object '%s' is neither file nor directory (%04o)",
                filename, info.st_mode);
    }
    delete[] path;
    return result;
}

DirectoryListing *Platform::listDirectory(const char *dirname)
{
    return (DirectoryListing *) new CommonDirectoryListing(dirname);
}

int Platform::getBestPNGMatch(int tile_size)
{
    int best = 0;
    char *pathname = joinPath(getAssetsDirectory(), "pngs", NULL);
    DirectoryListing *dir = listDirectory(pathname);
    const char *leafname;
    try {
        while ((leafname = dir->getNext()) != 0)
        {
            int size = atoi(leafname);
            if ((size >= tile_size && (best < tile_size || size < best)) ||
                    size > best)
            {
                best = size;
            }
        }
    }
    catch (exception e) {
        delete dir;
        delete[] pathname;
        throw e;
    }
    delete dir;
    delete[] pathname;
    return best;
}

Log *Platform::createLog(const char *name, Log::Level priority)
{
    return new Log(name, priority);
}

CommonDirectoryListing::CommonDirectoryListing(const char *dirname)
{
    mDir = opendir(dirname);
    if (!mDir)
    {
        THROW(ErrnoException, errno, "Error opening directory '%s'", dirname);
    }
}

CommonDirectoryListing::~CommonDirectoryListing()
{
    closedir(mDir);
}

const char *CommonDirectoryListing::getNext()
{
    errno = 0;
    struct dirent *entry = readdir(mDir);
    if (!entry)
    {
        if (errno)
        {
            THROW(ErrnoException, errno, "Error reading directory");
        }
        return 0;
    }
    if (entry->d_name[0] == '.' &&
            (!entry->d_name[1] || entry->d_name[1] == '.'))
    {
        return getNext();
    }
    return entry->d_name;
}

}
