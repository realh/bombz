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

#include "bombz/PauseScreen.h"

#include "bombz/GameScreen.h"
#include "bombz/MainMenuScreen.h"
#include "bombz/ScreenHub.h"

namespace bombz {

bool PauseScreen::TapListener::onTapEvent(hgame::TapEvent *e)
{
    (void) e;
    return (mScrn->*mMethod)();
}

bool PauseScreen::onResumeTapped()
{
    mQuit = true;
    mApplication->setScreen(mHub->getGameScreen());
    mHub->deletePauseScreen();
    return true;
}

bool PauseScreen::onExitTapped()
{
    mQuit = true;
    mApplication->setScreen(mHub->getMainMenuScreen());
    mHub->deleteGameScreen();
    mHub->deletePauseScreen();
    return true;
}

PauseScreen::PauseScreen(ScreenHub *hub) :
        MenuScreen(hub, "BombzPauseMenu"),
        mResumeListener(this, &PauseScreen::onResumeTapped),
        mExitListener(this, &PauseScreen::onExitTapped)
{
    float y = kMenuItemTop;
    addTextWidget(mApplication->getPlatform()->translate("Resume"),
            0.5, y, &mResumeListener);
    y += kMenuItemStride;
    addTextWidget(mApplication->getPlatform()->translate("Exit"),
            0.5, y, &mExitListener);
}

}
