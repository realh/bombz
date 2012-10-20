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

// See http://en.wikipedia.org/wiki/Triangle_strip

#include "hsdl/GLSprite.h"

#include "hgl/GLRenderContext.h"
#include "hgl/GLTextureAtlas.h"
#include "hgl/GLTextureRegion.h"

namespace hsdl {

GLSprite::GLSprite(GLRenderContext *rc, hgame::TextureRegion *texture,
            int width, int height) :
            hgame::Sprite(texture, width, height),
            mRc(rc)
{
}

void GLSprite::setPosition(int x, int y)
{
    mVertices[0] = (float) x;
    mVertices[1] = (float) y;
    mVertices[2] = (float) x;
    mVertices[3] = (float) (y + mH);
    mVertices[4] = (float) (x + mW);
    mVertices[5] = (float) y;
    mVertices[6] = (float) (x + mW);
    mVertices[7] = (float) (y + mH);
}

void GLSprite::render(hgame::RenderContext *rc)
{
    glVertexPointer(2, GL_FLOAT, 0, mVertices);
    glTexCoordPointer(2, GL_FLOAT, 0,
            ((hgl::GLTextureRegion *) mTexture)->getCoords());
    glDrawArrays(GL_QUADS, 0, 4);
}

}
