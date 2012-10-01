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

// Types.h: Common simple types

#ifndef HGAME_TYPES_H
#define HGAME_TYPES_H

#include "config.h"

#include <climits>

#if __GNUC__ >= 4
#define H_GNUC_NULL_TERMINATED __attribute__((__sentinel__))
#else
#define H_GNUC_NULL_TERMINATED
#endif
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
#define H_GNUC_PRINTF(format_idx, arg_idx)    \
    __attribute__((__format__ (__printf__, format_idx, arg_idx)))
#else /* !__GNUC__ */
#define H_GNUC_PRINTF(format_idx, arg_idx)
#endif

namespace hgame {

#if ULONG_MAX == 0xffffffffffffffffUL
typedef long HInt64;
typedef unsigned long HUInt64;
#else
typedef long long HInt64;
typedef unsigned long long HUInt64;
#endif
typedef int HInt32;
typedef unsigned int HUInt32;
typedef short HInt16;
typedef unsigned short HUInt16;
typedef char HInt8;
typedef unsigned char HUInt8;

class Colour {
protected:
    HUInt32 mRGBA;
public:
    Colour(HUInt32 rgba = 0) : mRGBA(rgba) {}

    Colour(HUInt8 red, HUInt8 green, HUInt8 blue,
            HUInt8 alpha)
    {
        setRGBA(red, green, blue, alpha);
    }

    void setRGBA(HUInt32 rgba)
    {
        mRGBA = rgba;
    }

    void setRGBA(HUInt8 red, HUInt8 green, HUInt8 blue,
            HUInt8 alpha)
    {
        mRGBA = (((HUInt32) red) << 24) |
                (((HUInt32) green) << 16) |
                (((HUInt32) blue) << 8) |
                ((HUInt32) alpha);
    }

    void setRed(HUInt8 red)
    {
        mRGBA = (mRGBA & ~0xff000000) | (((HUInt32) red) << 24);
    }

    void setGreen(HUInt8 green)
    {
        mRGBA = (mRGBA & ~0xff0000) | (((HUInt32) green) << 16);
    }

    void setBlue(HUInt8 blue)
    {
        mRGBA = (mRGBA & ~0xff00) | (((HUInt32) blue) << 8);
    }

    void setAlpha(HUInt8 alpha)
    {
        mRGBA = (mRGBA & ~0xff) | (HUInt32) alpha;
    }

    HUInt32 getRGBA() const
    {
        return mRGBA;
    }

    HUInt8 getRed() const
    {
        return mRGBA >> 24;
    }

    HUInt8 getGreen() const
    {
        return (mRGBA >> 16) & 0xff;
    }

    HUInt8 getBlue() const
    {
        return (mRGBA >> 8) & 0xff;
    }

    HUInt8 getAlpha() const
    {
        return mRGBA & 0xff;
    }
};

}

#endif // HGAME_TYPES_H
