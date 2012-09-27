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

// Platform.h: Various functions giving info about and for manipulating
//             the platform we're running on

#ifndef HGAME_PLATFORM_H
#define HGAME_PLATFORM_H

#include "config.h"

#include <cstdarg>

#include "hgame/Font.h"
#include "hgame/Image.h"

namespace hgame {

class DirectoryListing {
public:
    virtual ~DirectoryListing();
    // Returns a leafname
    virtual const char *getNext() = 0;
};

class Platform {
public:

    enum PlatformType {
        UNKNOWN,
        POSIX,
        WINDOWS,
        MAC,
        ANDROID,
        IOS
    };

    enum FileType {
        NOT_FOUND,
        PLAIN_FILE,
        DIRECTORY
    };

    // "BIG_ENDIAN" and "LITTLE_ENDIAN" seem to be defined elswehere
    enum Endianness {
        HGAME_LITTLE_ENDIAN,
        HGAME_BIG_ENDIAN
    };
    static Endianness getEndianness();

    virtual PlatformType getPlatformType() const = 0;

    // owner is used in Windows
    // This function ensures the directory exists
    // Return value is overwritten on subsequent calls
    virtual const char *getProfileFilename(const char *owner,
            const char *appname, const char *leafname) = 0;

    virtual char getDirectorySeparator() = 0;

    // ... must be NULL-terminated
    char *joinPath(const char *first, ...);
    char *joinPath(const char *first, std::va_list ap);

    // leafname is relative to app's assets folder
    virtual Image *loadPNG(const char *leafname) = 0;

    // Convenience function to load ${ASSETS}/pngs/size/leafname
    Image *loadPNG(const char *leafname, int size);

    virtual ~Platform();

    virtual Font *loadFont(unsigned int px) = 0;

    // filename relative to app's assets folder
    virtual FileType getFileType(const char *filename);

    virtual const char *getAssetsDirectory() = 0;

    virtual DirectoryListing *listDirectory(const char *dirName);

    // Assuming there are a number of subdirectories "48", "72" etc in
    // ${ASSETS}/pngs/ this returns the best match for tile_size
    int getBestPNGMatch(int tile_size);
};

}

#endif // HGAME_PLATFORM_H
