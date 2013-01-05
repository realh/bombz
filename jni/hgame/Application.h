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

#include "hgame/Controls.h"
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
extern const EventQuark EVENT_STOP;

class TickEvent : public Event {
public:
    TickEvent() : Event(EVENT_TICK) {}
};

class Application;
class Screen;

class SafeRunnable : public Runnable {
protected:
    bool mStopped;
    Application *mApplication;
    const char *mName;
    hgame::Log &mLog;
public:
    SafeRunnable(Application *app, const char *name);

    // Acts as wrapper to runSafely() with exception handling
    int run();

    virtual int runSafely() = 0;

    virtual void stop();
};

class ScreenRunnable : public SafeRunnable {
public:
    ScreenRunnable(Application *app) :
            SafeRunnable(app, "ScreenRunnable")
    {}
    int runSafely();
};


class Application {
protected:
    Log &mLog;
    Platform *mPlatform;
    RenderContext *mRenderContext;
    ThreadFactory *mThreadFactory;
    Screen *mScreen;
    hgame::Cond *mRenderingCond;
    volatile bool mRenderBlocking;
    volatile bool mRenderLooping;
    volatile bool mRenderWaiting;
    volatile bool mTapEventsEnabled;
    EventQueue mEvQueue;
    Controls *mControls;
    ScreenRunnable mScreenRunnable;
    Thread *mScreenThread;
    HUInt32 mLastTick;
    Event *mSavedEvent;
public:
    RenderContext *getRenderContext() { return mRenderContext; }

    Platform *getPlatform() { return mPlatform; }

    // Most of these args should be passed in by subclasses.
    // Note mRenderContext is not set here, some platforms can't create
    // one until later
    Application(Platform *platform, const char *log_name,
            ThreadFactory *thread_fact);

    virtual ~Application();

    ThreadFactory *getThreadFactory()
    {
        return mThreadFactory;
    }

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

    // Wake up rendering thread to service a render/init/shutdown request
    virtual void requestRender(bool block = false);

    Screen *getScreen()
    {
        return mScreen;
    }

    // replaceRenderingScreen() is called on old screen, if any.
    // New screen is synced to old one's state.
    void setScreen(Screen *new_act);

    // Call to get everything going after setScreen()
    virtual void start() = 0;

    // Call *from Screen* to shut everything down when finished; default
    // implementation is equivalent to shutting down screen's rendering
    // if mRenderLooping is true
    virtual void stop();

    void pushEvent(Event *ev)
    {
        mEvQueue.pushEvent(ev);
    }

    Event *getNextEvent()
    {
        return mEvQueue.getNextEvent();
    }

    // Makes sure there's a TICK event at intervals specified
    virtual Event *getNextEvent(int tick_period_ms);

    Log *createLog(const char *name, Log::Level priority = Log::VERBOSE)
    {
        return mPlatform->createLog(name, priority);
    }

    void setTapEventsEnabled(bool setting)
    {
        mTapEventsEnabled = setting;
    }

    bool getTapEventsEnabled()
    {
        return mTapEventsEnabled;
    }

    Controls *getControls()
    {
        return mControls;
    }

    Controls::Keys getControlsState()
    {
        return mControls->getControlsState();
    }
protected:
    // Allows stop() to do the same as requestRender() without an unsafe
    // extra unlock/lock. Mutex is still locked on exit from this method.
    // Can also block until request has been serviced by rendering thread,
    // this causes a temporary unlock.
    virtual void requestRenderWhileLocked(bool block = false);

    virtual void renderLoop();

    // To be called at start of rendering thread
    virtual void createRenderContext() = 0;
};

}

#endif // HGAME_APPLICATION_H
