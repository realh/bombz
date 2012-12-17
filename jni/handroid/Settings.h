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

// Settings.h: Key-value based settings for user prefs and game state
//             using Android SharedPreferences

#ifndef HANDROID_SETTINGS_H
#define HANDROID_SETTINGS_H

#include "config.h"

#include <jni.h>

#include "hgame/Settings.h"

namespace handroid {

class Platform;

class Settings : public hgame::Settings {
private:
    Platform *mPlatform;
    jobject mPreferences;
    jobject mEditor;
    jclass mPrefsClass, mEditorClass;
    char *mResult;      // Caches result for get(string)
public:
    Settings(Platform *platform);

    ~Settings();

    void set(const char *k, const char *v);

    // Note result may be overwritten on subsequent calls
    const char *get(const char *k, const char *d);

    void set(const char *k, int v);

    int get(const char *k, int d);

    void set(const char *k, float v);

    float get(const char *k, float d);

    // On some platforms this is necessary before calling set*()
    void enableEditing();

    // Saves settings to storage
    // Also undoes enableEditing() where appropriate
    void commit();
private:
    template<class T> void set(const char *method, const char *sig,
            const char *k, T v);
    template<class T> T get(const char *method, const char *sig,
            T (JNIEnv::*caller)(jobject, jmethodID, ...),
            const char *k, T d);
};

}

#endif // HANDROID_SETTINGS_H
