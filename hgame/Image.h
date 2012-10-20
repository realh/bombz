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

// Image.h: Some sort of bitmap surface that can be used to upload
//          textures to OpenGL

#ifndef HGAME_IMAGE_H
#define HGAME_IMAGE_H

#include "config.h"

#include "hgame/Types.h"

namespace hgame {

class Image {
#if ENABLE_SHADOWS
private:
    static const double kPi = 3.1415926535;
    static const double kE = 2.718281828;
    static const double kRho = 0.4;
    static const int kSpan = 3;
    static const double kOpacity = 0.575;
    static double smGaussTable[kSpan * 2 + 1][kSpan * 2 + 1];
    static void fillGaussTable();
#endif
public:
    // Create a new blank image, 32-bit with alpha
    virtual Image *createImage(int w, int h) = 0;

    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;

    virtual Colour getColourAt(int x, int y) = 0;
    virtual void setColourAt(int x, int y, Colour colour) = 0;

    // Used for creating drop shadows; alpha in range 0-255
    virtual HUInt8 getAlphaAt(int x, int y) = 0;
    virtual void setAlphaAt(int x, int y, HUInt8 alpha) = 0;

    // May be necessary for direct access to pixel data
    virtual void lock() = 0;
    virtual void unlock() = 0;

    // Copies part of one image into current image
    virtual void blit(Image *src, int dest_x, int dest_y,
            int src_x, int src_y, int w, int h);

    virtual ~Image();

#if ENABLE_SHADOWS
    // Create a new image which is copy of current one with drop shadow
    Image *createShadow(int offset);
#endif
};

}

#endif // HGAME_IMAGE_H
