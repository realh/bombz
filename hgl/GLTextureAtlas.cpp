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

#include "hgl/GLTextureAtlas.h"

#include <cstdio>
using namespace std;

namespace hgl {

int GLTextureAtlas::getWidth() const
{
    return mWidth;
}

int GLTextureAtlas::getHeight() const
{
    return mHeight;
}

GLTextureAtlas::GLTextureAtlas(GLRenderContext *rc, int w, int h,
        int scale_method) :
        mWidth(w), mHeight(h)
{
    fprintf(stderr, "Created GLTextureAtlas %p (hgame %p)\n",
            this, (const hgame::TextureAtlas *) this);
    glGenTextures(1, &mTextureId);
    rc->bindTexture((const hgame::TextureAtlas *) this);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, scale_method);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, scale_method);
}

GLTextureAtlas::~GLTextureAtlas()
{
    glDeleteTextures(1, &mTextureId);
}

}
