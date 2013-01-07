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

#include "bombz/Tiles.h"

#include <cstring>

#include "bombz/Level.h"
#include "bombz/ScreenHub.h"

namespace bombz {

Tiles::Tiles(ScreenHub *hub) :
        mTileAtlas(0), mTileBatcher(0),
        mTileRegions(new hgame::TextureRegion *[BOMB2_FUSED_LAST + 1]),
        mHub(hub)
{
    std::memset(mTileRegions, 0,
            (BOMB2_FUSED_LAST + 1) * sizeof(hgame::TextureRegion *));
}

Tiles::~Tiles()
{
    delete[] mTileRegions;
}

void Tiles::resizeRendering(hgame::RenderContext *rc)
{
    int scrn_tile_size = rc->calculateTileSize(Level::kWidth, Level::kHeight);
    if (scrn_tile_size == mScreenTileSize)
    {
        return;
    }
    mScreenTileSize = scrn_tile_size;
    deleteRendering(rc);
    mSrcTileSize = mHub->getPlatform()->getBestPNGMatch(mScreenTileSize);
    rc->setNeedScaling(mSrcTileSize != mScreenTileSize);
    hgame::Image *img = mHub->getPlatform()->loadPNG(
            "tile_atlas.png", mSrcTileSize);
    mTileAtlas = rc->uploadTexture(img);
    delete img;
    mTileBatcher = rc->createTileBatcher(Level::kWidth, Level::kHeight,
            mScreenTileSize);
    int n;
    for (n = BLANK; n <= BOMB2; ++n)
        mTileRegions[n] = createRegion(n, 0);
    // EXPLO00 is a special case
    mTileRegions[EXPLO00] = createRegion(BLANK);
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
}

void Tiles::initRendering(hgame::RenderContext *rc)
{
    mScreenTileSize = 0;
    resizeRendering(rc);
}

void Tiles::deleteRendering(hgame::RenderContext *rc)
{
    (void) rc;
    delete mTileBatcher;
    mTileBatcher = 0;
    if (mTileRegions)
    {
        for (int n = BLANK; n <= BOMB2_FUSED_LAST; ++n)
        {
            delete mTileRegions[n];
            mTileRegions[n] = 0;
        }
    }
    delete mTileAtlas;
    mTileAtlas = 0;
}

void Tiles::render(hgame::RenderContext *rc)
{
    rc->bindTexture(mTileAtlas);
    mTileBatcher->render(rc);
}

hgame::TextureRegion *Tiles::createRegion(int x, int y)
{
    return mTileAtlas->createRegion(x * mSrcTileSize, y * mSrcTileSize,
            mSrcTileSize, mSrcTileSize);
}

hgame::TextureRegion *Tiles::createRegion(int n)
{
    return createRegion(n % kAtlasColumns, n / kAtlasColumns);
}

}
