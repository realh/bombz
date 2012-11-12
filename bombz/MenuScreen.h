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

// MenuScreen.h: Base class for menus

#ifndef BOMBZ_MENU_SCREEN_H
#define BOMBZ_MENU_SCREEN_H

#include "config.h"

#include <list>

#include "hgame/Screen.h"
#include "hgame/WidgetGroup.h"

#include "bombz/Level.h"

namespace bombz {

class ScreenHub;

class MenuScreen : public hgame::Screen {
public:
    static const float kMenuItemTop = 0.38f;
    static const float kMenuItemStride = 0.1f;
private:
    hgame::WidgetGroup *mWidgetGroup;
    struct TextWidget {
        const char *text;
        float x, y;
        hgame::Alignment alignment;
        hgame::TapListener *listener;
        TextWidget(const char *t, float x_, float y_,
                hgame::Alignment a, hgame::TapListener *tap_listener) :
                text(t), x(x_), y(y_), alignment(a), listener(tap_listener)
        {}
    };
    std::list<TextWidget> mTextWidgets;
protected:
    ScreenHub *mHub;
public:
    MenuScreen(ScreenHub *hub, const char *name);

    ~MenuScreen();

    virtual void render(hgame::RenderContext *rc);

    virtual void initRendering(hgame::RenderContext *rc);

    // Should just forward to ScreenHub
    virtual void deleteRendering(hgame::RenderContext *rc);

    // Should do the actual deleting
    virtual void freeRendering(hgame::RenderContext *rc);

    virtual void replaceRenderingScreen(hgame::RenderContext *rc);

    // Coordinates are virtual, 0.0-1.0 in each direction; actual widgets
    // are not created until initRendering() when screen size is known
    virtual void addTextWidget(const char *t, float x, float y,
            hgame::TapListener *listener,
            hgame::Alignment a = (hgame::Alignment)
                    (hgame::ALIGN_CENTRE | hgame::ALIGN_TOP));

    int *getBestModes();

    int run();
};

}

#endif // BOMBZ_MENU_SCREEN_H
