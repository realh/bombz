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

#include "hgame/Activity.h"
#include "hgame/WidgetGroup.h"

#include "bombz/Level.h"

namespace bombz {

class ActivityHub;

class MenuScreen, public Activity {
private:
    hgame::WidgetGroup *mWidgetGroup;
    struct TextWidget {
        const char *text;
        float x, y;
        hgame::Alignment alignment;
        TextWidget(const char *t, int x_, int y_, Alignment a) :
                text(t), x(x_), y(y_), alignment(a)
        {}
    };
    std::list<Widget> mWidgets;
protected:
    ActivityHub *mHub;
public:
    MenuScreen(Application *app, ActivityHub *hub, const char *name);

    ~MenuScreen();

    virtual void render(RenderContext *rc);

    virtual void initRendering(hgame::RenderContext *rc);

    virtual void deleteRendering(hgame::RenderContext *rc);

    // Coordinates are virtual, 0.0-1.0 in each direction; actual widgets
    // are not created until initRendering() when screen size is known
    virtual void addTextWidget(const char *t, int x, int y, Alignment a);
};

}

#endif // BOMBZ_MENU_SCREEN_H
