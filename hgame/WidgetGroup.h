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

// HGame - a simple cross-platform game framework

// WidgetGroup.h: A group of widgets sharing an atlas

#ifndef HGAME_WIDGET_GROUP_H
#define HGAME_WIDGET_GROUP_H

#include "config.h"

#include <list>

#include "hgame/Renderer.h"
#include "hgame/TextureAtlas.h"
#include "hgame/Widget.h"

namespace hgame {

class WidgetGroup : public Renderer, public TapListener {
protected:
    TextureAtlas *mAtlas;
#ifdef ENABLE_WIDGET_HIGHLIGHTING
    TextureAtlas *mHighlightedAtlas;
#endif
    std::list<Widget *> mWidgets;
public:
    inline WidgetGroup() : mAtlas(0)
#ifdef ENABLE_WIDGET_HIGHLIGHTING
            , mHighlightedAtlas(0)
#endif
     {}

    virtual ~WidgetGroup();

    virtual void addWidget(Widget *w);

    virtual void render(RenderContext *rc);

    virtual bool onTapEvent(TapEvent *event);

    virtual void initRendering(RenderContext *rc) = 0;

    virtual void deleteRendering(RenderContext *rc) = 0;
};

}

#endif // HGAME_WIDGET_GROUP_H
