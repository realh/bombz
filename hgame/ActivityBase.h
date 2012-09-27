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

// ActivityBase.h: Base class for Activity and SubActivity

#ifndef HGAME_ACTIVITY_BASE_H
#define HGAME_ACTIVITY_BASE_H

#include "config.h"

#include "hgame/Application.h"
#include "hgame/Thread.h"

namespace hgame {

class ActivityBase : public Runnable {
    // Note we implement Runnable. The run() function should consist mainly of
    // a main loop which sets the mRunning flag when starting and regularly
    // checks it, exiting the loop when it's false.
    // It will usually have its own thread, so it should
    // catch any unhandled exceptions at the top-level.
protected:
    Application *mApplication;
    Log &mLog;
    volatile bool mRunning;
public:
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

    ActivityBase(Log *log) : mApplication(0), mLog(*log)
    {
    }

    // Called from Application constructor in case of Activity subclass
    virtual void setApplication(Application *app);

    virtual ~ActivityBase();

    // Called by Application when the activity should be shut down
    virtual void stop();

    // Called on the rendering thread
    virtual void render() = 0;

    // Called from rendering thread when a context becomes available.
    // Load textures etc here. Activity subclasses should forward to
    // their SubActivities.
    virtual void initRendering(RenderContext *rc) = 0;

    // Called when a render context is being destroyed or when requested
    // by Application::requestRender(). Activity subclasses should forward to
    // their SubActivities.
    virtual void deleteRendering(RenderContext *rc) = 0;
};

}

#endif // HGAME_ACTIVITY_BASE_H
