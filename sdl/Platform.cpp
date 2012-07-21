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

#include "sdl/Platform.h"

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "SDL_platform.h"

#include "hgame/cstdext.h"
#include "hgame/Log.h"


namespace sdl {

hgame::Platform::PlatformType SDLPlatform::getPlatformType()
{
    if (platformType == hgame::Platform::UNKNOWN)
    {
        /*
        const char *pn = SDL_GetPlatform();
        if (std::strstr(plat_name, "Win") == pn)
        {
            platformType = hgame::Platform::WINDOWS;
        }
        else if (std::strstr(plat_name, "Mac") == pn)
        {
            platformType = hgame::Platform::MAC;
        }
        else
        {
            // Assumption; we don't support anything else with SDL
            platformType = hgame::Platform::POSIX;
        }
        */
#ifdef __WIN32__
        platformType = hgame::Platform::WINDOWS;
#elif defined __MACOSX__
        platformType = hgame::Platform::MAC;
#else
        // Assumption; we don't support anything else with SDL
        platformType = hgame::Platform::POSIX;
#endif
    }
    return platformType;
}

const char *SDLPlatform::getProfileFilename(const char *owner,
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
            log.f("Unsupported platform type %d", pt);
            break;
    }
    const char *varval = std::getenv(var);
    char *dirname;
    asprintf(&dirname, "%s%c%s", varval, ds, body);
    std::free(body);
    log.d("Ensuring directory '%s' exists for profile file '%s'",
            dirname, leafname);
    if (!mkdirWithParents(dirname))
    {
        std::free(dirname);
        std::abort();
    }
    char *filename;
    asprintf(&filename, "%s%c%s", dirname, ds, leafname);
    std::free(dirname);
    return filename;
}
    
char SDLPlatform::getDirectorySeparator()
{
    if (getPlatformType() == hgame::Platform::WINDOWS)
        return '\\';
    return '/';
}
    
SDLPlatform::SDLPlatform() :
        platformType(hgame::Platform::UNKNOWN),
        log("Platform")
{
}

SDLPlatform::~SDLPlatform()
{
}

static bool platform_mkdir(const char *dir, hgame::Log &log)
{
    if (mkdir(dir, 0755))
    {
        log.e("Unable to create directory '%s': %s", dir,
                std::strerror(cerrno));
        return false;
    }
    else
    {
        return true;
    }
}

bool SDLPlatform::mkdirWithParents(const char *dir)
{
    struct stat dinfo;
    bool result = stat(dir, &dinfo) == 0;
    if (result)
    {
        if (S_ISDIR(dinfo.st_mode))
        {
            return true;
        }
        else
        {
            log.e("'%s' exists but is not a directory", dir);
            return false;
        }
    }
    else if (cerrno == ENOENT)
    {
        char *parent = cstrdup(dir);
        char ds = getDirectorySeparator();
        char *sep = std::strrchr(parent, ds);
        if (!sep)
        {
            result = platform_mkdir(dir, log);
        }
        else
        {
            *sep = 0;
            if (mkdirWithParents(parent))
                result = platform_mkdir(dir, log);
            else
                result = false;
        }
        std::free(parent);
    }
    else
    {
        log.e("Unable to create directory '%s': FS error %s", dir,
                std::strerror(cerrno));
    }
    return result;
}

}
