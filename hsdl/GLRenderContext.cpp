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

#include <cstdlib>

#include "hsdl/GLRenderContext.h"

#include "hgame/Log.h"

#include "hgl/GLTextureAtlas.h"

#include "hsdl/GLSprite.h"
#include "hsdl/GLTileBatcher.h"
#include "hsdl/Image.h"

namespace hsdl {

GLRenderContext::GLRenderContext() :
        hgl::GLRenderContext(new hgame::Log("GLRenderContext"))
{
    SDL_Rect **modes = SDL_ListModes(0, SDL_FULLSCREEN | SDL_HWSURFACE);
    int w, h;
    if (!modes)
    {
        THROW(hgame::Throwable, "No modes available");
    }
    else if ((long) modes == -1)
    {
        w = 1920;
        h = 1080;
    }
    else
    {
        int n;
        mLog.d("Available modes:");
        for (n = 0; modes[n]; ++n)
        {
            mLog.d("  %dx%d", modes[n]->w, modes[n]->h);
        }
    }
    std::exit(0);
}

hgame::TextureAtlas *GLRenderContext::uploadTexture(hgame::Image *img)
{
    SDL_Surface *surf = ((Image *) img)->getSurface();
    int w = surf->w;
    int h = surf->h;
    GLint bpp = surf->format->BytesPerPixel;
    GLenum tex_fmt;
    if (bpp == 4)
    {
        if (surf->format->Rmask == 0xff)
            tex_fmt = GL_RGBA;
        else
            tex_fmt = GL_BGRA;
    }
    else if (bpp == 3)
    {
        if (surf->format->Rmask == 0xff)
            tex_fmt = GL_RGB;
        else
            tex_fmt = GL_BGR;
    }
    else
    {
        THROW(hgame::Throwable, "Unsupported bytes per pixel: %d", bpp);
    }
    hgl::GLTextureAtlas *atlas = new hgl::GLTextureAtlas(this, w, h,
            getNeedScaling() ? GL_LINEAR : GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, bpp, w, h, 0, tex_fmt, GL_UNSIGNED_BYTE,
            surf->pixels);
    delete img;
    return (hgame::TextureAtlas *) atlas;
}

hgame::Sprite *GLRenderContext::createSprite(hgame::TextureRegion *texture,
            int width, int height)
{
    return new GLSprite(this, texture, width, height);
}

hgame::TileBatcher *GLRenderContext::createTileBatcher(int nColumns, int nRows,
        int tile_size)
{
    return new GLTileBatcher(this, nColumns, nRows, tile_size);
}

}
