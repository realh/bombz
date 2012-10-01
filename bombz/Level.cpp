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

#include "bombz/Activity.h"
#include "bombz/Level.h"

namespace bombz {

Level::Level() : mRc(0), mTileAtlas(0), mTileBatcher(0), mTileRegions(0)
{
    mLevel = new HUInt8[kWidth * kHeight];
    mTmpLevel = new HUInt8[kWidth * kHeight];
}

Level::~Level()
{
    delete[] mTmpLevel;
    delete[] mLevel;
}

void initRendering(hgame::RenderContext *rc, Activity *act)
{
    mRc = rc;
    mTileAtlas = act->getTileAtlas();
    mAlphaAtlas = act->getAlphaAtlas();
    mScreenTileSize = act->getScreenTileSize();
    mSrcTileSize = act->getSrcTileSize();
    mTileBatcher = rc->createTileBatcher(kWidth, kHeight, mScreenTileSize);
    mTileRegions = new hgame::TextureRegion *[BOMB2_FUSED_LAST + 1];
    int n;
    for (n = BLANK; n <= BOMB2; ++n)
        mTileRegions[n] = createRegion(n, 0);
    // EXPLO00 is a special case
    mTileRegions[EXPLO00] = createRegion(0, 0);
    for (n = EXPLO00 + 1; n <= EXPLO11; ++n)
        mTileRegions[n] = createRegion(n - 1, 0);
    for (n = CHROME00; n <= CHROME15; ++n)
        mTileRegions[n] = createRegion(n - CHROME00 + 17);
    int m;
    for (m = 0, n = BOMB1_FUSED_FIRST; n <= BOMB1_FUSED_LAST; ++n, ++m)
    {
        mTileRegions[n] = createRegion(((i & 4) == 0) ? BOMB1 : BLANK;
    }
    for (m = 0, n = BOMB2_FUSED_FIRST; n <= BOMB2_FUSED_LAST; ++n, ++m)
    {
        mTileRegions[n] = createRegion(((i & 4) == 0) ? BOMB2 : BLANK;
    }
    mExplo00Region = mAlphaAtlas->createRegion(1, 1,
            mSrcTileSize * 3, mSrcTileSize * 3);
}

    void deleteRendering(hgame::RenderContext *rc);

    void render(hgame::RenderContext *rc);

hgame::TextureRegion *Level::createRegion(int x, int y)
{
    return mTileAtlas->createRegion(x * (mSrcTileSize + 2) + 1,
            y * (mSrcTileSize + 2) + 1);
}

hgame::TextureRegion *Level::createRegion(int n)
{
    return createRegion(n % kAtlasColumns, n / kAtlasColumns);
}

}
