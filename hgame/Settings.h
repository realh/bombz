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

#ifndef HGAME_SETTINGS_H
#define HGAME_SETTINGS_H

#include "config.h"

namespace hgame {

class Settings {
    // d parameter of set() is default value
public:
    virtual ~Settings();

    virtual void set(const char *k, const char *v) = 0;

    // Note result may be overwritten on subsequent calls
    virtual const char *get(const char *k, const char *d) = 0;

    virtual void set(const char *k, int v) = 0;

    virtual int get(const char *k, int d) = 0;

    inline void set(const char *k, bool v)
    {
        set(k, int(v));
    }

    inline bool get(const char *k, bool d)
    {
        return bool(get(k, int(d)));
    }

    virtual void set(const char *k, float v) = 0;

    virtual float get(const char *k, float d) = 0;

    // On some platforms this is necessary before calling set*()
    virtual void enableEditing() = 0;

    // Saves settings to storage
    // Also undoes enableEditing() where appropriate
    virtual void commit() = 0;
};

}

#endif // HGAME_SETTINGS_H
