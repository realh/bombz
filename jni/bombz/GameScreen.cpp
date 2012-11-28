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

#include "bombz/GameScreen.h"

#include "bombz/Level.h"
#include "bombz/ScreenHub.h"
#include "bombz/Settings.h"

namespace bombz {

GameScreen::GameScreen(ScreenHub *hub) :
        hgame::Screen(hub->getApplication(), "Bombz game"),
        mLevel(hub->getLevel()),
        mPusher(new Pusher(hub, hub->getPlatform()->createLog("Bombz pusher"))),
        mHub(hub)
{
    Settings *settings = mHub->getSettings();
    mLevel->loadByNumber(settings->getLastLevel() + 1);
    mPusher->reset();
}

GameScreen::~GameScreen()
{
    delete mPusher;
}

void GameScreen::render(hgame::RenderContext *rc)
{
    rc->enableBlend(false);
    rc->bindTexture(mHub->getTileAtlas());
    mHub->getLevel()->render(rc);
    // Rendering level already enables blending and binds alpha
    /*
    rc->enableBlend(true);
    rc->bindTexture(mHub->getAlphaAtlas());
    */
    mPusher->render(rc);
}

void GameScreen::initRendering(hgame::RenderContext *rc)
{
    mHub->initRendering(rc);
    mPusher->initRendering(rc);
}

void GameScreen::deleteRendering(hgame::RenderContext *rc)
{
    mHub->deleteRendering(rc);
}

void GameScreen::freeRendering(hgame::RenderContext *rc)
{
    mPusher->deleteRendering(rc);
}

void GameScreen::replaceRenderingScreen(hgame::RenderContext *rc)
{
    mHub->replaceRenderingScreen(rc);
}

int *GameScreen::getBestModes()
{
    return mHub->getBestModes();
}

int GameScreen::run()
{
    bool quit = false;
    mApplication->setTapEventsEnabled(false);
    requestRenderState(hgame::RendererWithState::RENDER_STATE_RENDERING);
    mApplication->requestRender(true);
    while (!quit)
    {
        hgame::Event *event = mApplication->getNextEvent(kPeriod);
        if (event->getType() == hgame::EVENT_STOP)
        {
            quit = true;
        }
        else if (event->getType() == hgame::EVENT_TICK)
        {
            bool refresh = mPusher->tick();
            refresh = mLevel->tick() || refresh;
            if (refresh)
                mApplication->requestRender(false);
        }
    }
    return 0;
}

}
