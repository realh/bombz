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

// TextureRegion.h: A region within a texture atlas

#ifndef HGAME_TEXTURE_REGION_H
#define HGAME_TEXTURE_REGION_H

#include "config.h"

namespace hgame {

class TextureAtlas;

class TextureRegion {
protected:
    // Coordinates are normalised to 0.0-1.0 with origin at top-left
    TextureAtlas *mAtlas;
    float mU0, mV0, mU1, mV1;
public:
    TextureRegion(TextureAtlas *atlas, float u0, float v0, float u1, float v1) :
            mAtlas(atlas), mU0(u0), mV0(v0), mU1(u1), mV1(v1)
            {}

    // Coordinates are in pixel units in atlas source's space
    // with origin at top-left
    TextureRegion(TextureAtlas *atlas, int x, int y, int w, int h);

    virtual ~TextureRegion();

    inline float getU0() const { return mU0; }

    inline float getV0() const { return mV0; }

    inline float getU1() const { return mU1; }

    inline float getV1() const { return mV1; }

    inline const TextureAtlas *getAtlas() const { return mAtlas; }
};

}

#endif // HGAME_TEXTURE_REGION_H