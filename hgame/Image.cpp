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

#include "hgame/Image.h"

#include <cmath>
//#include <cstdio>

namespace hgame {

#if ENABLE_SHADOWS

double Image::smGaussTable[kSpan * 2 + 1][kSpan * 2 + 1];

void Image::fillGaussTable()
{
    int j = 0;
    double total = 0.0;
    for (int y = -kSpan; y <= kSpan; ++y, ++j)
    {
        int i = 0;
        for (int x = -kSpan; x <= kSpan; ++x, ++i)
        {
            smGaussTable[j][i] = pow(kE,
                (double) (-x * x -y * y) / 2.0 * kRho * kRho) /
                sqrt(2.0 * kPi * kRho * kRho);
            total += smGaussTable[j][i];
        }
    }
    total /= kOpacity;
    j = 0;
    for (int y = -kSpan; y <= kSpan; ++y, ++j)
    {
        int i = 0;
        for (int x = -kSpan; x <= kSpan; ++x, ++i)
        {
            smGaussTable[j][i] /= total;
        }
    }
    
    /*
    for (j = 0; j <= 2 * kSpan; ++j)
    {
        for (int i = 0; i <= 2 * kSpan; ++i)
        {
            std::printf("%10f ", smGaussTable[j][i]);
        }
        std::printf("\n");
    }
    printf("\n%f\n", total);
    */
}

Image *Image::createShadow(int offset)
{
    static bool filled_table = false;
    if (!filled_table)
    {
        fillGaussTable();
        filled_table = true;
    }
    int extra, offset0;
    if (offset < kSpan)
    {
        extra = offset + kSpan + kSpan - offset;
        offset = kSpan;
        offset0 = kSpan - offset;
    }
    else
    {
        extra = offset + kSpan;
        offset0 = 0;
    }
    int w = getWidth();
    int h = getHeight();
    Image *shadow = createImage(w + extra, h + extra);
    lock();
    shadow->lock();
    for (int y0 = -kSpan; y0 <= h + kSpan; ++y0)
    {
        for (int x0 = -kSpan; x0 <= w + kSpan; ++x0)
        {
            double alpha = 0.0;
            int j = 0;
            for (int y1 = -kSpan; y1 <= kSpan; ++y1, ++j)
            {
                int y = y0 + y1;
                if (y < 0)
                    continue;
                else if (y >= h)
                    break;
                int i = 0;
                for (int x1 = -kSpan; x1 <= kSpan; ++x1, ++i)
                {
                    int x = x0 + x1;
                    if (x < 0)
                        continue;
                    else if (x >= w)
                        break;
                    alpha += (double) getAlphaAt(x, y) / 255.0 *
                            smGaussTable[j][i];
                }
            }
            shadow->setAlphaAt(x0 + offset, y0 + offset,
                    (HUInt8) (alpha * 255.0));
        }
    }
    shadow->unlock();
    unlock();
    shadow->blit(this, offset0, offset0, 0, 0, w, h);
    return shadow;
}

#endif

Image::~Image()
{
}

void Image::blit(Image *src, int dest_x, int dest_y,
            int src_x, int src_y, int w, int h)
{
    int src_w = src->getWidth();
    int src_h = src->getHeight();
    int dest_w = getWidth();
    int dest_h = getHeight();
    
    if (dest_x >= dest_w || dest_y > dest_h || src_x >= src_w || src_y >= src_h)
        return;
    if (src_x + w > src_w)
        w = src_w - src_x;
    if (src_y + h > src_h)
        h = src_h - src_y;
    if (dest_x + w > dest_w)
        w = dest_w - dest_x;
    if (dest_y + h > dest_h)
        h = dest_h - dest_y;
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            Colour src_c = src->getColourAt(src_x + x, src_y + y);
            Colour dest_c = getColourAt(dest_x + x, dest_y + y);
            HUInt32 src_a = src_c.getAlpha();
            HUInt32 dest_a = dest_c.getAlpha();
            dest_c.setRed(src_c.getRed() * src_a / 255 +
                    dest_c.getRed() * dest_a / 255 * (255 - dest_a) / 255);
            dest_c.setGreen(src_c.getGreen() * src_a / 255 +
                    dest_c.getGreen() * dest_a / 255 * (255 - dest_a) / 255);
            dest_c.setBlue(src_c.getBlue() * src_a / 255 +
                    dest_c.getBlue() * dest_a / 255 * (255 - dest_a) / 255);
            dest_c.setAlpha(src_a + (dest_a * (255 - src_a)) / 255);
            setColourAt(dest_x + x, dest_y + y, dest_c);
        }
    }
}
            
}
