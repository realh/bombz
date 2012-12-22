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

// Image.h: Wrapper for SDL_Surface

#ifndef HSDL_IMAGE_H
#define HSDL_IMAGE_H

#include "config.h"

#include "SDL.h"

#include "hgame/Image.h"

#include "hsdl/Types.h"

namespace hsdl {

class Image : public hgame::Image {
private:
    SDL_Surface *mSurface;
public:
    Image(SDL_Surface *surf) : mSurface(surf) {}
    ~Image();

    int getWidth() const;
    int getHeight() const;

    // Use with care!
    SDL_Surface *getSurface()
    {
        return mSurface;
    }

    Colour getColourAt(int x, int y);
    void setColourAt(int x, int y, Colour colour);

    HUInt8 getAlphaAt(int x, int y);
    void setAlphaAt(int x, int y, HUInt8 alpha);

    Image *createImage(int w, int h);

    // SDL_BlitSurface is broken
    /*
    void blit(hgame::Image *src, int dest_x, int dest_y,
            int src_x, int src_y, int w, int h);
    */

    void lock();
    void unlock();
private:
    void *getPixelAddr(int x, int y);
    HUint32 getPixelRawValue(int x, int y, void **pAddr = 0);
    void setPixelRawValue(int x, int y, HUInt32 pix);
    void setPixelRawValue(void *addr, HUInt32 pix);
};

}

#endif // HSDL_IMAGE_H
