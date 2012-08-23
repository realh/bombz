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
    // a main loop which checks the mRunning flag and exits when it's false,
    // calling stopRendering before destroying any resources associated with
    // the RenderContext.
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
    
    // subclasses must initialise log
    ActivityBase(Application *app, Log *log) : mApplication(app), mLog(*log)
    {
    }
    
    virtual ~ActivityBase();
    
    // Called when Application has created a render context, starting the
    // activity. Subclasses should chain up
    virtual void start();
    
    // Called by Application when the activity should be shut down
    virtual void stop();
    
    // Called on the rendering thread
    virtual void render() = 0;
    
    // Wake up rendering thread
    virtual void requestRender() = 0;
    
    // Called to wait for the rendering thread to stop
    virtual void stopRendering() = 0;
};

}

#endif // HGAME_ACTIVITY_BASE_H
