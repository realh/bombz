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
#include <cstring>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "hgame/Log.h"
#include "hgame/Types.h"

namespace hgame {

using namespace std;

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

Platform::FileType Platform::getFileType(const char *filename)
{
    char *path = joinPath(getAssetsDirectory(), filename, 0);
    struct stat info;
    FileType result = NOT_FOUND;
    if (stat(path, &info))
    {
        if (errno != ENOENT)
        {
            THROW(Throwable, "Error accessing file '%s': %s",
                    filename, strerror(errno));
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
        THROW(Throwable, "Object '%s' is neither file nor directory (%04o)",
                filename, info.st_mode);
    }
    return result;
}

}
