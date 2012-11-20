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

#include "hsdl/GLSprite.h"
#include "hsdl/GLTextureAtlas.h"
#include "hsdl/GLTileBatcher.h"
#include "hsdl/Image.h"
#include "hsdl/Log.h"

namespace hsdl {

GLRenderContext::GLRenderContext(int *best_modes) :
        hgl::GLRenderContext(new hsdl::Log("GLRenderContext"))
{
    SDL_Rect **modes = SDL_ListModes(0, SDL_FULLSCREEN | SDL_HWSURFACE);
    int w, h;   // Actual mode
    int vw = 0, vh = 0; // Viewport size
    bool scaling = true;
    if (!modes)
    {
        THROW(hgame::Throwable, "No screen modes available");
    }
    else if ((long) modes == -1)
    {
        vw = w = best_modes[0];
        vh = h = best_modes[1];
        scaling = false;
    }
    else
    {
        int n;
        mLog.v("Available modes:");
        for (n = 0; modes[n]; ++n)
        {
            mLog.v("  %dx%d", modes[n]->w, modes[n]->h);
        }
        int best_w = 0;
        int best_h = 0;
        int best_vh = 0;
        int highest_w = 0;
        int highest_h = 0;
        int m;
        bool exact = false;
        // Try to find highest resolution that doesn't need scaling
        for (m = 0; best_modes[2 * m]; ++m)
        {
            for (n = 0; modes[n]; ++n)
            {
                int tvh;
                int cvh;
                if (modes[n]->w >= highest_w && modes[n]->h >= highest_h)
                {
                    highest_w = modes[n]->w;
                    highest_h = modes[n]->h;
                }
                if ((float) modes[n]->w / (float) modes[n]->h >=
                    (float) best_modes[2 * m] / (float) best_modes[2 * m + 1])
                {
                    // Test height
                    cvh = modes[n]->h;
                    tvh = best_modes[2 * m + 1];
                }
                else
                {
                    // Test width
                    cvh = modes[n]->w;
                    tvh = best_modes[2 * m];
                }
                if (cvh == tvh)
                {
                    if (!exact || modes[n]->h > best_h)
                    {
                        vw = best_w = modes[n]->w;
                        vh = best_h = modes[n]->h;
                        best_vh = cvh;
                        exact = true;
                        scaling = false;
                    }
                }
                else if (!exact && ((tvh == 1080 && cvh > 1080 && cvh <= 1200)
                        || (tvh == 720 && cvh > 720 && cvh <= 800)))

                {
                    if (scaling || modes[n]->h > best_h)
                    {
                        vw = best_w = modes[n]->w;
                        vh = best_h = modes[n]->h;
                        best_vh = cvh;
                        scaling = false;
                    }
                }
                else if (scaling && cvh > best_vh)
                {
                    best_w = modes[n]->w;
                    best_h = modes[n]->h;
                    best_vh = cvh;
                }
            }
            if (!scaling)
                break;
        }
        if (!best_h)
        {
            w = highest_w;
            h = highest_h;
        }
        else
        {
            w = best_w;
            h = best_h;
        }
    }
    // Use windowed mode for now, easier to test and debug
    w = 960;
    h = 720;
    scaling = false;

    if (!vw)
    {
        int m;
        for (m = 0; best_modes[2 * m]; ++m)
        {
            if (best_modes[2 * m] <= w && best_modes[2 * m + 1] <= h)
                break;
        }
        if (!best_modes[2 * m])
            m = 0;
        vw = best_modes[2 * m];
        vh = best_modes[2 * m + 1];
    }
    if (vw > w || vh > h)
    {
        float game_aspect = (float) vw / (float) vh;
        if ((float) w / (float) h >= game_aspect)
        {
            // Screen is wider than game
            vh = h;
            vw = int((float) vh * game_aspect);

        }
        else
        {
            // Screen is taller than game
            vw = w;
            vh = int((float) vw / game_aspect);
        }
    }
    mLog.i("Using video mode %dx%d, viewport %dx%d", w, h, vw, vh);

    //initSurface(w, h, vw, vh, scaling ? 2 : 0);
    // Can't find GLX visual if AA is enabled: intel driver "feature"?
    initSurface(w, h, vw, vh, 0);
}

void GLRenderContext::initSurface(int w, int h, int vw, int vh, int antialias)
{
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    //SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, antialias ? 1 : 0);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, antialias);
    mSurface = SDL_SetVideoMode(w, h, 32,
            SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL /* | SDL_FULLSCREEN */);
    initGL(w, h, vw, vh);
}

hgame::TextureAtlas *GLRenderContext::uploadTexture(hgame::Image *img)
{
    SDL_Surface *surf = (dynamic_cast<Image *>(img))->getSurface();
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
    GLTextureAtlas *atlas = new GLTextureAtlas(this, w, h,
            getNeedScaling() ? GL_LINEAR : GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, bpp, w, h, 0, tex_fmt, GL_UNSIGNED_BYTE,
            surf->pixels);
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

void GLRenderContext::flipBuffers()
{
    SDL_GL_SwapBuffers();
}

}
