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

#include "hsdl/Exception.h"
#include "hsdl/Font.h"
#include "hsdl/Image.h"

#include <cstdio>

namespace hsdl {

const char *Font::smPath = 0;

hgame::Image *Font::render(hgame::Colour colour, const char *text)
{
    SDL_Color sc;
    sc.r = colour.getRed();
    sc.g = colour.getGreen();
    sc.b = colour.getBlue();
    return new Image(TTF_RenderUTF8_Blended(mFont, text, sc));
}
    
Font::Font(unsigned int px)
{
    if (!smPath)
    {
        if (TTF_Init())
        {
            THROW(Exception, "Unable to initialise font system");
        }
        smPath = WITH_FONT;
    }
    mFont = TTF_OpenFont(smPath, px);
    if (!mFont)
    {
        THROW(Exception, "Unable to load font '%s' at size %d", smPath, px);
    }
}
    
Font::~Font()
{
    TTF_CloseFont(mFont);
}

}
