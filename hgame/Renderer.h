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

// Renderer.h: Interface for objects which render something

#ifndef HGAME_RENDERER_H
#define HGAME_RENDERER_H

#include "config.h"

#include "hgame/RenderContext.h"
#include "hgame/Thread.h"

namespace hgame {

class Renderer {
public:
    enum RenderState {
        RENDER_STATE_UNINITIALISED,
        RENDER_STATE_INITIALISED,
        RENDER_STATE_RENDERING,
        RENDER_STATE_FREE
    };
protected:
    volatile RenderState mCurrentRenderState;
    volatile RenderState mRequestedRenderState;
    Mutex *mRenderStateMutex;
public:
    Renderer(ThreadFactory *tf);

    virtual ~Renderer();

    // The difference between FREE and UNINITIALISED is that FREE is for
    // freeing up resoureces only for current activity,
    virtual void requestRenderState(RenderState new_state);

    // Called in rendering thread to call one of the following pure virtual
    // functions depending on mRequestedRenderState
    virtual void serviceRenderRequest(RenderContext *rc);

    // For RENDER_STATE_RENDERING
    virtual void render(RenderContext *rc) = 0;

    // For RENDER_STATE_INITIALISED
    virtual void initRendering(RenderContext *rc) = 0;

    // For RENDER_STATE_FREE/UNINITILISED
    virtual void deleteRendering(RenderContext *rc) = 0;
};

}

#endif // HGAME_RENDERER_H