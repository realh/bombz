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

// MainMenuScreen.h: The main menu screen

#ifndef BOMBZ_MAIN_MENU_SCREEN_H
#define BOMBZ_MAIN_MENU_SCREEN_H

#include "config.h"

#include "bombz/MenuScreen.h"

namespace bombz {

class MainMenuScreen : public MenuScreen {
private:
    class PlayListener : public hgame::TapListener {
    private:
        MainMenuScreen *mMMS;
    public:
        PlayListener(MainMenuScreen *m) : mMMS(m) {}

        bool onTapEvent(hgame::TapEvent *e);
    };

    PlayListener mPlayListener;

    void onPlayTapped();

#ifdef HAVE_QUIT_WIDGET
    class QuitListener : public hgame::TapListener {
    private:
        MainMenuScreen *mMMS;
    public:
        QuitListener(MainMenuScreen *m) : mMMS(m) {}

        bool onTapEvent(hgame::TapEvent *e);
    };

    QuitListener mQuitListener;

    void onQuitTapped();
#endif
public:
    MainMenuScreen(ScreenHub *hub);
};

}

#endif // BOMBZ_MAIN_MENU_SCREEN_H
