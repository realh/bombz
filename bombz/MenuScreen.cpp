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

#include "bombz/MenuScreen.h"

#include "hgame/TextWidget.h"

#include "bombz/ActivityHub.h"

namespace bombz {

MenuScreen::MenuScreen(hgame::Application *app, ActivityHub *hub,
        const char *name) :
        hgame::Activity(app, name),
        mWidgetGroup(0),
        mHub(hub)
{
    mHub->getLevel()->reset(true);
}

MenuScreen::~MenuScreen()
{
}

void MenuScreen::render(hgame::RenderContext *rc)
{
    rc->bindTexture(mHub->getTileAtlas());
    mHub->getLevel()->render(rc);
    rc->bindTexture(mHub->getLogoAtlas());
    mHub->getLogoSprite()->render(rc);
    rc->bindTexture(mWidgetGroup->getAtlas());
    mWidgetGroup->render(rc);
}

void MenuScreen::initRendering(hgame::RenderContext *rc)
{
    mHub->initRendering(rc);
    int ts = mHub->getScreenTileSize();
    delete mWidgetGroup;
    mWidgetGroup = new hgame::WidgetGroup;
    hgame::Font *font = mHub->getPlatform()->loadFont(ts);
    for (std::list<TextWidget>::iterator i = mTextWidgets.begin();
            i != mTextWidgets.end(); ++i)
    {
        hgame::TextWidget *w = new hgame::TextWidget(i->text,
                font, ActivityHub::kTextColour,
#if ENABLE_WIDGET_HIGHLIGHTING
                ActivityHub::kHighlightedTextColour,
#endif
                int(i->x * ts * 20), int(i->y * ts * 15), i->alignment,
                ts / 8);
        w->setTapListener(i->listener);
        mWidgetGroup->addWidget(w);
    }
    mWidgetGroup->initRendering(rc);
    delete font;
}

void MenuScreen::deleteRendering(hgame::RenderContext *rc)
{
    mWidgetGroup->deleteRendering(rc);
    delete mWidgetGroup;
    mWidgetGroup = 0;
    mHub->deleteRendering(rc);
}

void MenuScreen::clientChangeRendering(hgame::RenderContext *rc)
{
    // Will be used to switch between screens
}

void MenuScreen::addTextWidget(const char *t, float x, float y,
        hgame::TapListener *listener, hgame::Alignment a)
{
    mTextWidgets.push_back(TextWidget(t, x, y, a, listener));
}

}
