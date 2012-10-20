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

#include "hgame/TextWidget.h"

namespace hgame {

TextWidget::TextWidget(const char *text, Font *font, Colour colour,
        int x, int y, Alignment align, int shadow_offset) :
        Widget(x, y, x, y)
{
    mImage = font->render(colour, text);
#if ENABLE_SHADOW
    if (shadow_offset)
    {
        Image *img = mImage;
        mImage = img->createShadow(shadow_offset);
        delete img;
    }
#endif
    int w = mImage->getWidth();
    int h = mImage->getHeight();
    switch (align & ALIGN_HMASK)
    {
        case ALIGN_RIGHT:
            mX0 = x - w;
            break;
        case ALIGN_CENTRE:
            mX0 -= w / 2;
            mX1 = mX0 + w;
            break;
        default:
            mX1 = x + w;
            break;
    }
    switch (align & ALIGN_VMASK)
    {
        case ALIGN_BOTTOM:
            mY0 = y - h;
            break;
        case ALIGN_CENTRE:
            mY0 -= h / 2;
            mY1 = mY0 + h;
            break;
        default:
            mY1 = y + h;
            break;
    }
}

TextWidget::~TextWidget()
{
    delete mImage;
}

Image *TextWidget::getImage()
{
    return mImage;
}

}
