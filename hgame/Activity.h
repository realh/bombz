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

// Activity.h: A game's activity

#ifndef HGAME_ACTIVITY_H
#define HGAME_ACTIVITY_H

#include "config.h"

#include "hgame/Application.h"
#include "hgame/Thread.h"

namespace hgame {

class Activity : public Runnable {
    // Note we implement Runnable. The run() function should consist mainly of
    // a main loop which sets the mRunning flag when starting and regularly
    // checks it, exiting the loop when it's false.
    // It will usually have its own thread, so it should
    // catch any unhandled exceptions at the top-level.
public:
    enum RenderState {
        RENDER_STATE_UNINITIALISED,
        RENDER_STATE_INITIALISED,
        RENDER_STATE_RENDERING,
        RENDER_STATE_FREE
    };
protected:
    Application *mApplication;
    Log &mLog;
    volatile bool mRunning;
    volatile RenderState mCurrentRenderState;
    volatile RenderState mRequestedRenderState;
    Mutex *mRenderStateMutex;
    char *mName;
public:
    Activity(Application *app, const char *name);

    virtual ~Activity();

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

    inline Application *getApplication()
    {
        return mApplication;

    }

    inline RenderContext *getRenderContext()
    {
        return mApplication->getRenderContext();

    }

    inline Platform *getPlatform()
    {
        return mApplication->getPlatform();
    }

    // Return a 0-terminated array of ints in pairs of x, y;
    // sorted with best mode first;
    // must be able to delete[] result
    virtual int *getBestModes() = 0;
};

}

#endif // HGAME_ACTIVITY_H
