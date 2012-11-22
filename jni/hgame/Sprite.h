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

// Sprite: A textured quadrilateral

#ifndef HGAME_SPRITE_H
#define HGAME_SPRITE_H

#include "config.h"

#include "hgame/RenderContext.h"
#include "hgame/TextureRegion.h"

namespace hgame {

class Sprite {
protected:
    TextureRegion *mTexture;
    int mW, mH;             // size
public:
    // All dimensions are in viewport space
    Sprite(TextureRegion *texture, int width, int height) :
            mTexture(texture), mW(width), mH(height)
            {}
    virtual ~Sprite();

    virtual void setPosition(int x, int y) = 0;

    // Calling function must set everything up first, including
    // binding the texture if necessary
    virtual void render(RenderContext *rc) = 0;

    inline void bind(RenderContext *rc)
    {
        rc->bindTexture(mTexture->getAtlas());
    }

    inline void setTexture(TextureRegion *tex)
    {
        mTexture = tex;
    }
};

}

#endif // HGAME_SPRITE_H
