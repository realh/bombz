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

#include "bombz/Level.h"

#include <cstdio>
#include <cstdlib>

#include "bombz/ScreenHub.h"

namespace bombz {

Level::Level(ScreenHub *hub, hgame::Log *log) :
        LevelBase(hub),
        mLevel(new HUInt8[kWidth * kHeight]), mTmpLevel(0),
        mAlphaAtlas(0), mExplo00Region(0), mExplo00Sprite(0),
        mLog(*log)
{
    reset();
}

Level::~Level()
{
    delete[] mLevel;
}

void Level::initRendering(hgame::RenderContext *rc)
{
    mLog.v("Creating tile textures");
    LevelBase::initRendering(rc);
    mAlphaAtlas = mHub->getAlphaAtlas();
    mTileRegions = new hgame::TextureRegion *[BOMB2_FUSED_LAST + 1];
    int n;
    for (n = BLANK; n <= BOMB2; ++n)
        mTileRegions[n] = createRegion(n, 0);
    for (n = EXPLO00 + 1; n <= EXPLO11; ++n)
    {
        mTileRegions[n] = createRegion(n - 1);
    }
    mTileRegions[PRE_EXPLO] = createRegion(0, 0);
    for (n = CHROME00; n <= CHROME15; ++n)
        mTileRegions[n] = createRegion(n - CHROME00 + 17);
    int m;
    for (m = 0, n = BOMB1_FUSED_FIRST; n <= BOMB1_FUSED_LAST; ++n, ++m)
    {
        mTileRegions[n] = createRegion(((m & 4) == 0) ? BOMB1 : BLANK);
    }
    for (m = 0, n = BOMB2_FUSED_FIRST; n <= BOMB2_FUSED_LAST; ++n, ++m)
    {
        mTileRegions[n] = createRegion(((m & 4) == 0) ? BOMB2 : BLANK);
    }
    // EXPLO00 is a special case
    mTileRegions[EXPLO00] = createRegion(BLANK);
    mExplo00Region = mAlphaAtlas->createRegion(0, 0,
            mSrcTileSize * 3, mSrcTileSize * 3);
    mExplo00Sprite = rc->createSprite(mExplo00Region,
            mScreenTileSize * 3, mScreenTileSize * 3);
}

void Level::deleteRendering(hgame::RenderContext *rc)
{
    delete mExplo00Sprite;
    mExplo00Sprite = 0;
    delete mExplo00Region;
    mExplo00Region = 0;
    if (mTileRegions)
    {
        for (int n = BLANK; n <= BOMB2_FUSED_LAST; ++n)
        {
            delete mTileRegions[n];
            mTileRegions[n] = 0;
        }
        delete[] mTileRegions;
        mTileRegions = 0;
    }
    // Atlases are owned by Screen, don't delete them
    mAlphaAtlas = 0;
    LevelBase::deleteRendering(rc);
}

void Level::render(hgame::RenderContext *rc)
{
    int n, x, y;
    for (n = 0, y = 0; y < kHeight; ++y)
    {
        for (x = 0; x < kWidth; ++x, ++n)
            mTileBatcher->setTextureAt(mTileRegions[mLevel[n]], x, y);
    }
    rc->bindTexture(mTileAtlas);
    mTileBatcher->render(rc);
    rc->enableBlend(true);
    rc->bindTexture(mAlphaAtlas);
    for (n = 0, y = 0; y < kHeight; ++y)
    {
        for (x = 0; x < kWidth; ++x, ++n)
        {
            if (mLevel[n] == EXPLO00)
            {
                mExplo00Sprite->setPosition((x - 1) * mScreenTileSize,
                        (y - 1) * mScreenTileSize);
                mExplo00Sprite->render(rc);
            }
        }
    }
}

void Level::reset()
{
    int n, x, y;
    for (n = 0, y = 0; y < kHeight; ++y)
    {
        for (x = 0; x < kWidth; ++x, ++n)
        {
            mLevel[n] = BLANK;
        }
    }
    resetVars();
}

void Level::resetVars()
{
    mNBombs = 0;
    mBombActivity = false;
    mStartX = mStartY = 0;
    mTimeLimit = 180;
}

void Level::loadByNumber(int n)
{
    char *leafname;
    hgame::Platform *plat = mHub->getPlatform();
    asprintf(&leafname, "levels%c%02d", plat->getDirectorySeparator(), n);
    char *txt = plat->loadText(leafname);
    loadFromText(txt);
    delete[] txt;
}

void Level::loadFromText(const char *text)
{
    resetVars();
    const char *s = text;
    int n = 0;
    for (int y = 0; y < kHeight; ++y)
    {
        for (int x = 0; x < kWidth; ++x, ++n, ++s)
        {
            switch (*s)
            {
                case ' ':
                    mLevel[n] = BLANK;
                    break;
                case '.':
                    mLevel[n] = EARTH;
                    break;
                case 'D':
                    mLevel[n] = MATCH;
                    break;
                case 'B':
                    mLevel[n] = PICKET;
                    break;
                case 'O':
                    mLevel[n] = BOMB1;
                    ++mNBombs;
                    break;
                case '0':
                    mLevel[n] = BOMB1_FUSED_FIRST;
                    ++mNBombs;
                    mBombActivity = true;
                    break;
                case 'S':
                    mLevel[n] = BLANK;
                    mStartX = x;
                    mStartY = y;
                    break;
                case 'X':
                    mLevel[n] = CHROME15;
                    break;
                default:
                    mLevel[n] = BLANK;
                    break;
            }
        }
        skipNL(&s);
    }
    mTimeLimit = std::atoi(s);
    skipLine(&s);
    prettify();
}

void Level::prettify()
{
    mTmpLevel = new HUInt8[kWidth * kHeight];
    hollowChrome();
    shapeChrome();
    randomiseBombs();
    delete[] mTmpLevel;
    mTmpLevel = 0;
}

void Level::hollowChrome()
{
    int n = 0;
    for (int y = 0; y < kHeight; ++y)
    {
        for (int x = 0; x < kWidth; ++x, ++n)
        {
            if (isChromeAt(x, y))
            {
                bool nonChrome = false;
                for (int y0 = y - 1; !nonChrome && y0 <= y + 1; ++y0)
                {
                    for (int x0 = x - 1; !nonChrome && x0 <= x + 1; ++x0)
                    {
                        if (!isChromeAt(x0, y0))
                        {
                            nonChrome = true;
                        }
                    }
                }
                mTmpLevel[n] = nonChrome ? CHROME15 : BLANK;
            }
            else
            {
                mTmpLevel[n] = mLevel[n];
            }
        }
    }
    swapTmpLevel();
}

void Level::shapeChrome()
{
    int n = 0;
    for (int y = 0; y < kHeight; ++y)
    {
        for (int x = 0; x < kWidth; ++x, ++n)
        {
            if (isChromeAt(x, y))
            {
                bool left = isChromeAt(x - 1, y);
                bool right = isChromeAt(x + 1, y);
                bool above = isChromeAt(x, y - 1);
                bool below = isChromeAt(x, y + 1);
                if (left)
                {
                    if (right)
                    {
                        if (above)
                        {
                            if (below)
                            {
                                //   |
                                // --+--
                                //   |
                                mTmpLevel[n] = CHROME00 + 8;
                            }
                            else
                            {
                                //   |
                                // --+--
                                //
                                mTmpLevel[n] = CHROME00 + 11;
                            }
                        }
                        else	// left && right && !above
                        {
                            if (below)
                            {
                                //
                                // --+--
                                //   |
                                mTmpLevel[n] = CHROME00 + 13;
                            }
                            else
                            {
                                //
                                // --+--
                                //
                                mTmpLevel[n] = CHROME00 + 1;
                            }
                        }
                    }
                    else	// left && !right
                    {
                        if (above)
                        {
                            if (below)
                            {
                                //   |
                                // --+
                                //   |
                                mTmpLevel[n] = CHROME00 + 12;
                            }
                            else
                            {
                                //   |
                                // --+
                                //
                                mTmpLevel[n] = CHROME00 + 5;
                            }
                        }
                        else	// left && !right && !above
                        {
                            if (below)
                            {
                                //
                                // --+
                                //   |
                                mTmpLevel[n] = CHROME00 + 2;
                            }
                            else
                            {
                                //
                                // --+
                                //
                                mTmpLevel[n] = CHROME00 + 9;
                            }
                        }
                    }
                }
                else	// !left
                {
                    if (right)
                    {
                        if (above)
                        {
                            if (below)
                            {
                                //   |
                                //   +--
                                //   |
                                mTmpLevel[n] = CHROME00 + 14;
                            }
                            else
                            {
                                //   |
                                //   +--
                                //
                                mTmpLevel[n] = CHROME00 + 4;
                            }
                        }
                        else	// !left && right && !above
                        {
                            if (below)
                            {
                                //
                                //   +--
                                //   |
                                mTmpLevel[n] = CHROME00;
                            }
                            else
                            {
                                //
                                //   +--
                                //
                                mTmpLevel[n] = CHROME00 + 7;
                            }
                        }
                    }
                    else	// !left && !right
                    {
                        if (above)
                        {
                            if (below)
                            {
                                //   |
                                //   +
                                //   |
                                mTmpLevel[n] = CHROME00 + 3;
                            }
                            else
                            {
                                //   |
                                //   +
                                //
                                mTmpLevel[n] = CHROME00 + 10;
                            }
                        }
                        else	// !left && right && !above
                        {
                            if (below)
                            {
                                //
                                //   +
                                //   |
                                mTmpLevel[n] = CHROME00 + 6;
                            }
                            else
                            {
                                //
                                //   +
                                //
                                mTmpLevel[n] = CHROME00 + 15;
                            }
                        }
                    }
                }
            }
            else
            {
                mTmpLevel[n] = mLevel[n];
            }
        }
    }
    swapTmpLevel();
    disconnectTs();
}

/**
 * If we have two parallel rows or columns they will be unnecessarily
 * linked with T sections ie for a nicer look we can replace:
 *
 *    ===========         ===========
 *    ==+=+=+=+==         ===========
 *      | | | |     with
 *    ==+=+=+=+==         ===========
 *    ===========         ===========
 */
void Level::disconnectTs()
{
    // Don't need to use tmpLvl for this
    int n = 0;
    for (int y = 0; y < kHeight; ++y)
    {
        for (int x = 0; x < kWidth; ++x, ++n)
        {
            int tile = getTileAt(x, y);
            if (tile == CHROME00 + 13)
            {
                // --+--
                //   |
                if (getTileAt(x, y + 1) == CHROME00 + 11)
                {
                    //   |
                    // --+--
                    mLevel[n] = CHROME00 + 1;	        // --+--
                    mLevel[n + kWidth] = CHROME00 + 1;  // --+--
                }
            }
            else if (tile == CHROME00 + 14)
            {
                // |
                // +--
                // |
                if (getTileAt(x + 1, y) == CHROME00 + 12)
                {
                    //   |
                    // --+
                    //   |
                    mLevel[n] = CHROME00 + 3;           //   ||
                    mLevel[n + 1] = CHROME00 + 3;       //   ++
                                                        //   ||
                }
            }
        }
    }
}

void Level::randomiseBombs()
{
    int n = 0;
    for (int y = 0; y < kHeight; ++y)
    {
        for (int x = 0; x < kWidth; ++x, ++n)
        {
            if (mLevel[n] == BOMB1 && std::rand() % 2)
                mLevel[n] = BOMB2;
            else if (mLevel[n] == BOMB1_FUSED_FIRST && std::rand() % 2)
                mLevel[n] = BOMB2_FUSED_FIRST;
        }
    }
}

bool Level::canMoveTo(int x, int y, int dx, int dy, bool have_match, bool bomb)
{
    if (x < 0 || x >= kWidth || y < 0 || y >= kHeight)
        return false;
    HUInt8 c = mLevel[y * kWidth + x];
    switch (c)
    {
        case BLANK:
            return true;
        case EARTH:
        case MATCH:
            return !bomb;
        case PICKET:
            return false;
        case BOMB1:
        case BOMB2:
            if (bomb)
                return false;
            else if (have_match)
                return true;
            else
                return canMoveTo(x + dx, y + dy, dx, dy, false, true);
            break;
        default:
            if (c >= EXPLO00 && c <= EXPLO11)
                return true;
            break;
    }
    return false;
}

bool Level::tick()
{
    if (!mBombActivity)
        return false;
    mBombActivity = false;
    for (int y = 0; y < kHeight; ++y)
    {
        for (int x = 0; x < kWidth; ++x)
        {
            HUInt8 c = getTileAt(x, y);
            if ((c >= BOMB1_FUSED_FIRST && c < BOMB1_FUSED_LAST) ||
                    (c >= BOMB2_FUSED_FIRST && c < BOMB2_FUSED_LAST) ||
                    (c > EXPLO00 && c < EXPLO11))
            {
                setTileAt(x, y, c + 1);
                mBombActivity = true;
            }
            else if (c == PRE_EXPLO)
            {
                setTileAt(x, y, EXPLO00 + 1);
                mBombActivity = true;
            }
            else if (c == EXPLO00)
            {
                mBombActivity = true;
                for (int y0 = (y >= 1 ? y - 1 : y);
                        y0 <= (y < kHeight - 1 ? y + 1 : y);
                        ++y0)
                {
                    for (int x0 = (x >= 1 ? x - 1 : x);
                            x0 <= (x < kWidth - 1 ? x + 1 : x);
                            ++x0)
                    {
                        exploAt(x0, y0, y0 < y || (y0 == y && x0 <= x));
                    }
                }
            }
            else if (c == BOMB1_FUSED_LAST || c == BOMB2_FUSED_LAST)
            {
                --mNBombs;
                mBombActivity = true;
                setTileAt(x, y, EXPLO00);
            }
            else if (c == EXPLO11)
            {
                makeBlank(x, y);
            }
        }
    }
    return true;
}

// If behind it means this cell has already been processed by tick(),
// otherwise it's about to be processed
void Level::exploAt(int x, int y, bool behind)
{
    int c = getTileAt(x, y);
    if (c == BOMB1 ||
            (c >= BOMB1_FUSED_FIRST &&
            c <= BOMB1_FUSED_LAST))
    {
        if (behind)
            setTileAt(x, y, BOMB1_FUSED_LAST);
        else
            setTileAt(x, y, BOMB1_FUSED_LAST - 1);
    }
    else if (c == BOMB2 ||
            (c >= BOMB2_FUSED_FIRST &&
            c <= BOMB2_FUSED_LAST))
    {
        if (behind)
            setTileAt(x, y, BOMB2_FUSED_LAST);
        else
            setTileAt(x, y, BOMB2_FUSED_LAST - 1);
    }
    else if (c < CHROME00 || c > CHROME15)
    {
        if (behind)
            setTileAt(x, y, EXPLO00 + 1);
        else
            setTileAt(x, y, PRE_EXPLO);
    }
}

}
