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

// GLTextureAtlas.h: Common OpenGL Texture Atlas

#ifndef HGL_TEXTURE_ATLAS_H
#define HGL_TEXTURE_ATLAS_H

#include "config.h"

#ifdef WITH_SDL
#include "SDL.h"
#include "SDL_opengl.h"
#endif

#include "hgl/GLRenderContext.h"

namespace hgl {

class GLTextureAtlas {
private:
    int mWidth, mHeight;
    GLuint mTextureId;
public:
    int getWidth() const;
    int getHeight() const;
    inline GLuint getTextureId() { return mTextureId; }
    
    GLTextureAtlas(GLRenderContext *rc, int width, int height);
    ~GLTextureAtlas();
    
    inline void bind(GLRenderContext *rc)
    {
        if (rc->getBoundTexture() != this)
        {
            rc->bindTexture(this);
        }
    }
    
    inline bool isBound(GLRenderContext *rc)
    {
        return rc->getBoundTexture() == this;
    }
};

}

#endif // HGL_TEXTURE_ATLAS_H
