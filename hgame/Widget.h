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

// Widget.h: An on-screen widget that responds to tap-events

#ifndef HGAME_WIDGET_H
#define HGAME_WIDGET_H

#include "config.h"

#include "hgame/Image.h"
#include "hgame/RenderContext.h"
#include "hgame/Sprite.h"
#include "hgame/TapEvent.h"
#include "hgame/TextureRegion.h"
#include "hgame/Types.h"

namespace hgame {

// Note this isn't a Renderer despite having a render method because
// resources are usually part of a shared atlas, so it has a setTextureRegion
// method instead
class Widget : public TapListener {
public:
    static const Alignment kDefaultAlignment =
            (Alignment) (ALIGN_LEFT | ALIGN_TOP);
protected:
    TapListener *mListener;
    TextureRegion *mRegion;
    Sprite *mSprite;
    int mX0, mY0, mX1, mY1;
#ifdef ENABLE_WIDGET_HIGHLIGHTING
    TextureRegion *mHighlightedRegion;
    Sprite *mHighlightedSprite;
    bool mHighlighted;
#endif
public:
    Widget(int x, int y, int w, int h);

    virtual ~Widget();

    // Forwards to mListener if coords overlap
    virtual bool onTapEvent(TapEvent *event);

    void setTapListener(TapListener *l)
    {
        mListener = l;
    }

    // Caller takes ownership of image
    virtual Image *getImage() = 0;

#ifdef ENABLE_WIDGET_HIGHLIGHTING
    virtual Image *getHighlightedImage() = 0;

    // Takes ownership of regions
    virtual void setTextureRegions(RenderContext *rc,
            TextureRegion *region, TextureRegion *highlighted_region);

    void setHighlighted(bool highlight = true)
    {
        mHighlighted = highlight;
    }
#else
    // Takes ownership of region
    virtual void setTextureRegion(RenderContext *rc, TextureRegion *region);
#endif

    virtual void render(RenderContext *rc);

    int getLeft() const
    {
        return mX0;
    }

    int getRight() const
    {
        return mX1;
    }

    int getTop() const
    {
        return mY0;
    }

    int getBottom() const
    {
        return mY1;
    }

    int getWidth() const
    {
        return mX1 - mX0;
    }

    int getHeight() const
    {
        return mY1 - mY0;
    }
};

}

#endif // HGAME_WIDGET_H
