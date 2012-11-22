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

// RendererWithState.h: Renderer with state management

#ifndef HGAME_RENDERER_WITH_STATE_H
#define HGAME_RENDERER_WITH_STATE_H

#include "config.h"

#include "hgame/Renderer.h"
#include "hgame/Thread.h"

namespace hgame {

class RendererWithState: public Renderer {
public:
    enum RenderState {
        RENDER_STATE_UNINITIALISED,
        RENDER_STATE_INITIALISED,
        RENDER_STATE_RENDERING,
        RENDER_STATE_REPLACE_SCREEN
        // REPLACE_SCREEN is eg for switching between screens, some resources
        // need to be created, some destroyed. Should be called while current
        // state is RENDERING. A replaceRenderingScreen() method may then set
        // mRequestedRenderState to RENDERING to force the new screen to be
        // rendered - or it can just call render() itself.
    };
protected:
    volatile RenderState mCurrentRenderState;
    volatile RenderState mRequestedRenderState;
    Mutex *mRenderStateMutex;
public:
    RendererWithState(ThreadFactory *tf);

    virtual ~RendererWithState();

    inline RenderState getRenderState() const
    {
        return mCurrentRenderState;
    }

    virtual void requestRenderState(RenderState new_state);

    // Called in rendering thread to call one of Renderer's pure virtual
    // functions or replaceRenderingScreen() depending on mRequestedRenderState.
    // Automatically calls rc->flipBuffers() after render().
    virtual void serviceRenderRequest(RenderContext *rc);

    // For RENDER_STATE_REPLACE_SCREEN
    virtual void replaceRenderingScreen(RenderContext *rc) = 0;
};

}

#endif // HGAME_RENDERER_WITH_STATE_H
