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

#ifndef HANDROID_PLATFORM_H
#define HANDROID_PLATFORM_H

#include "config.h"

#include <jni.h>
#include <android/android_native_app_glue.h>

#include "hgame/Log.h"
#include "hgame/Platform.h"

#include "handroid/Thread.h"

namespace handroid {

class Platform : public hgame::Platform {
private:
    //char *mAssetsDir;
    android_app *mApp;
    JavaVM *mJVM;
    const char *mAppPackagName;
public:
    hgame::Platform::PlatformType getPlatformType() const;

    const char *getProfileFilename(const char *owner, const char *domain,
            const char *appname, const char *leafname);

    // Returns the top-level directory holding game's assets
    const char *getAssetsDirectory();

    // Returns full path to an asset given its leafname.
    // leafname may include subdirs. '/' separators are converted to
    // '\' on Windows.
    // Result must be std::freed, not deleted.
    char *getAsset(const char *leafname);

    char getDirectorySeparator();

    hgame::Image *loadPNG(const char *leafname);

    hgame::Font *loadFont(unsigned int px);

    // app_pkg_name must be static
    Platform(android_app *app, const char *app_pkg_name);
    ~Platform();

    hgame::Log *createLog(const char *name,
            hgame::Log::Level priority = hgame::Log::VERBOSE);

    hgame::Settings *createSettings(const char *owner, const char *domain,
            const char *appname);

    // Android-specific

    // Attaches calling thread
    JNIEnv *getJNIEnv();

    inline Thread *getCurrentThread()
    {
    	return Thread::getCurrentThread();
    }

    inline android_app *getAndroidApp()
    {
        return mApp;
    }

    inline ANativeActivity *getNativeActivity()
    {
        return mApp->-activity;
    }

    inline jobject getJActivity()
    {
        return mApp->-activity>clazz;
    }

    inline const char *getAppPkgName() const
    {
    	return mAppPkgName;
    }

protected:
    hgame::Log &mLog;

    void mkdirWithParents(const char *dir);
};

}

#endif // HANDROID_PLATFORM_H