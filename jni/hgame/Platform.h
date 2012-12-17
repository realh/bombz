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
#include <cstddef>

#include "hgame/Font.h"
#include "hgame/Image.h"
#include "hgame/Log.h"
#include "hgame/Translate.h"
#include "hgame/Types.h"

namespace hgame {

class Settings;

class DirectoryListing {
public:
    virtual ~DirectoryListing();
    // Returns a leafname; . and .. are excluded
    virtual const char *getNext() = 0;
};

class Platform {
protected:
    Translate *mTranslate;
    Log &mLog;
public:
    Platform(Translate *trans, Log *log) :
    		mTranslate(trans), mLog(*log)
	{}

    enum PlatformType {
        UNKNOWN,
        POSIX,
        WINDOWS,
        MAC,
        ANDROID,
        IOS
    };
    virtual PlatformType getPlatformType() const = 0;

    /*
    enum FileType {
        NOT_FOUND,
        PLAIN_FILE,
        DIRECTORY
    };
    */

    // "BIG_ENDIAN" and "LITTLE_ENDIAN" seem to be defined elswehere
    enum Endianness {
        HGAME_LITTLE_ENDIAN,
        HGAME_BIG_ENDIAN
    };
    static Endianness getEndianness();


    // owner is used in Windows and Mac, domain in Linux etc
    // This function ensures the directory exists
    // Return value is overwritten on subsequent calls
    virtual const char *getProfileFilename(const char *owner,
            const char *domain,
            const char *appname, const char *leafname) = 0;

    virtual char getDirectorySeparator() = 0;

    // ... must be NULL-terminated
    // delete[] result
    char *joinPath(const char *first, ...) H_GNUC_NULL_TERMINATED;
    char *joinPath(const char *first, std::va_list ap);

    // leafname is relative to app's assets folder
    virtual Image *loadPNG(const char *leafname) = 0;

    // leafname is relative to app's assets folder.
    // Result should be delete[]d.
    virtual char *loadText(const char *leafname, std::size_t *psize = 0);

    // Convenience function to load ${ASSETS}/pngs/size/leafname
    Image *loadPNG(const char *leafname, int size);

    virtual ~Platform();

    virtual Font *loadFont(unsigned int px) = 0;

    // filename relative to app's assets folder
    // virtual FileType getFileType(const char *filename);

    virtual const char *getAssetsDirectory() = 0;

    virtual DirectoryListing *listDirectory(const char *dirName);

    // Assuming there are a number of subdirectories "48", "72" etc in
    // ${ASSETS}/pngs/ this returns the best match for tile_size
    int getBestPNGMatch(int tile_size);

    virtual Log *createLog(const char *name,
            Log::Level priority = Log::VERBOSE) = 0;

    const char *translate(const char *tag)
    {
        return (*mTranslate)(tag);
    }

    virtual Settings *createSettings(const char *owner, const char *domain,
            const char *appname) = 0;
};

}

#endif // HGAME_PLATFORM_H
