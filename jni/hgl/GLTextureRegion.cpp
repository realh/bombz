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

#include "config.h"

#include "hgl/GLTextureAtlas.h"
#include "hgl/GLTextureRegion.h"

namespace hgl {

GLTextureRegion::GLTextureRegion(GLTextureAtlas *atlas,
            float u0, float v0, float u1, float v1) :
        hgame::TextureRegion((hgame::TextureAtlas *) atlas, u0, v0, u1, v1),
        mCoords(0)
{
}

GLTextureRegion::GLTextureRegion(GLTextureAtlas *atlas,
        int x, int y, int w, int h) :
        // We're going to overwrite U/V so init them quickest way possible
        hgame::TextureRegion((hgame::TextureAtlas *) atlas,
                0.0f, 0.0f, 0.0f, 0.0f)
{
    // Note corners are brought inwards by half a pixel to prevent
    // artifacts at edges
    float aw = (float) atlas->getWidth();
    float ah = (float) atlas->getHeight();
    mU0 = ((float) x + 0.5) / aw;
    mV0 = ((float) y + 0.5) / ah;
    mU1 = mU0 + (float) (w - 1) / aw;
    mV1 = mV0 + (float) (h - 1) / ah;
}

GLTextureRegion::~GLTextureRegion()
{
    delete[] mCoords;
}

}
