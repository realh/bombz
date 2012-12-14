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

// ScreenHub.h: Data etc for all of Bombz's activities

#ifndef BOMBZ_ACTIVITY_HUB_H
#define BOMBZ_ACTIVITY_HUB_H

#include "config.h"

#include "hgame/Screen.h"
#include "hgame/Application.h"
#include "hgame/Log.h"
#include "hgame/Sprite.h"
#include "hgame/TextureAtlas.h"
#include "hgame/TextureRegion.h"

#include "bombz/Background.h"
#include "bombz/Level.h"
#include "bombz/Settings.h"
#include "bombz/Tiles.h"

namespace bombz {

class GameScreen;
class MenuScreen;

class ScreenHub {
public:
    static const hgame::Colour kTextColour;
#if ENABLE_WIDGET_HIGHLIGHTING
    static const hgame::Colour kHighlightedTextColour;
#endif
private:
    hgame::Log &mLog;
    hgame::Application *mApplication;
    Settings *mSettings;
    hgame::TextureAtlas *mAlphaAtlas;
    hgame::TextureAtlas *mLogoAtlas;
    hgame::TextureRegion *mLogoRegion;
    hgame::Sprite *mLogoSprite;
    Tiles *mTiles;
    Background *mBackground;
    Level *mLevel;
    int mRcIndex;
    MenuScreen *mMainMenuScrn;
    GameScreen *mGameScrn;
    int mLeftMargin, mTopMargin;
    int mVpWidth, mVpHeight;
    bool mWantLogo;
public:
    ScreenHub(hgame::Application *app);

    ~ScreenHub();

    void initRendering(hgame::RenderContext *rc);

    void deleteRendering(hgame::RenderContext *rc);

    void replaceRenderingScreen(hgame::RenderContext *rc);

    int *getBestModes();

    inline hgame::Application *getApplication()
    {
        return mApplication;
    }

    inline hgame::Platform *getPlatform()
    {
        return mApplication->getPlatform();
    }

    inline hgame::TextureAtlas *getAlphaAtlas()
    {
        return mAlphaAtlas;
    }

    inline hgame::TextureAtlas *getTileAtlas()
    {
        return mTiles->getTileAtlas();
    }

    inline hgame::TextureAtlas *getLogoAtlas()
    {
        return mLogoAtlas;
    }

    inline hgame::Sprite *getLogoSprite()
    {
        return mLogoSprite;
    }

    inline int getScreenTileSize() const
    {
        return mTiles->getScreenTileSize();
    }

    inline int getSrcTileSize() const
    {
        return mTiles->getSrcTileSize();
    }

    inline Level *getLevel() const
    {
        return mLevel;
    }

    inline Background *getBackground() const
    {
        return mBackground;
    }

    inline MenuScreen *getMainMenuScreen()
    {
        if (!mMainMenuScrn)
            createMainMenuScreen();
        return mMainMenuScrn;
    }

    inline GameScreen *getGameScreen()
    {
        if (!mGameScrn)
            createGameScreen();
        return mGameScrn;
    }

    void deleteMainMenuScreen();

    void deleteGameScreen();

    // Call immediately before initRendering or replaceClientRendering
    inline void setWantLogo(bool want = true)
    {
        mWantLogo = want;
    }

    inline Settings *getSettings()
    {
        return mSettings;
    }
private:
    inline void setRcViewport(hgame::RenderContext *rc)
    {
        rc->setViewport2D(mLeftMargin, mTopMargin, mVpWidth, mVpHeight);
    }

    void loadAlpha(hgame::RenderContext *rc);

    void deleteAlpha();

    void loadLogo(hgame::RenderContext *rc);

    void deleteLogo();

    void createMainMenuScreen();

    void createGameScreen();
};

}

#endif // BOMBZ_ACTIVITY_HUB_H