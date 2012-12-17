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

#include "handroid/Settings.h"

#include <cstring>

#include "handroid/Platform.h"
#include "handroid/Types.h"

namespace handroid {

Settings::Settings(Platform *platform) :
        mPlatform(platform), mPreferences(0), mEditor(0),
        mPrefsClass(0), mEditorClass(0), mResult(0)
{
    JNIEnv *jenv = mPlatform->getJNIEnv();
    jclass act_class = jenv->FindClass("android/app/Activity");
    jmethodID get_prefs = 0;
    if (act_class)
        get_prefs = jenv->GetMethodID(act_class, "getPreferences", "(I)L");
    if (get_prefs)
    {
        mPreferences = jenv->CallObjectMethod(mPlatform->getJActivity,
                get_prefs, 0 /* MODE_PRIVATE */);
    }
    if (mPreferences)
        mPreferences = jenv->NewGlobalRef(mPreferences);
    if (!mPreferences)
    {
        THROW(JavaExcception, "Unable to open SharedPreferences");
    }
    mPrefsClass = jenv->FindClass("android/content/SharedPreferences");
    if (mPrefsClass)
        mPrefsClass = jenv->NewGlobalRed(mPrefsClass);
    if (mPrefsClass)
    {
        mEditorClass = jenv->FindClass(
                "android/content/SharedPreferences.Editor");
    }
    if (mEditorClass)
        mEditorClass = jenv->NewGlobalRed(mEditorClass);
    if (!mEditorClass || !mPrefsClass)
    {
        THROW(JavaException, "Unable to cache Preferences classes");
    }
}

Settings::~Settings()
{
    if (mEditor)
        commit();
    JNIEnv *jenv = mPlatform->getJNIEnv();
    if (mPreferences)
        jenv->DeleteGlobalRef(mPreferences);
    if (mEditorClass)
        jenv->DeleteGlobalRef(mEditorClass);
    if (mPrefsClass)
        jenv->DeleteGlobalRef(mPrefsClass);
}

template<class T> void Settings::set(const char *method, const char *sig,
        const char *k, T v)
{
    if (!mEditor)
    {
        THROW(hgame::Throwable,
                "Tried to set setting '%s' while editing not enabled", k);
    }
    JNIEnv *jenv = mPlatform->getJNIEnv();
    jobject jk = jenv->NewStringUTF(k);
    if (!jk)
    {
        THROW(JavaException, "Unable to make Java string to set setting '%s'",
                k);
    }
    jmethodID jmeth = jenv->GetMethodID(mEditorClass, method, sig);
    if (jmeth)
    {
        jenv->CallVoidMethod(mEditor, jmeth, jk, v);
        jenv->DeleteLocalRef(jk);
    }
    else
    {
        jenv->DeleteLocalRef(jk);
        THROW(JavaException, "Unable to get method to set setting '%s'", k);
    }
    jmeth = jenv->GetMethodID(mEditorClass, "apply", "(V)V");
    if (jmeth)
        jenv->CallVoidMethod(mEditor, jmeth);
}

template<class T> T Settings::get(const char *method, const char *sig,
        T (JNIEnv::*caller)(jobject, jmethodID, ...),
        const char *k, T d)
{
    JNIEnv *jenv = mPlatform->getJNIEnv();
    jobject jk = jenv->NewStringUTF(k);
    if (!jk)
    {
        THROW(JavaException, "Unable to make Java string to get setting '%s'",
                k);
    }
    jmethodID jmeth = jenv->GetMethodID(mEditorClass, method, sig);
    T result = 0;
    if (jmethodID)
    {
        result = (jenv->*caller)(mEditor, jmeth, k, d);
        jenv->DeleteLocalRef(jk);
    }
    else
    {
        jenv->DeleteLocalRef(jk);
        THROW(JavaException, "Unable to get method to get setting '%s'", k);
    }
    return result;
}

void Settings::set(const char *k, const char *v)
{
    JNIEnv *jenv = mPlatform->getJNIEnv();
    jobject jv = jenv->NewStringUTF(v);
    if (!jv)
    {
        THROW(JavaException,
                "Unable to make Java string of value '%s' to set setting '%s'",
                v, k);
    }
    set<jobject>("putString", "(LL)V", k, jv);
    jenv->DeleteLocalRef(jv);
}

const char *Settings::get(const char *k, const char *d)
{
    JNIEnv *jenv = mPlatform->getJNIEnv();
    jobject jd = jenv->NewStringUTF(d);
    if (!jd)
    {
        THROW(JavaException,
                "Unable to make Java string of default value '%s' "
                "to get setting '%s'",
                d, k);
    }
    jobject jresult = get<jobject>("getString", "(LL)L",
            &JNIEnv::CallObjectMethod, k, jd);
    jenv->DeleteLocalRef(jd);
    const char *result = 0;
    if (jresult)
        result = jenv->GetStringUTFChars(jresult, 0);
    if (result)
    {
        std::free(mResult);
        mResult = strdup(result);
        jenv->ReleaseStringUTFChars(result);
    }
    else
    {
        THROW(JavaException, "Unable to get string from settings key '%s'", k);
    }
    return mResult;
}

void Settings::set(const char *k, int v)
{
    set<int>("putInt", "(LI)V", k, v);
}

int Settings::get(const char *k, int d)
{
    return get<int>("getInt", "(LI)I", &JNIEnv::CallIntMethod, k, d);
}

void Settings::set(const char *k, float v)
{
    set<float>("putFloat", "(LF)V", k, v);
}

float Settings::get(const char *k, float d)
{
    return get<float>("getFloat", "(LF)F", &JNIEnv::CallFloatMethod, k, d);
}

void Settings::enableEditing()
{
    if (mEditor)
        return;
    JNIEnv *jenv = mPlatform->getJNIEnv();
    jmethodID edit = jenv->GetMethodID(mPrefsClass, "edit", "(V)L");
    if (edit)
    {
        mEditor = jenv->CallObjectMethod(mPrefs, edit);
    }
    if (mEditor)
        mEditor = jenv->NewGlobalRef(mEditor);
    if (!mEditor)
    {
        THROW(JavaException, "Unable to enable settings editing");
    }
}

void Settings::commit()
{
    if (!mEditor)
    {
        THROW(hgame::Throwable, "Trying to commit unedited settings");
    }
    JNIEnv *jenv = mPlatform->getJNIEnv();
    jmethodID commit = jenv->GetMethodID(mPrefsClass, "commit", "(V)V");
    if (commit)
    {
        jenv->CallVoidMethod(mEditor, commit);
    }
    else
    {
        THROW(JavaException, "Unable to commit settings");
    }
    jenv->DeleteGlobalRef(mEditor);
    mEditor = 0;
}

}
