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

#include "bombz/MainMenuScreen.h"

namespace bombz {

bool MainMenuScreen::PlayListener::onTapEvent(hgame::TapEvent *e)
{
    (void) e;
    mMMS->onPlayTapped();
    return true;
}

void MainMenuScreen::onPlayTapped()
{
}

bool MainMenuScreen::ChooseLevelListener::onTapEvent(hgame::TapEvent *e)
{
    (void) e;
    mMMS->onChooseLevelTapped();
    return true;
}

void MainMenuScreen::onChooseLevelTapped()
{
}

#ifdef HAVE_QUIT_WIDGET
bool MainMenuScreen::QuitListener::onTapEvent(hgame::TapEvent *e)
{
    (void) e;
    mMMS->onQuitTapped();
    return true;
}

void MainMenuScreen::onQuitTapped()
{
    mApplication->stop();
}

#endif

MainMenuScreen::MainMenuScreen(ScreenHub *hub) :
        MenuScreen(hub, "BombzMainMenu"),
        mPlayListener(this),
        mChooseLevelListener(this)
#ifdef HAVE_QUIT_WIDGET
        , mQuitListener(this)
#endif
{
    float y = kMenuItemTop;
    addTextWidget(mApplication->getPlatform()->translate("Play"),
            0.5, y, &mPlayListener);
    y += kMenuItemStride;
    addTextWidget(mApplication->getPlatform()->translate("Choose Level"),
            0.5, y, &mChooseLevelListener);
    y += kMenuItemStride;
#ifdef HAVE_QUIT_WIDGET
    addTextWidget(mApplication->getPlatform()->translate("Quit"),
            0.5, y, &mQuitListener);
#endif
}

}
