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

// Application.h: Core SDL Application

#ifndef HSDL_APPLICATION_H
#define HSDL_APPLICATION_H

#include "config.h"

#include "hgame/Application.h"

#include "hsdl/Types.h"

namespace hsdl {

class Application;

class SafeRunnable : public hgame::Runnable {
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

class EventRunnable : public SafeRunnable {
public:
    EventRunnable(Application *app) :
            SafeRunnable(app, "EventRunnable")
    {}
    int runSafely();
    void stop();
};

class Application : public hgame::Application {
private:
    HUInt32 mLastTick;
    hgame::Event *mSavedEvent;
    ScreenRunnable mScreenRunnable;
    hgame::Thread *mScreenThread;
    EventRunnable mEventRunnable;
    hgame::Thread *mEventThread;
public:
    Application(int argc, char **argv);
    ~Application();
    void start();
    void stop();
    void createRenderContext();
    hgame::Event *getNextEvent(int tick_period_ms);
};

}

#endif // HSDL_APPLICATION_H
