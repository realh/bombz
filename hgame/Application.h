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

#include "hgame/Log.h"
#include "hgame/Platform.h"
#include "hgame/RenderContext.h"
#include "hgame/Thread.h"

namespace hgame {

class Activity;

class Application {
protected:
    Log &mLog;
    Platform *mPlatform;
    RenderContext *mRenderContext;
    ThreadFactory *mThreadFactory;
    Activity *mActivity;
public:
    inline RenderContext *getRenderContext() { return mRenderContext; }
    inline Platform *getPlatform() { return mPlatform; }
    Application(Log *log, Platform *platform, RenderContext *rc,
            ThreadFactory *thread_fact);
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
    
    // Either starts a thread for rendering or activates rendering in
    // main thread depending on implementation. Either way, should be called
    // from a different thread.
    virtual void startRendering(Runnable *r) = 0;
    
    // Waits for rendering to stop, either by joining the thread, or waiting
    // on a cond depending on implementation. Either way, should be called
    // from a different thread.
    virtual int awaitRendering() = 0;
    
    void setActivity(Activity *activity)
    {
        mActivity = activity;
    }
    
    Activity *getActivity()
    {
        return mActivity;
    }
    
    virtual void start() = 0;
};

}

#endif // HGAME_APPLICATION_H
