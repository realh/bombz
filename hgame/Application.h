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

// Application.h: Abstract base class for gluing everything else together

#ifndef HGAME_APPLICATION_H
#define HGAME_APPLICATION_H

#include "config.h"

#include "hgame/Event.h"
#include "hgame/Log.h"
#include "hgame/Platform.h"
#include "hgame/RenderContext.h"
#include "hgame/Thread.h"
#include "hgame/Types.h"

namespace hgame {

extern const EventQuark EVENT_TICK;
extern const EventQuark EVENT_RENDER_CONTEXT_CREATED;
extern const EventQuark EVENT_RENDER_CONTEXT_DESTROYED;
extern const EventQuark EVENT_PAUSE;
extern const EventQuark EVENT_RESUME;

class TickEvent : public Event {
public:
    TickEvent() : Event(EVENT_TICK) {}
};

class Activity;

typedef enum {
    RENDER_REASON_RENDER,   // Render current frame
    RENDER_REASON_SUSPEND,  // Do nothing
    RENDER_REASON_DELETE,   // Free resources associated with rc,
                            // it's going away
    RENDER_REASON_SHUTDOWN  // As above, app is shutting down
} RenderReason;

class Application {
protected:
    Log &mLog;
    Platform *mPlatform;
    RenderContext *mRenderContext;
    ThreadFactory *mThreadFactory;
    Activity *mActivity;
    hgame::Cond *mRenderingCond;
    volatile bool mRenderWaiting;
    volatile bool mRenderReason;
    volatile bool mRenderLooping;
    EventQueue mEvQueue;
public:
    inline RenderContext *getRenderContext() { return mRenderContext; }

    inline Platform *getPlatform() { return mPlatform; }

    // Most of these args should be passed in by subclasses.
    // Note mRenderContext is not set here, some platforms can't create
    // one until later
    Application(Log *log, Platform *platform,
            Activity *activity, ThreadFactory *thread_fact);

    virtual ~Application();

    Mutex *createMutex()
    {
        return mThreadFactory->createMutex();
    }

    Cond *createCond(Mutex *mutex = 0)
    {
        return mThreadFactory->createCond(mutex);
    }

    Thread *createThread(Runnable *r, const char *name = 0)
    {
        return mThreadFactory->createThread(r, name);
    }

    // Wake up rendering thread. If reason is DELETE or SHUTDOWN, activity's
    // deleteRendering() will be called. If reason != RENDER this will block
    // until request has been serviced
    virtual void requestRender(RenderReason reason = RENDER_REASON_RENDER);

    inline Activity *getActivity()
    {
        return mActivity;
    }

    // Call to get everything going after setActivity()
    virtual void start() = 0;

    // Call *from Activity* to shut everything down when finished; default
    // implementation is equivalent to requestRender(RENDER_REASON_SHUTDOWN)
    // if mRenderLooping is true
    virtual void stop();

    inline void pushEvent(Event *ev)
    {
        mEvQueue.pushEvent(ev);
    }

    inline Event *getNextEvent()
    {
        return mEvQueue.getNextEvent();
    }

    // Makes sure there's a TICK event at intervals specified
    virtual Event *getNextEvent(int tick_period_ms) = 0;

protected:
    // Allows stop() to do the same as requestRender() without an unsafe
    // extra unlock/lock. Mutex is unlocked on exit from this method.
    virtual void requestRenderAlreadyLocked(RenderReason reason =
            RENDER_REASON_RENDER);

    virtual void renderLoop();

    // To be called at start of rendering thread
    virtual void createRenderContext() = 0;
};

}

#endif // HGAME_APPLICATION_H
