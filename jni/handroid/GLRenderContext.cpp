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

#include "handroid/GLRenderContext.h"

#include "hgles/GLSprite.h"
#include "hgles/GLTextureAtlas.h"
#include "hgles/GLTileBatcher.h"

#include "handroid/Image.h"
#include "handroid/Log.h"

namespace hsdl {

GLRenderContext::GLRenderContext(Platform *platform, int *best_modes) :
        hgl::GLRenderContext(new handroid::Log("GLRenderContext")),
        mPlatform(platform),
        mDisplay(EGL_NO_DISPLAY),
        mSurface(EGL_NO_CONTEXT),
        mContext(EGL_NO_SURFACE)
{
    EGLint format, n_configs, result;
    EGLConfig config;
    EGLint attrs[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
        EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_NONE
    };

    // Get and initialise display
    if ((mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY)
    {
        THROW(hgame::Throwable, "Unable to connect to OpenGL display");
    }
    if (!eglInitialize(mDisplay, NULL, NULL))
    {
        THROW(hgame::Throwable, "Unable to initialise OpenGL display");
    }
    // Get config
    if(!eglChooseConfig(mDisplay, attrs, &config, 1, &n_configs) ||
            (n_configs <= 0))
    {
        mLog.w("Unable to get 24-/32-bit OpenGL config, trying 16-bit");
        attrs[3] = attrs[7] = 5;
        attrs[5] = 6;
        if(!eglChooseConfig(mDisplay, attrs, &config, 1, &n_configs) ||
                (n_configs <= 0))
        {
            THROW(hgame::Throwable, "Unable to get OpenGL config");
        }
    }
    // Configure window
    if (!eglGetConfigAttrib(mDisplay, config, EGL_NATIVE_VISUAL_ID, &format))
    {
        THROW(hgame::Throwable, "Unable to configure OpenGL window format");
    }
    ANativeWindow *window = mPlatform->getNativeWindow();
    ANativeWindow_setBuffersGeometry(window, 0, 0, format);
    // Create surface and context
    mSurface = eglCreateWindowSurface(mDisplay, lConfig, window, 0);
    if (mSurface == EGL_NO_SURFACE)
    {
        THROW(hgame::Throwable, "Unable to create OpenGL surface");
    }
    mContext = eglCreateContext(mDisplay, lConfig, EGL_NO_CONTEXT, 0);
    if (mContext == EGL_NO_CONTEXT)
    {
        THROW(hgame::Throwable, "Unable to create OpenGL context");
    }
    // Activate display
    if (!eglMakeCurrent(mDisplay, mSurface, mSurface, mContext)
            || !eglQuerySurface(mDisplay, mSurface, EGL_WIDTH, &mWidth)
            || !eglQuerySurface(mDisplay, mSurface, EGL_HEIGHT, &mHeight)
            || (mWidth <= 0) || (mHeight <= 0))
    {
        THROW(hgame::Throwable, "Unable to activate OpenGL display");
    }
}

GLRenderContext::~GLRenderContext()
{
    if (mDisplay != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(mDisplay,
                EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (mContext != EGL_NO_CONTEXT)
        {
            eglDestroyContext(mDisplay, mContext);
            mContext = EGL_NO_CONTEXT;
        }
        if (mSurface != EGL_NO_SURFACE)
        {
            eglDestroySurface(mDisplay, mSurface);
            mSurface = EGL_NO_SURFACE;
        }
        eglTerminate(mDisplay);
        mDisplay = EGL_NO_DISPLAY;
    }
}

hgame::TextureAtlas *GLRenderContext::uploadTexture(hgame::Image *img)
{
    Image *aimg = dynamic_cast<Image *>(img);
    int w = surf->w;
    int h = surf->h;
    GLint bpp = 0;
    GLenum tex_fmt = GL_RGBA;
    UInt32 ifmt = aimg->getFormat();
    switch (ifmt)
    {
        case ANDROID_BITMAP_FORMAT_RGBA_8888:
            bpp = 4;
            tex_fmt = GL_RGBA;
            break;
        case ANDROID_BITMAP_FORMAT_RGB_565:
            bpp = 2;
            tex_fmt = GL_RGB;
            break;
        case ANDROID_BITMAP_FORMAT_RGB_4444:
            bpp = 2;
            tex_fmt = GL_RGBA;
            break;
        default:
            THROW(hgame::Throwable,
                    "Unsupported Android bitmap format for textures: %d", ifmt);
            break;
    }
    hgles::GLTextureAtlas *atlas = new hgles::GLTextureAtlas(this, w, h,
            getNeedScaling() ? GL_LINEAR : GL_NEAREST);
    aimg->lock();
    glTexImage2D(GL_TEXTURE_2D, 0, bpp, w, h, 0, tex_fmt, GL_UNSIGNED_BYTE,
            aimg->getRawPixelData());
    aimg->unlock();
    return atlas;
}

hgame::Sprite *GLRenderContext::createSprite(hgame::TextureRegion *texture,
            int width, int height)
{
    return new hgles::GLSprite(this, texture, width, height);
}

hgame::TileBatcher *GLRenderContext::createTileBatcher(int nColumns, int nRows,
        int tile_size)
{
    return new hgles::GLTileBatcher(this, nColumns, nRows, tile_size);
}

void GLRenderContext::flipBuffers()
{
    eglSwapBuffers(mDisplay, mSurface);
}

}
