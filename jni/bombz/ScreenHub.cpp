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

#include "bombz/ScreenHub.h"

#include "bombz/GameScreen.h"
#include "bombz/MainMenuScreen.h"
#include "bombz/PauseScreen.h"

namespace bombz {

const hgame::Colour ScreenHub::kTextColour = hgame::Colour(0x0000aaff);
#if ENABLE_WIDGET_HIGHLIGHTING
const hgame::Colour ScreenHub::kHighlightedTextColour =
        hgame::Colour(0x880000ff);
#endif

ScreenHub::ScreenHub(hgame::Application *app) :
        mLog(*(app->createLog("BombzScreenHub"))),
        mApplication(app),
        mSettings(new Settings(app->getPlatform()->createSettings(
                "realh", "realh.co.uk", "Bombz"))),
        mAlphaAtlas(0),
        mLogoAtlas(0),
        mLogoRegion(0),
        mLogoSprite(0),
        mTiles(new Tiles(this)),
        mBackground(new Background(mTiles)),
        mLevel(new Level(this, mTiles, app->createLog("Bombz level"))),
        mRcIndex(-1),
        mGameScrn(0),
        mMainMenuScrn(0),
        mPauseScrn(0),
        mWantLogo(false)
{
}

ScreenHub::~ScreenHub()
{
    delete mLevel;
    delete mBackground;
    delete mTiles;
}

void ScreenHub::initRendering(hgame::RenderContext *rc)
{
    if (rc->getIndex() != mRcIndex)
    {
        mTiles->deleteRendering(rc);
        mTiles->initRendering(rc);
        mLog.d("Using source tile size %d", mTiles->getSrcTileSize());
        mVpWidth = mTiles->getSrcTileSize() * Level::kWidth;
        mVpHeight = mTiles->getSrcTileSize() * Level::kHeight;
        // FIXME: In mobile versions margins will depend on touchscreen controls
        mLeftMargin = (rc->getWidth() - mVpWidth) / 2;
        mTopMargin = (rc->getHeight() - mVpHeight) / 2;
        deleteLogo();
        deleteAlpha();
        loadAlpha(rc);
        if (mWantLogo)
            loadLogo(rc);
        mLevel->deleteRendering(rc);
        mLevel->initRendering(rc);
        mBackground->deleteRendering(rc);
        mBackground->initRendering(rc);
        // FIXME: Needs to be done in render() in mobile versions because
        // of on-screen controls
        setRcViewport(rc);
        mRcIndex = rc->getIndex();
    }
    else if (mWantLogo)
    {
        loadLogo(rc);
    }
}

void ScreenHub::deleteRendering(hgame::RenderContext *rc)
{
    if (mMainMenuScrn)
        mMainMenuScrn->freeRendering(rc);
    mLevel->deleteRendering(rc);
    mBackground->deleteRendering(rc);
    mTiles->deleteRendering(rc);
    deleteAlpha();
    deleteLogo();
    mRcIndex = -1;
}

void ScreenHub::replaceRenderingScreen(hgame::RenderContext *rc)
{
    (void) rc;
    if (!mWantLogo)
        deleteLogo();
}

void ScreenHub::loadAlpha(hgame::RenderContext *rc)
{
    hgame::Image *img = getPlatform()->loadPNG("alpha_atlas.png",
            mTiles->getSrcTileSize());
    mAlphaAtlas = rc->uploadTexture(img);
    delete img;
}

void ScreenHub::deleteAlpha()
{
    delete mAlphaAtlas;
    mAlphaAtlas = 0;
}

void ScreenHub::loadLogo(hgame::RenderContext *rc)
{
    int src_ts = mTiles->getSrcTileSize();
    int scr_ts = mTiles->getScreenTileSize();
    hgame::Image *img = getPlatform()->loadPNG("title_logo.png", src_ts);
    mLogoAtlas = rc->uploadTexture(img);
    delete img;
    mLogoRegion = mLogoAtlas->createRegion(0, 0, src_ts * 16, src_ts * 4);
    mLogoSprite = rc->createSprite(mLogoRegion, scr_ts * 16, scr_ts * 4);
    mLogoSprite->setPosition(scr_ts * 2, (scr_ts * 4) / 3);
}

void ScreenHub::deleteLogo()
{
    delete mLogoSprite;
    mLogoSprite = 0;
    delete mLogoRegion;
    mLogoRegion = 0;
    delete mLogoAtlas;
    mLogoAtlas = 0;
}

int *ScreenHub::getBestModes()
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

void ScreenHub::createGameScreen()
{
    mGameScrn = new GameScreen(this);
}

void ScreenHub::createMainMenuScreen()
{
    mMainMenuScrn = new MainMenuScreen(this);
}

void ScreenHub::createPauseScreen()
{
    mPauseScrn = new PauseScreen(this);
}

void ScreenHub::deleteGameScreen()
{
    delete mGameScrn;
    mGameScrn = 0;
}

void ScreenHub::deleteMainMenuScreen()
{
    delete mMainMenuScrn;
    mMainMenuScrn = 0;
}

void ScreenHub::deletePauseScreen()
{
    delete mPauseScrn;
    mPauseScrn = 0;
}

}
