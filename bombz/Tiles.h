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

// Tiles.h: Tile texture management and rendering

#ifndef BOMBZ_TILES_H
#define BOMBZ_TILES_H

#include "config.h"

#include "hgame/RenderContext.h"
#include "hgame/Renderer.h"
#include "hgame/Sprite.h"
#include "hgame/TextureAtlas.h"
#include "hgame/TextureRegion.h"
#include "hgame/TileBatcher.h"
#include "hgame/Types.h"

namespace bombz {

class ScreenHub;

class Tiles : public hgame::Renderer {
public:
    static const int kAtlasColumns = 6;

    static const int kExploTicks = 12;
    static const int kFuseTicks = 60;
    /*
    static const int kEasyExtraFuse = 20;
    static const int kMaxFuseTicks = kFuseTicks + kEasyExtraFuse;
    */
    static const int kMaxFuseTicks = kFuseTicks;

    enum TileType {
        BLANK,
        EARTH,
        MATCH,
        PICKET,
        BOMB1,
        BOMB2,
        EXPLO00,
        EXPLO11 = EXPLO00 + 11,
        PRE_EXPLO,
        CHROME00,
        CHROME15 = CHROME00 + 15,
        BOMB1_FUSED_FIRST = CHROME15 + 1,
        BOMB1_FUSED_LAST = (int) (BOMB1_FUSED_FIRST + kMaxFuseTicks - 1),
        BOMB2_FUSED_FIRST = BOMB1_FUSED_LAST + 1,
        BOMB2_FUSED_LAST = (int) (BOMB2_FUSED_FIRST + kMaxFuseTicks - 1)
    };
private:
    hgame::TextureAtlas *mTileAtlas;
    hgame::TileBatcher *mTileBatcher;
    int mScreenTileSize;
    int mSrcTileSize;

    hgame::TextureRegion **mTileRegions;

    ScreenHub *mHub;
public:
    Tiles(ScreenHub *hub);
    virtual ~Tiles();

    void initRendering(hgame::RenderContext *rc);

    void deleteRendering(hgame::RenderContext *rc);

    void render(hgame::RenderContext *rc);

    inline int getScreenTileSize() const
    {
        return mScreenTileSize;
    }

    inline int getSrcTileSize() const
    {
        return mSrcTileSize;
    }

    inline hgame::TextureAtlas *getTileAtlas() const
    {
        return mTileAtlas;
    }

    inline hgame::TileBatcher *getTileBatcher() const
    {
        return mTileBatcher;
    }

    inline void setTileAt(int t, int x, int y)
    {
        mTileBatcher->setTextureAt(mTileRegions[t], x, y);
    }
private:
    hgame::TextureRegion *createRegion(int x, int y);

    hgame::TextureRegion *createRegion(int n);
};

}

#endif // BOMBZ_TILES_H
