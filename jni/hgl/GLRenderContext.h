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

// GLRenderContext.h: Base class for texture-based rendering with OpenGL

#ifndef HGL_GL_RENDER_CONTEXT_H
#define HGL_GL_RENDER_CONTEXT_H

#include "config.h"

#include "hgame/RenderContext.h"

namespace hgl {

class GLRenderContext : public hgame::RenderContext {
public:
    GLRenderContext(hgame::Log *log) :
            hgame::RenderContext(log)
    {}

    ~GLRenderContext();

    void setViewport2D(int left, int top, int width, int height);
protected:
    void doBindTexture(const hgame::TextureAtlas *tex);

    // Sets up GL for texture mapping etc with glEnable[ClientState].
    // width and height are screen size, vp_w and vp_h are size of viewport
    // within it, chosen based on "best modes".
    virtual void initGL(int width, int height, int vp_w, int vp_h);

    virtual void enableBlend(bool enable = true);
private:
    bool mNeedScaling;
};

}

#endif // HGL_GL_RENDER_CONTEXT_H
