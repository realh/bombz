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

#include "config.h"

#include <algorithm>
#include <vector>

#include "bombz/ActivityHub.h"

namespace bombz {

ActivityHub::ActivityHub(hgame::Application *app) :
        mLog(*(app->createLog("BombzActivityHub"))),
        mApplication(app),
        mAlphaAtlas(0),
        mTileAtlas(0),
        mLogoAtlas(0),
        mLogoRegion(0),
        mLogoSprite(0)
{
    mLevel = new Level();
}

ActivityHub::~ActivityHub()
{
    delete mLevel;
}

void ActivityHub::initRendering(hgame::RenderContext *rc)
{
    hgame::Platform *platform = mApplication->getPlatform();
    mScreenTileSize = rc->calculateTileSize(Level::kWidth, Level::kHeight);
    mSrcTileSize = platform->getBestPNGMatch(mScreenTileSize);
    mLog.d("Using source tile size %d", mSrcTileSize);
    rc->setNeedScaling(mSrcTileSize != mScreenTileSize);
    hgame::Image *img = platform->loadPNG("tile_atlas.png", mSrcTileSize);
    mTileAtlas = rc->uploadTexture(img);
    delete img;
    img = platform->loadPNG("alpha_atlas.png", mSrcTileSize);
    mAlphaAtlas = rc->uploadTexture(img);
    delete img;
    loadLogo(rc);
    mLevel->initRendering(rc, this);
}

void ActivityHub::deleteRendering(hgame::RenderContext *rc)
{
    mLevel->deleteRendering(rc);
    delete mAlphaAtlas;
    mAlphaAtlas = 0;
    delete mTileAtlas;
    mTileAtlas = 0;
    deleteLogo();
}

void ActivityHub::loadLogo(hgame::RenderContext *rc)
{
    hgame::Image *img = getPlatform()->loadPNG("title_logo.png", mSrcTileSize);
    mLogoAtlas = rc->uploadTexture(img);
    delete img;
    mLogoRegion = mLogoAtlas->createRegion(1, 1,
            mSrcTileSize * 16, mSrcTileSize * 4);
    mLogoSprite = rc->createSprite(mLogoRegion,
            mScreenTileSize * 16, mScreenTileSize * 4);
    mLogoSprite->setPosition(mScreenTileSize * 2, mScreenTileSize * 2);
}

void ActivityHub::deleteLogo()
{
    delete mLogoSprite;
    mLogoSprite = 0;
    delete mLogoRegion;
    mLogoRegion = 0;
    delete mLogoAtlas;
    mLogoAtlas = 0;
}

int *ActivityHub::getBestModes()
{
    hgame::Platform *plat = getPlatform();
    char *pngs_dir = plat->joinPath(plat->getAssetsDirectory(), "pngs", NULL);
    hgame::DirectoryListing *dir = plat->listDirectory(pngs_dir);
    delete[] pngs_dir;
    std::vector<int> sizes;
    const char *ss;
    while ((ss = dir->getNext()) != 0)
    {
        mLog.v("Found pngs subdirectory %s", ss);
        sizes.push_back(std::atoi(ss));
    }
    delete dir;
    std::sort(sizes.begin(), sizes.end());
    int *modes = new int[2 * sizes.size() + 2];
    std::vector<int>::reverse_iterator i;
    int n;
    for (n = 0, i = sizes.rbegin(); i != sizes.rend(); ++n, ++i)
    {
        modes[n * 2] = *i * Level::kWidth;
        modes[n * 2 + 1] = *i * Level::kHeight;
    }
    modes[n * 2] = 0;
    modes[n * 2 + 1] = 0;
    return modes;
}

}
