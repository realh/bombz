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

#include "hgame/Activity.h"

namespace hgame {

Activity::Activity(Application *app, Log *log, const char *name) :
        ActivityBase(app, log), mRenderLoop(0), mSubActivity(0),
        mRenderNeeded(false)
{
    mThread = app->createThread(this, name);
    mRenderCond = app->createCond();
}

Activity::~Activity()
{
    delete mThread;
}
    
void Activity::start()
{
    ActivityBase::start();
    mThread->start();
}
    
void Activity::stop()
{
    ActivityBase::stop();
    stopRendering();
    if (mSubActivity)
    {
        mSubActivity->stop();
        mSubActivity->stopped();
    }
    mThread->wait();
}

int Activity::run()
{
    int result = -1;
    
    while (mSubActivity && mRunning)
    {
        result = mSubActivity->run();
    }
    return result;
}

void Activity::setSubActivity(SubActivity *subact)
{
    if (mSubActivity)
    {
        mSubActivity->stop();
    }
    mSubActivity = subact;
}

int Activity::runRenderLoop()
{
    while (mRunning)
    {
        mRenderCond->wait();
        mRenderCond->getMutex()->lock();
        mRenderCond->getMutex()->unlock();
        bool dirty = true;
        while (dirty)
        {
            render();
            mRenderCond->getMutex()->lock();
            dirty = mRenderNeeded;
            mRenderNeeded = false;
            mRenderCond->getMutex()->unlock();
        }
    }
    return 0;
}
    
void Activity::render()
{
    if (mSubActivity)
        mSubActivity->render();
}

void Activity::requestRender()
{
    mRenderCond->getMutex()->lock();
    if (!mRenderNeeded)
    {
        mRenderNeeded = true;
        mRenderCond->signal();
    }
    mRenderCond->getMutex()->unlock();
}

void Activity::stopRendering()
{
    requestRender();
    mApplication->awaitRendering();
}

int RenderLoop::run()
{
    return mActivity->run();
}

}
