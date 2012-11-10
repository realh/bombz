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

// GLRenderContext.h: Texture-based rendering with OpenGL in SDL

#ifndef HSDL_GL_RENDER_CONTEXT_H
#define HSDL_GL_RENDER_CONTEXT_H

#include "config.h"

#include "SDL.h"
#include "SDL_opengl.h"

#include "hgl/GLRenderContext.h"

namespace hsdl {

class GLRenderContext : public hgl::GLRenderContext {
private:
    SDL_Surface *mSurface;
public:
    // See hgame/Screen.h for an explanation of best_modes
    GLRenderContext(int *best_modes);
    hgame::TextureAtlas *uploadTexture(hgame::Image *img);
    hgame::Sprite *createSprite(hgame::TextureRegion *texture,
            int width, int height);
    hgame::TileBatcher *createTileBatcher(int nColumns, int nRows,
            int tile_size);
    void flipBuffers();
private:
    void initSurface(int w, int h, int vw, int vh, int antialias = 0);
};

}

#endif // HSDL_GL_RENDER_CONTEXT_H
