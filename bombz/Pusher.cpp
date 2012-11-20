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
    for (int n = 0; n < 6; ++n)
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
    mTextures[0] = atlas->createRegion(mSrcTileSize * 3, 0,
            mSrcTileSize, mSrcTileSize);
    mTextures[1] = atlas->createRegion(mSrcTileSize * 4, 0,
            mSrcTileSize, mSrcTileSize);
    mTextures[2] = atlas->createRegion(mSrcTileSize * 3, mSrcTileSize,
            mSrcTileSize, mSrcTileSize);
    mTextures[3] = atlas->createRegion(mSrcTileSize * 4, mSrcTileSize,
            mSrcTileSize, mSrcTileSize);
    mTextures[4] = atlas->createRegion(mSrcTileSize * 3, mSrcTileSize * 2,
            mSrcTileSize, mSrcTileSize);
    mTextures[5] = atlas->createRegion(mSrcTileSize * 4, mSrcTileSize * 2,
            mSrcTileSize, mSrcTileSize);
    mSprite = rc->createSprite(mTextures[1],
            mScreenTileSize, mScreenTileSize);
}

void Pusher::deleteRendering(hgame::RenderContext *rc)
{
    (void) rc;
    delete mSprite;
    for (int n = 0; n < 4; ++n)
    {
        delete mTextures[n];
        mTextures[n] = 0;
    }
}

void Pusher::render(hgame::RenderContext *rc)
{
    int x = mTileX * mScreenTileSize +
            (mInterX * mScreenTileSize) / kStepsPerTile;
    int y = mTileY * mScreenTileSize +
            (mInterY * mScreenTileSize) / kStepsPerTile;
    mSprite->setTexture(mTextures[mDirection]);
    mSprite->setPosition(x, y);
    mSprite->render(rc);
    if (mPushingBomb)
    {
        mSprite->setTexture(mTextures[(mPushingBomb == Level::BOMB2) ? 5 : 4]);
        switch (mDirection)
        {
            case LEFT:
                x -= mScreenTileSize;
                break;
            case RIGHT:
                x += mScreenTileSize;
                break;
            case UP:
                y -= mScreenTileSize;
                break;
            case DOWN:
                y += mScreenTileSize;
                break;
        }
        mSprite->setPosition(x, y);
        mSprite->render(rc);
    }
}

void Pusher::reset()
{
    mTileX = mLevel->getStartX();
    mTileY = mLevel->getStartY();
    mInterX = 0;
    mInterY = 0;
    mDirection = RIGHT;
    mMoving = false;
    mHaveMatch = false;
    mPushingBomb = 0;
}

bool Pusher::tick()
{
    bool refresh = false;
    if (!mMoving)
    {
        hgame::Controls::Keys keys = mHub->getApplication()->getControlsState();
        bool left = keys & hgame::Controls::LEFT;
        bool right = keys & hgame::Controls::RIGHT;
        bool up = keys & hgame::Controls::UP;
        bool down = keys & hgame::Controls::DOWN;
        // 2 opposite directions at once cancel each other out
        if (left && right)
        {
            left = right = false;
        }
        if (up && down)
        {
            up = down = false;
        }
        // In case user is pressing horiz & vert simultaneously,
        // alternate based on previous direction
        if (mDirection == UP || mDirection == DOWN)
        {
            if (!checkHoriz(&left, &right, &up, &down))
                checkVert(&left, &right, &up, &down);
        }
        else
        {
            if (!checkVert(&left, &right, &up, &down))
                checkHoriz(&left, &right, &up, &down);
        }
        int dx = 0;
        int dy = 0;
        if (left)
        {
            mMoving = true;
            mDirection = LEFT;
            dx = -1;
        }
        else if (right)
        {
            mMoving = true;
            mDirection = RIGHT;
            dx = 1;
        }
        else if (up)
        {
            mMoving = true;
            mDirection = UP;
            dy = -1;
        }
        else if (down)
        {
            mMoving = true;
            mDirection = DOWN;
            dy = 1;
        }
        HUInt8 c = mLevel->getTileAt(mTileX + dx, mTileY +dy);
        if (c == Level::BOMB1 || c == Level::BOMB2)
        {
            if (mHaveMatch)
            {
                mLog.v("Lighting bomb");
                if (c == Level::BOMB1)
                {
                    mLevel->setTileAt(mTileX + dx, mTileY +dy,
                            Level::BOMB1_FUSED_FIRST, true);
                }
                else
                {
                    mLevel->setTileAt(mTileX + dx, mTileY +dy,
                            Level::BOMB2_FUSED_FIRST, true);
                }
                mMoving = false;
                mHaveMatch = false;
            }
            else
            {
                mPushingBomb = c;
                mLevel->makeBlank(mTileX + dx, mTileY +dy);
            }
        }
    }
    if (mMoving)
    {
        refresh = true;
        switch (mDirection)
        {
            case LEFT:
                mInterX -= 1;
                if (mInterX == -kStepsPerTile)
                    gotToNewTile(-1, 0);
                else if (mInterX == -kStepsPerTile / 2)
                    gotHalfway();
                break;
            case RIGHT:
                mInterX += 1;
                if (mInterX == kStepsPerTile)
                    gotToNewTile(1, 0);
                else if (mInterX == kStepsPerTile / 2)
                    gotHalfway();
                break;
            case UP:
                mInterY -= 1;
                if (mInterY == -kStepsPerTile)
                    gotToNewTile(0, -1);
                else if (mInterY == -kStepsPerTile / 2)
                    gotHalfway();
                break;
            case DOWN:
                mInterY += 1;
                if (mInterY == kStepsPerTile)
                    gotToNewTile(0, 1);
                else if (mInterY == kStepsPerTile / 2)
                    gotHalfway();
                break;
        }
    }
    return refresh;
}

bool Pusher::checkHoriz(bool *left, bool *right, bool *up, bool *down)
{
    if ((*left && mLevel->canMoveTo(mTileX - 1, mTileY, -1, 0, mHaveMatch)) ||
            (*right && mLevel->canMoveTo(mTileX + 1, mTileY, 1, 0, mHaveMatch)))
    {
        *up = *down = false;
        return true;
    }
    else
    {
        *left = *right = false;
    }
    return false;
}

bool Pusher::checkVert(bool *left, bool *right, bool *up, bool *down)
{
    if ((*up && mLevel->canMoveTo(mTileX, mTileY - 1, 0, -1, mHaveMatch)) ||
            (*down && mLevel->canMoveTo(mTileX, mTileY + 1, 0, 1, mHaveMatch)))
    {
        *left = *right = false;
        return true;
    }
    else
    {
        *up = *down = false;
    }
    return false;
}

void Pusher::gotHalfway()
{
    int x = mTileX;
    int y = mTileY;
    switch (mDirection)
    {
        case LEFT:
            x -= 1;
            break;
        case RIGHT:
            x += 1;
            break;
        case UP:
            y -= 1;
            break;
        case DOWN:
            y += 1;
            break;
    }
    HUInt8 c = mLevel->getTileAt(x, y);
    switch (c)
    {
        case Level::MATCH:
            mHaveMatch = true;
        case Level::EARTH:
            mLevel->makeBlank(x, y);
            break;
    }
}

void Pusher::gotToNewTile(int dx, int dy)
{
    mInterX = mInterY = 0;
    mTileX += dx;
    mTileY += dy;
    mMoving = false;
    if (mPushingBomb)
    {
        mLevel->setTileAt(mTileX + dx, mTileY + dy, mPushingBomb);
        mPushingBomb = 0;
    }
}

}
