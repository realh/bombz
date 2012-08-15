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
    Uint32 rm, gm, bm, am;
    SDL_PixelFormat *fmt = mSurface->format;
    if (fmt->BitsPerPixel == 32)
    {
        rm = fmt->Rmask;
        gm = fmt->Gmask;
        bm = fmt->Bmask;
        am = fmt->Amask;
    }
    else
    {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rm = 0xff000000;
        gm = 0x00ff0000;
        bm = 0x0000ff00;
        am = 0x000000ff;
#else
        rm = 0x000000ff;
        gm = 0x0000ff00;
        bm = 0x00ff0000;
        am = 0xff000000;
#endif
    }
    SDL_Surface *surf = SDL_CreateRGBSurface(SDL_SRCALPHA, w, h, 32,
            rm, gm, bm, am);
    return new Image(surf);
}

unsigned int Image::getAlphaAt(int x, int y)
{
    SDL_PixelFormat *fmt = mSurface->format;
    return ((getPixelRawValue(x, y) & fmt->Amask) >> fmt->Ashift) << fmt->Aloss;
}

void Image::setAlphaAt(int x, int y, unsigned int alpha)
{
    SDL_PixelFormat *fmt = mSurface->format;
    void *ppix;
    Uint32 pix = (getPixelRawValue(x, y, &ppix) & ~fmt->Amask) |
            (((alpha >> fmt->Aloss) << fmt->Ashift) & fmt->Amask);
    if (fmt->BitsPerPixel >= 24)
    {
        *((Uint32 *) ppix) = pix;
    }
    else if (fmt->BitsPerPixel >= 15)
    {
        *((Uint16 *) ppix) = pix;
    }
    else
    {
        *((Uint8 *) ppix) = pix;
    }
}

void Image::blit(hgame::Image *src, int dest_x, int dest_y,
        int src_x, int src_y, int w, int h)
{
    blit((Image *) src, dest_x, dest_y, src_x, src_y, w, h);
}

void Image::blit(Image *src, int dest_x, int dest_y,
        int src_x, int src_y, int w, int h)
{
    SDL_Rect src_rect;
    SDL_Rect dest_rect;
    src_rect.x = src_x;
    src_rect.y = src_y;
    src_rect.w = w;
    src_rect.h = h;
    dest_rect.x = dest_x;
    dest_rect.y = dest_y;
    dest_rect.w = w;
    dest_rect.h = h;
    if (SDL_BlitSurface(src->mSurface, &src_rect, mSurface, &dest_rect))
    {
        THROW(Exception, "Image blit failed");
    }
}

void *Image::getPixelAddr(int x, int y)
{
    return (void *) (((char *) mSurface->pixels) + mSurface->pitch * y +
            x * mSurface->format->BytesPerPixel);
}

Uint32 Image::getPixelRawValue(int x, int y, void **pAddr)
{
    SDL_PixelFormat *fmt = mSurface->format;
    void *ppix = getPixelAddr(x, y);
    if (pAddr)
        *pAddr = ppix;
    if (fmt->BitsPerPixel >= 24)
    {
        return *((Uint32 *) ppix);
    }
    else if (fmt->BitsPerPixel >= 15)
    {
        return (Uint32) *((Uint16 *) ppix);
    }
    return (Uint32) *((Uint8 *) ppix);
}

void Image::lock()
{
    if (SDL_LockSurface(mSurface))
    {
        THROW(Exception, "Unable to lock surface");
    }
}

void Image::unlock()
{
    SDL_UnlockSurface(mSurface);
}

}
