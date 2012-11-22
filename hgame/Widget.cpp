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

#include "hgame/Widget.h"

namespace hgame {

Widget::Widget(int x, int y, int w, int h) :
        mListener(0), mRegion(0), mSprite(0),
        mX0(x), mY0(y), mX1(x + w), mY1(y + h)
#if ENABLE_WIDGET_HIGHLIGHTING
        , mHighlightedRegion(0), mHighlightedSprite(0),
        mHighlighted(false)
#endif
{}

Widget::~Widget()
{
#if ENABLE_WIDGET_HIGHLIGHTING
    delete mHighlightedSprite;
    delete mHighlightedRegion;
#endif
    delete mSprite;
    delete mRegion;
}

bool Widget::onTapEvent(TapEvent *event)
{
    int x = event->getX();
    int y = event->getY();
    if (mListener && x >= mX0 && x < mX1 && y >= mY0 && y < mY1)
        return mListener->onTapEvent(event);
    return false;
}

#if ENABLE_WIDGET_HIGHLIGHTING
void Widget::setTextureRegions(RenderContext *rc,
        TextureRegion *region, TextureRegion *highlighted_region)
{
    mRegion = region;
    mHighlightedRegion = highlighted_region;
    mSprite = rc->createSprite(region, mX1 - mX0, mY1 - mY0);
    mSprite->setPosition(mX0, mY0);
    mHighlightedSprite = rc->createSprite(highlighted_region,
            mX1 - mX0, mY1 - mY0);
    mHighlightedSprite->setPosition(mX0, mY0);
}
#else
void Widget::setTextureRegion(RenderContext *rc, TextureRegion *region)
{
    mRegion = region;
    mSprite = rc->createSprite(region, mX1 - mX0, mY1 - mY0);
    mSprite->setPosition(mX0, mY0);
}
#endif

void Widget::render(RenderContext *rc)
{
#if ENABLE_WIDGET_HIGHLIGHTING
    if (mHighlighted)
        mHighlightedSprite->render(rc);
    else
#endif
        mSprite->render(rc);
}

}
