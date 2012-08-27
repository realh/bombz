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

#include "hsdl/Exception.h"
#include "hsdl/Font.h"
#include "hsdl/Image.h"
#include "hsdl/Platform.h"

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <SDL_image.h>
#include <SDL_platform.h>

#include "hsdl/Log.h"

namespace hsdl {

using namespace std;

hgame::Platform::PlatformType Platform::getPlatformType() const
{
    /*
    if (platformType == hgame::Platform::UNKNOWN)
    {
        const char *pn = SDL_GetPlatform();
        if (strstr(plat_name, "Win") == pn)
        {
            platformType = hgame::Platform::WINDOWS;
        }
        else if (strstr(plat_name, "Mac") == pn)
        {
            platformType = hgame::Platform::MAC;
        }
        else
        {
            // Assumption; we don't support anything else with SDL
            platformType = hgame::Platform::POSIX;
        }
    }
    */
#ifdef __WIN32__
    return hgame::Platform::WINDOWS;
#elif defined __MACOSX__
    return hgame::Platform::MAC;
#else
    // Assumption; we don't support anything else with SDL
    return hgame::Platform::POSIX;
#endif
}

const char *Platform::getProfileFilename(const char *owner,
            const char *appname, const char *leafname)
{
    const char *var = "HOME";
    char ds = getDirectorySeparator();
    char *body;
    hgame::Platform::PlatformType pt = getPlatformType();
    switch (pt)
    {
        case hgame::Platform::WINDOWS:
            var = "APPDATA";
            asprintf(&body, "%s%c%s", owner, ds, appname);
            break;
        case hgame::Platform::MAC:
            asprintf(&body, "Library%c%s", ds, appname);
            break;
        case hgame::Platform::POSIX:
            asprintf(&body, ".%s", appname);
            break;
        default:
            mLog.f("Unsupported platform type %d", pt);
            break;
    }
    const char *varval = getenv(var);
    char *dirname;
    asprintf(&dirname, "%s%c%s", varval, ds, body);
    free(body);
    mLog.d("Ensuring directory '%s' exists for profile file '%s'",
            dirname, leafname);
    mkdirWithParents(dirname);
    char *filename;
    asprintf(&filename, "%s%c%s", dirname, ds, leafname);
    free(dirname);
    return filename;
}

const char *Platform::getAssetsDirectory()
{
    return mAssetsDir;
}
    
char *Platform::getAsset(const char *leafname)
{
    char *pathname;
    char ds = getDirectorySeparator();
    asprintf(&pathname, "%s%c%s", mAssetsDir, ds, leafname);
    if (ds != '/')
    {
        for (int n = 0; pathname[n]; ++n)
        {
            if (pathname[n] == '/')
                pathname[n] = ds;
        }
    }
    return pathname;
}

char Platform::getDirectorySeparator()
{
    if (getPlatformType() == hgame::Platform::WINDOWS)
        return '\\';
    return '/';
}
    
hgame::Image *Platform::loadPNG(const char *leafname)
{
    char *pathname = getAsset(leafname);
    SDL_Surface *surf = IMG_Load(pathname);
    free(pathname);
    if (!surf)
    {
        THROW(Exception, "Unable to load PNG '%s'", leafname);
    }
    return new Image(surf);
}
    
hgame::Font *Platform::loadFont(unsigned int px)
{
    return new Font(px);
}

Platform::Platform(int argc, char **argv) :
        mLog(*new Log("sdl::Platform"))
{
    char ds = getDirectorySeparator();
    char *dir = strdup(argv[0]);
    *strrchr(dir, ds) = 0;
    *strrchr(dir, ds) = 0;
    asprintf(&mAssetsDir, "%s%cshare%c%s", dir, ds, ds, APPNAME_LOWER);
    free(dir);
}

Platform::~Platform()
{
    free(mAssetsDir);
}

static void platform_mkdir(const char *dir)
{
    if (mkdir(dir, 0755))
    {
        THROW(hgame::Throwable, errno, "Unable to create directory '%s'", dir);
    }
}

void Platform::mkdirWithParents(const char *dir)
{
    struct stat dinfo;
    if (stat(dir, &dinfo) == 0)
    {
        if (S_ISDIR(dinfo.st_mode))
        {
            return;
        }
        else
        {
            THROW(hgame::Throwable, "'%s' exists but is not a directory", dir);
        }
    }
    else if (errno == ENOENT)
    {
        char *parent = strdup(dir);
        char ds = getDirectorySeparator();
        char *sep = strrchr(parent, ds);
        if (!sep || sep == parent)
        {
            platform_mkdir(dir);
        }
        else
        {
            *sep = 0;
            mkdirWithParents(parent);
            platform_mkdir(dir);
        }
        free(parent);
    }
    else
    {
        THROW(hgame::Throwable, errno,
                "Unable to create directory '%s': stat error", dir);
    }
}

}
