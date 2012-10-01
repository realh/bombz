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

namespace bombz {

const char *Activity::kName = "BombzActivity";

Activity::Activity() :
        hgame::Activity(new hgame::Log(kName), kName),
        mAlphaAtlas(0),
        mTileAtlas(0),
        mLogoAtlas(0)
{
    mLevel = new Level();
}

Activity::~Activity()
{
    delete mLevel;
}

void Activity::initRendering(hgame::RenderContext *rc)
{
    hgame::Platform *platform = getPlatform();
    // FIXME: Replace numbers with constants from a Level class or whatever
    mScreenTileSize = rc->calculateTileSize(20, 15);
    mSrcTileSize = platform->getBestPNGMatch(mScreenTileSize);
    hgame::Image *img = platform->loadPNG("tile_atlas.png", mSrcTileSize);
    mTileAtlas = rc->uploadTexture(img);
    delete img;
    img = platform->loadPNG("alpha_atlas.png", mSrcTileSize);
    mAlphaAtlas = rc->uploadTexture(img);
    delete img;
    mLevel->initRendering(rc, this);
    if (mSubActivity)
        mSubActivity->initRendering(rc);
}

void Activity::deleteRendering(hgame::RenderContext *rc)
{
    if (mSubActivity)
        mSubActivity->deleteRendering(rc);
    mLevel->deleteRendering(rc);
    delete mAlphaAtlas;
    mAlphaAtlas = 0;
    delete mTileAtlas;
    mTileAtlas = 0;
}

void Activity::loadLogo()
{
    hgame::Image *img = getPlatform()->loadPNG("title_logo.png", mSrcTileSize);
    mLogoAtlas = getRenderContext()->uploadTexture(img);
    delete img;
}

}
