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

#include "bombz/Background.h"

#include "bombz/ScreenHub.h"

namespace bombz {

Background::~Background()
{
}

void Background::initRendering(hgame::RenderContext *rc)
{
    LevelBase::initRendering(rc);
    mTileRegions = new hgame::TextureRegion *[CHROME15 + 1];
    mTileRegions[BLANK] = createRegion(0, 0);
    int n;
    for (n = EARTH; n < CHROME00; ++n)
        mTileRegions[n] = 0;
    for (n = CHROME00; n <= CHROME15; ++n)
        mTileRegions[n] = createRegion(n - CHROME00 + 17);
    int x, y;
    for (n = 0, y = 0; y < kHeight; ++y)
    {
        for (x = 0; x < kWidth; ++x, ++n)
        {
            HUInt8 t = BLANK;
            if (y == 0 && x == 0)
                t = CHROME00;
            else if (y == 0 && x == kWidth - 1)
                t = CHROME00 + 2;
            else if (y == kHeight - 1 && x == 0)
                t = CHROME00 + 4;
            else if (y == kHeight - 1 && x == kWidth - 1)
                t = CHROME00 + 5;
            else if (y == 0 || y == kHeight - 1)
                t = CHROME00 + 1;
            else if (x == 0 || x == kWidth - 1)
                t = CHROME00 + 3;
            mTileBatcher->setTextureAt(mTileRegions[t], x, y);
        }
    }
}

void Background::deleteRendering(hgame::RenderContext *rc)
{
    if (mTileRegions)
    {
        delete mTileRegions[BLANK];
        mTileRegions[BLANK] = 0;
        for (int n = CHROME00; n <= CHROME15; ++n)
        {
            delete mTileRegions[n];
            mTileRegions[n] = 0;
        }
        delete[] mTileRegions;
        mTileRegions = 0;
    }
    LevelBase::deleteRendering(rc);
}

void Background::render(hgame::RenderContext *rc)
{
    rc->bindTexture(mTileAtlas);
    mTileBatcher->render(rc);
}

}
