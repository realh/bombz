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

// Settings.h: Key-value based settings for user prefs and game state (SDL)

#ifndef HSDL_SETTINGS_H
#define HSDL_SETTINGS_H

#include "config.h"

#include <map>
#include <string>

#include "hgame/Platform.h"
#include "hgame/Settings.h"

namespace hsdl {

class Settings : public hgame::Settings {
private:
    std::map<std::string, std::string> mHash;
    char *mFilename;
public:
    Settings(hgame::Platform *plat,
            const char *owner, const char *domain, const char *appname);

    ~Settings();

    void set(const char *k, const char *v);

    const char *get(const char *k, const char *d);

    void set(const char *k, int v);

    int get(const char *k, int d);

    void set(const char *k, float v);

    float get(const char *k, float d);

    void enableEditing();

    void commit();
private:
    template<class T>
    void set(const char *k, T v);

    template<class T>
    T get(const char *k, T d);
};

}

#endif // HSDL_SETTINGS_H
