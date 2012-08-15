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
//             the platform we're running on - SDL version

#ifndef HSDL_PLATFORM_H
#define HSDL_PLATFORM_H

#include "config.h"

#include "hgame/Log.h"
#include "hgame/Platform.h"

namespace hsdl {

class Platform : public hgame::Platform {
private:
    char *mAssetsDir;
public:
    hgame::Platform::PlatformType getPlatformType() const;
    
    const char *getProfileFilename(const char *owner,
            const char *appname, const char *leafname);
    
    // Returns the top-level directory holding game's assets
    const char *getAssetsDirectory() const;
    
    // Returns full path to an asset given its leafname.
    // leafname may include subdirs. '/' separators are converted to
    // '\' on Windows.
    // Result must be std::freed, not deleted.
    char *getAsset(const char *leafname);
    
    char getDirectorySeparator();
    
    hgame::Image *loadPNG(const char *leafname);
    
    Platform(int argc, char **argv);
    ~Platform();

protected:
    hgame::Log &mLog;
    
    void mkdirWithParents(const char *dir);
};

}

#endif // HSDL_PLATFORM_H
