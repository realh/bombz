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

// Level.h: Manages the level

#ifndef BOMBZ_LEVEL_H
#define BOMBZ_LEVEL_H

#include "config.h"

#include "hgame/Log.h"

#include "bombz/LevelBase.h"

namespace bombz {

typedef hgame::HUInt8 HUInt8;

class Level : public LevelBase {
public:
    static const int kExploTicks = 12;
    static const int kFuseTicks = 60;
    /*
    static const int kEasyExtraFuse = 20;
    static const int kMaxFuseTicks = kFuseTicks + kEasyExtraFuse;
    */
    static const int kMaxFuseTicks = kFuseTicks;

    enum {
        BOMB1_FUSED_FIRST = CHROME15 + 1,
        BOMB1_FUSED_LAST = (int) (BOMB1_FUSED_FIRST + kMaxFuseTicks - 1),
        BOMB2_FUSED_FIRST = BOMB1_FUSED_LAST + 1,
        BOMB2_FUSED_LAST = (int) (BOMB2_FUSED_FIRST + kMaxFuseTicks - 1)
    };

private:
    HUInt8 *mLevel;
    HUInt8 *mTmpLevel;

    hgame::TextureAtlas *mAlphaAtlas;
    hgame::TextureRegion *mExplo00Region;
    hgame::Sprite *mExplo00Sprite;

    hgame::Log &mLog;

    int mNBombs;
    int mStartX, mStartY;
    int mTimeLimit;
    bool mBombActivity;

public:
    Level(ScreenHub *hub, hgame::Log *log);
    ~Level();

    void initRendering(hgame::RenderContext *rc);

    void deleteRendering(hgame::RenderContext *rc);

    void render(hgame::RenderContext *rc);

    void reset();

    void loadFromText(const char *text);

    void loadByNumber(int n);

    inline int getStartX() const
    {
        return mStartX;
    }

    inline int getStartY() const
    {
        return mStartY;
    }

    // bomb == true means test whether a bomb can be pushed there rather
    // than whether pusher can move there.
    // Returns true if have_match and there's a bomb there
    bool canMoveTo(int x, int y, int dx, int dy,
            bool have_match, bool bomb = false);

    inline void makeBlank(int x, int y)
    {
        setTileAt(x, y, BLANK);
    }

    inline void setTileAt(int x, int y, HUInt8 t, bool activate = false)
    {
        mLevel[y * kWidth + x] = t;
        if (activate)
            mBombActivity = true;
    }

    inline HUInt8 getTileAt(int x, int y)
    {
        if (x < 0 || x >= kWidth || y < 0 || y >= kHeight)
            return BLANK;
        return mLevel[y * kWidth + x];
    }

    inline bool isChromeAt(int x, int y)
    {
        int t = getTileAt(x, y);
        return t >= CHROME00 && t <= CHROME15;
    }

    // Returns true if screen needs refreshing
    bool tick();
private:
    void resetVars();

    inline static void skipNL(char const **s)
    {
        char c;
        while ((c = **s) == '\n' || c == '\r')
            ++*s;
    }

    inline static void skipLine(char const **s)
    {
        char c;
        while ((c = **s) != '\n' && c != '\r')
            ++*s;
        skipNL(s);
    }

    void prettify();

    void hollowChrome();

    void shapeChrome();

    void disconnectTs();

    void randomiseBombs();

    inline void swapTmpLevel()
    {
        HUInt8 *tmp = mTmpLevel;
        mTmpLevel = mLevel;
        mLevel = tmp;
    }

    void exploAt(int x, int y, bool behind);
};

}

#endif // BOMBZ_LEVEL_H
