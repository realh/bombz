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

// Pusher.h: Manages the main character

#ifndef BOMBZ_PUSHER_H
#define BOMBZ_PUSHER_H

#include "config.h"

#include "hgame/Log.h"
#include "hgame/RenderContext.h"
#include "hgame/Renderer.h"
#include "hgame/Sprite.h"
#include "hgame/TextureRegion.h"

namespace bombz {

class Level;
class ScreenHub;

// Must be created after level
class Pusher : public hgame::Renderer {
private:
    static const int kStepsPerTile = 16;

    enum Direction { LEFT, RIGHT, UP, DOWN };

    int mTileX, mTileY;
    int mInterX, mInterY;
    Direction mDirection;
    bool mMoving;

    int mScreenTileSize;
    int mSrcTileSize;

    hgame::TextureRegion *mTextures[4];     // Left, right, up, down
    hgame::Sprite *mSprite;

    ScreenHub *mHub;
    Level *mLevel;

    hgame::Log &mLog;
public:
    Pusher(ScreenHub *hub, hgame::Log *log);

    void reset();

    void initRendering(hgame::RenderContext *rc);

    void deleteRendering(hgame::RenderContext *rc);

    void render(hgame::RenderContext *rc);

    // Returns true if movement etc requires screen refresh
    bool tick();
private:
    // Returns true if should move horizontally
    bool checkHoriz(bool left, bool right);

    // Returns true if should move vertically
    bool checkVert(bool up, bool down);
};

}

#endif // BOMBZ_PUSHER_H
