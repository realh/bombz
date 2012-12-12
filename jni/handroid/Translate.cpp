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

#include "handroid/Translate.h"

#include <cstdlib>
#include <stdexcept>

#include "handroid/Platform.h"

#include "uk_co_realh_hgame_handroid_TranslateHelper.h"

namespace hsdl {

const char *Translate::operator()(const char *tag) const
{
    try {
        return mHash.at(tag);
    }
    catch (std::out_of_range) {
        return tag;
    }
}

Translate::Translate(Platform *platform)
{
    JNIEnv *jenv = platform->getJNIEnv();
    // Create a TranslateHelper
    jclass helper_class = jenv->FindClass(
    		"uk/co/realh/hgame/handroid/TranslateHelper");
    jmethodID ctor = 0;
    jobject helper = 0;
    jobject pkg_name = 0;
    if (helper_class)
		ctor = jenv->GetMethodID(helper_class, "<init>", "(LJL)V");
    if (ctor)
    	pkg_name = jenv->NewStringUTF(platform->getAppPkgName);
    if (pkg_name)
    {
    	helper = jenv->NewObject(helper_class, ctor, platform->getJActivity(),
    			(long) this, pkg_name);
    	jenv->DeleteLocalRef(pkg_name);
    	pkg_name = 0;
    }
    if (helper)
    	jenv->DeleteLocalRef(helper);
    if (ctor)
    	jenv->DeleteLocalRef(ctor);
    if (helper_class)
    	jenv->DeleteLocalRef(helper_class);
    if (!helper)
    {
    	THROW(Throwable, "JNI error initialising Translate object");
    }
}

Translate::~Translate()
{
    map<char *, char *, CompStrKey>::iterator it;
    for (it = mHash.begin(); it != mHash.end(); ++it)
    {
        std::free(it->second);
    }
}

void Translate::addKeyValue(const char *key, const char *value)
{
	mHash[strdup(key)] = strdup(value);
}

} // namespace

extern "C" {

JNIEXPORT void JNICALL Java_uk_co_realh_hgame_handroid_TranslateHelper_addKeyValue
		(JNIEnv *jenv, jobject self, jlong nobj_addr, jstring jkey, jstring jvalue)
{
	handroid::Translate *trans = (handroid::Translate *) nobj_addr;
	const char *key = jenv->GetStringUTFChars(jkey, 0);
	const char *value = jenv->GetStringUTFChars(jvalue, 0);
	trans->addKeyValue(key, value);
	jenv->ReleaseStringUTFChars(value);
	jenv->ReleaseStringUTFChars(key);
}

}	// extern "C"
