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

#include "handroid/Font.h"

#include "handroid/Image.h"
#include "handroid/Platform.h"

namespace handroid {

Font::Font(Platform *platform, int size) :
        mPlatform(platform), mHelperClass(0), mHelper(0)
{
    JNIEnv *jenv = platform->getJNIEnv();
    mHelperClass = jenv->FindClass("src/uk/realh/hgame/handroid/FontHelper");
    jmethodID ctor = 0;
    if (mHelperClass)
    {
        mHelperClass = reinterpret_cast<jclass>
                (jenv->NewGlobalRef(mHelperClass));
    }
    if (mHelperClass)
        ctor = jenv->GetMethodID(mHelperClass, "<init>", "(I)V");
    if (ctor)
        mHelper = jenv->NewObject(mHelperClass, ctor, size);
    if (mHelper)
        mHelper = jenv->NewGlobalRef(mHelper);
    if (!mHelper)
    {
        THROW(JavaException, "Unable to create FontHelper");
    }
}

Font::~Font()
{
    JNIEnv *jenv = mPlatform->getJNIEnv();
    jenv->DeleteGlobalRef(mHelper);
    jenv->DeleteGlobalRef(mHelperClass);
}

hgame::Image *Font::render(hgame::Colour colour, const char *text)
{
    JNIEnv *jenv = mPlatform->getJNIEnv();
    jmethodID render_meth = jenv->GetMethodID(mHelperClass, "render", "(IL)L");
    jobject jtext = 0;
    if (render_meth)
        jtext = jenv->NewStringUTF(text);
    jobject bmp = 0;
    if (jtext)
    {
        bmp = jenv->CallObjectMethod(mHelper, render_meth,
                colour.getRGBA(), jtext);
        jenv->DeleteLocalRef(jtext);
    }
    if (!bmp)
    {
        THROW(JavaException, "Unable to render text '%s'", text);
    }
    return new Image(mPlatform, bmp, jenv);
}

}
