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

// Bombz - 2D puzzle game

// Settings.h: User preferences and other saveable state

#ifndef BOMBZ_SETTINGS_H
#define BOMBZ_SETTINGS_H

#include "config.h"

#include "hgame/Settings.h"

namespace bombz {

class Settings {
private:
    hgame::Settings *mSettings;
    int mBestLevel;     // Highest level ever completed
    int mLastLevel;     // Level most recently completed
public:
    // Takes ownership of settings
    Settings(hgame::Settings *settings);

    ~Settings()
    {
        delete mSettings;
    }

    void setBestLevel(int l)
    {
        mBestLevel = l;
    }

    void setLastLevel(int l)
    {
        mLastLevel = l;
    }

    int getBestLevel() const
    {
        return mBestLevel;
    }

    int getLastLevel() const
    {
        return mLastLevel;
    }

    // Commits to mSettings
    void sync();
};

}

#endif // BOMBZ_SETTINGS_H
