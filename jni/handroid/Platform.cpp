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

#include <cstdlib>
#include <cstring>

#include "handroid/Font.h"
#include "handroid/Image.h"
#include "handroid/Settings.h"

namespace handroid {

hgame::Platform::PlatformType Platform::getPlatformType() const
{
    return hgame::Platform::ANDROID;
}

const char *Platform::getAssetsDirectory()
{
    return "";
}

char Platform::getDirectorySeparator()
{
    return '/';
}

jobject Platform::openAsset(const char *filename, JNIEnv *jenv)
{
    if (!jenv)
        jenv = getJNIEnv();
    jclass act_class = 0;
    jclass asset_class = 0;
    act_class = jenv->FindClass("android/app/Activity");
    if (act_class)
        asset_class = jenv->FindClass("android/content/res/AssetManager");
    jmethodID get_assets_method = 0;
    jmethodID open_method = 0;
    if (asset_class)
        get_assets_method = jenv->GetMethodID(act_class, "getAssets", "(V)L");
    if (get_assets_method)
        open_method = jenv->GetMethodID(asset_class, "open", "(L)L");
    jobject assets = 0;
    jobject istream = 0;
    if (open_method)
        assets = jenv->CallObjectMethod(getJActivity(), get_assets_method);
    jstring jfilename = 0;
    if (assets)
        jfilename = jenv->NewStringUTF(filename);
    if (jfilename)
    {
        istream = jenv->CallObjectMethod(assets, open_method, jfilename);
        jenv->DeleteLocalRef(jfilename);
    }
    if (!istream)
    {
        THROW(JavaException, "Unable to open asset '%s'", filename);
    }
    return istream;
}


void Platform::closeStream(jobject istream, JNIEnv *jenv)
{
    if (!jenv)
        jenv = getJNIEnv();
    jclass istream_class = 0;
    jmethodID close_method = 0;
    istream_class = jenv->FindClass("java/io/InputStream");
    if (istream_class)
        close_method = jenv->GetMethodID(istream_class, "close", "(V)V");
    if (close_method)
        jenv->CallObjectMethod(istream, close_method);
}

hgame::Image *Platform::loadPNG(const char *filename)
{
    JNIEnv *jenv = getJNIEnv();
    jclass fct_class = 0;
    jmethodID decodeStream = 0;
    /*
    jclass bmp_class = 0;
    jclass opt_class = 0;
    jclass cfg_class = 0;
    jmethodID opt_ctor = 0;
    jfieldID config_field = 0;
    jfieldID argb888_field = 0;
    */
    jobject istream = 0;
    jobject bmp = 0;

    fct_class = jenv->FindClass("android/graphics/BitmapFactory");
    if (fct_class)
    {
        decodeStream = jenv->GetStaticMethodID(fct_class, "decodeStream",
                "(L)L");
    }
    /*
    if (decodeStream)
        bmp_class = jenv->FindClass("android/graphics/Bitmap");
    if (bmp_class)
        opt_class = jenv->FindClass("android/graphics/BitmapFactory/Options");
    if (opt_class)
        cfg_class = jenv->FindClass("android/graphics/Bitmap/Config");
    if (cfg_class)
        opt_ctor = jenv->GetMethodID(opt_class, "<init>", "(V)L");
    */
    if (decodeStream)
        istream = openAsset(filename);
    if (istream)
    {
        bmp = jenv->CallStaticObjectMethod(fct_class, decodeStream, istream);
        closeStream(istream, jenv);
    }
    if (!bmp)
    {
        THROW(JavaException, "Unable to load PNG '%s'", filename);
    }
    return new Image(this, bmp);
}

hgame::Font *Platform::loadFont(unsigned int px)
{
    return new Font(this, px);
}

char *Platform::loadText(const char *leafname, std::size_t *psize)
{
    AAsset *asset = AAssetManager_open(getNativeAssetManager(),
            leafname, AASSET_MODE_BUFFER);
    if (!asset)
    {
        THROW(hgame::Throwable, "Unable to open asset '%s'", leafname);
    }
    off_t len = AAsset_getLength(asset);
    const void *src = AAsset_getBuffer(asset);
    char *buf = 0;
    if (src)
        buf = new char[len];
    if (buf)
        std::memcpy(buf, src, len);
    AAsset_close(asset);
    if (!buf)
    {
        THROW(hgame::Throwable, "Unable to load asset '%s'", leafname);
    }
    return buf;
}

class AndroidDirectoryListing : public hgame::DirectoryListing {
private:
    AAssetManager *mMgr;
    AAssetDir *mADir;
public:
    AndroidDirectoryListing(AAssetManager *mgr, const char *parent_dir);
    ~AndroidDirectoryListing();
    const char *getNext();
};

AndroidDirectoryListing::AndroidDirectoryListing(AAssetManager *mgr,
        const char *parent_dir) : mMgr(mgr)
{
    mADir = AAssetManager_openDir(mMgr, parent_dir);
    if (!mADir)
    {
        THROW(hgame::Throwable, "Unable to open asset directory '%s'",
                parent_dir);
    }
}

AndroidDirectoryListing::~AndroidDirectoryListing()
{
    if (mADir)
        AAssetDir_close(mADir);
}

const char *AndroidDirectoryListing::getNext()
{
    const char *filename = AAssetDir_getNextFileName(mADir);
    if (filename && filename[0] == '.' &&
            (!filename[1] ||
            (filename[1] == '.' && !filename[2])))
    {
        return getNext();
    }
    return filename;
}

hgame::DirectoryListing *Platform::listDirectory(const char *dirName)
{
    return new AndroidDirectoryListing(getNativeAssetManager(), dirName);
}

Platform::Platform(struct android_app *app, const char *app_pkg_name) :
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
    return new Settings(this);
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
		std::abort();
	return env;
}

}
