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

#include "hgl/GLTextureRegion.h"

#include "hsdl/GLTextureAtlas.h"
#include "hsdl/GLTextureRegion.h"

namespace hsdl {

GLTextureRegion::GLTextureRegion(GLTextureAtlas *atlas,
            float u0, float v0, float u1, float v1) :
        hgl::GLTextureRegion((hgl::GLTextureAtlas *) atlas, u0, v0, u1, v1)
{
    initCoords();
}

GLTextureRegion::GLTextureRegion(GLTextureAtlas *atlas,
        int x, int y, int w, int h) :
        hgl::GLTextureRegion((hgl::GLTextureAtlas *) atlas, x, y, w, h)
{
    initCoords();
}

void GLTextureRegion::initCoords()
{
    // These coords are for a GL_QUAD, see
    // http://immersedcode.org/2011/4/7/sdl-surface-to-texture/
    // Top-left, bottom-left, bottom-right, top-right
    mCoords = new float[8];
    mCoords[0] = mU0;
    mCoords[1] = mV0;
    mCoords[2] = mU0;
    mCoords[3] = mV1;
    mCoords[4] = mU1;
    mCoords[5] = mV1;
    mCoords[6] = mU1;
    mCoords[7] = mV0;
}

}
