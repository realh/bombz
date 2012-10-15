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

// ActivityHub.h: Data etc for all of Bombz's activities

#ifndef BOMBZ_ACTIVITY_HUB_H
#define BOMBZ_ACTIVITY_HUB_H

#include "config.h"

#include "hgame/Activity.h"
#include "hgame/Application.h"
#include "hgame/Log.h"
#include "hgame/Sprite.h"
#include "hgame/TextureAtlas.h"
#include "hgame/TextureRegion.h"

#include "bombz/Level.h"

namespace bombz {

class ActivityHub {
private:
    hgame::Log &mLog;
    hgame::Application *mApplication;
    hgame::TextureAtlas *mAlphaAtlas;
    hgame::TextureAtlas *mTileAtlas;
    hgame::TextureAtlas *mLogoAtlas;
    hgame::TextureRegion *mLogoRegion;
    hgame::Sprite *mLogoSprite;
    int mScreenTileSize;
    int mSrcTileSize;
    Level *mLevel;
public:
    ActivityHub(hgame::Application *app);

    ~ActivityHub();

    void initRendering(hgame::RenderContext *rc);

    void deleteRendering(hgame::RenderContext *rc);

    void loadLogo(hgame::RenderContext *rc);

    void deleteLogo();

    int *getBestModes();

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
        return mTileAtlas;
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
        return mScreenTileSize;
    }

    inline int getSrcTileSize() const
    {
        return mSrcTileSize;
    }

    inline Level *getLevel() const
    {
        return mLevel;
    }
};

}

#endif // BOMBZ_ACTIVITY_HUB_H
