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

namespace hsdl {

const char *Translate::smTags = {
        "Play",
        "Choose_Level",
        "Quit",
        0
};

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
    jobject act = platform->getAndroidApp()->activity->clazz;
    jclass act_class = jenv->GetObjectClass(jenv);
    jmethodID getString_method = jenv->GetMethodId(act_class, "getString",
            "(L)L");
    jclass r_class = jenv->FindClass("uk/co/realh/bombznative/R");
    int n;
    for (n = 0; smTags[n]; ++n)
    {
        jstring tag = jenv->NewStringUTF(smTags[n]);
        jstring value = jenv->CallObjectMethod(act, getString_method, tag);
        const char *value_s = jenv->GetStringUTFChars(value);
        mHash[smTags[n]] = strdup(value_s);
        jenv->ReleaseStringUTFChars(value_s);
        jenv->DeleteLocalRef(value);
        jenv->DeleteLocalRef(tag);
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

}
