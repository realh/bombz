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

#include "hsdl/Image.h"

#include "hgame/Log.h"
#include "hgame/Platform.h"

#include "hsdl/Exception.h"

namespace hsdl {

Image::~Image()
{
    SDL_FreeSurface(mSurface);
}
    
int Image::getWidth() const
{
    return mSurface->w;
}

int Image::getHeight() const
{
    return mSurface->h;
}
    
hgame::Image *Image::createImage(int w, int h)
{
    return 0;
}

unsigned int Image::getAlphaAt(int x, int y) const
{
    return 0;
}

void Image::setAlphaAt(int x, int y, unsigned int alpha)
{
}

void Image::blit(hgame::Image *src, int dest_x, int dest_y,
        int src_x, int src_y, int w, int h)
{
    blit((Image *) src, dest_x, dest_y, src_x, src_y, w, h);
}

void Image::blit(Image *src, int dest_x, int dest_y,
        int src_x, int src_y, int w, int h)
{
}

}
