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

#include "bombz/Pusher.h"

#include "bombz/ScreenHub.h"

namespace bombz {

Pusher::Pusher(ScreenHub *hub, hgame::Log *log) :
        mSprite(0),
        mHub(hub), mLevel(hub->getLevel()), mLog(*log)
{
    for (int n = 0; n < 4; ++n)
    {
        mTextures[n] = 0;
    }
    reset();
}

void Pusher::initRendering(hgame::RenderContext *rc)
{
    mLog.v("Creating pusher textures");
    hgame::TextureAtlas *atlas = mHub->getAlphaAtlas();
    mScreenTileSize = mHub->getScreenTileSize();
    mSrcTileSize = mHub->getSrcTileSize();
    mTextures[LEFT] = atlas->createRegion(mSrcTileSize * 3, 0,
            mSrcTileSize, mSrcTileSize);
    mTextures[RIGHT] = atlas->createRegion(mSrcTileSize * 4, 0,
            mSrcTileSize, mSrcTileSize);
    mTextures[UP] = atlas->createRegion(mSrcTileSize * 3, mSrcTileSize,
            mSrcTileSize, mSrcTileSize);
    mTextures[DOWN] = atlas->createRegion(mSrcTileSize * 4, mSrcTileSize,
            mSrcTileSize, mSrcTileSize);
    mSprite = rc->createSprite(mTextures[1],
            mScreenTileSize, mScreenTileSize);
}

void Pusher::deleteRendering(hgame::RenderContext *rc)
{
    delete mSprite;
    for (int n = 0; n < 4; ++n)
    {
        delete mTextures[n];
        mTextures[n] = 0;
    }
}

void Pusher::render(hgame::RenderContext *rc)
{
    mSprite->setTexture(mTextures[mDirection]);
    mSprite->setPosition(mTileX * mScreenTileSize +
            (mInterX * mScreenTileSize) / 16,
            mTileY * mScreenTileSize +
            (mInterY * mScreenTileSize) / 16);
    mSprite->render(rc);
}

void Pusher::reset()
{
    mTileX = mLevel->getStartX();
    mTileY = mLevel->getStartY();
    mInterX = 0;
    mInterY = 0;
    mDirection = RIGHT;
}



}
