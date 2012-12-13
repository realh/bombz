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

#include "handroid/Platform.h"

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace handroid {

using namespace std;

hgame::Platform::PlatformType Platform::getPlatformType() const
{
    return hgame::Platform::ANDROID;
}

/*
const char *Platform::getAssetsDirectory()
{
    return mAssetsDir;
}
*/

char *Platform::getAsset(const char *leafname)
{
	/*
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
    */
	return 0;
}

char Platform::getDirectorySeparator()
{
    return '/';
}

hgame::Image *Platform::loadPNG(const char *leafname)
{
}

hgame::Font *Platform::loadFont(unsigned int px)
{
}


Platform::Platform(android_app *app, const char *app_pkg_name) :
		hgame::Platform(0, new Log("handroid::Platform")),
		mApp(app), mJVM(app->activity->vm),
		mAppPkgName(app_pkg_name)
{
    // Need to fully initialise stuff above before we can create a Translate
    mTranslate = Translate(this);
}

Platform::~Platform()
{
    // free(mAssetsDir);
}

hgame::Log *Platform::createLog(const char *name, hgame::Log::Level priority)
{
    return new Log(name, priority);
}

hgame::Settings *Platform::createSettings(const char *owner, const char *domain,
        const char *appname)
{
    //return new Settings(this, owner, domain, appname);
	return 0;
}

JNIEnv *Platform::getJNIEnv()
{
	Thread *thread = getCurrentThread();
	JavaVMAttachArgs aargs;
	aargs.version = JNI_VERSION_1_6;
	aargs.name = thread ? thread->getName() : "Parent thread";
	aargs.group = 0;
	JNIEnv *env;
	if (mJVM->AttachCurrentThread(&env, &aargs) != JNI_OK)
		abort();
	return env;
}

}
