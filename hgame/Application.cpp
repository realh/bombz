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

#include "hgame/Application.h"
#include "hgame/Activity.h"

namespace hgame {

const EventQuark EVENT_TICK("TICK");
const EventQuark EVENT_RENDER_CONTEXT_CREATED("RCUP");
const EventQuark EVENT_RENDER_CONTEXT_DESTROYED("RCDN");
const EventQuark EVENT_PAUSE("PAUS");
const EventQuark EVENT_RESUME("RESM");

Application::Application(Log *log, Platform *platform,
        Activity *activity, ThreadFactory *thread_fact) :
        mLog(*log), mPlatform(platform), mRenderContext(0),
        mThreadFactory(thread_fact), mActivity(activity),
        mRenderBlocking(false), mRenderLooping(false), mRenderWaiting(false),
        mEvQueue(thread_fact)
{
    mRenderingCond = createCond();
    // activity won't be null in a normal app, but can be in some basic tests
    if (activity)
        activity->setApplication(this);
}

Application::~Application()
{
    delete mRenderingCond;
    delete mRenderContext;
    delete mPlatform;
    delete &mLog;
}

void Application::renderLoop()
{
    mRenderingCond->lock();
    bool wait = !(mRenderWaiting = true);
    mRenderLooping = true;
    mRenderingCond->unlock();
    // Have to do an extra loop if wait is false even if mRenderLooping is false
    // because it means stop() was called during last loop
    while (mRenderLooping || wait)
    {
        if (wait)
            mRenderingCond->wait();
        mRenderingCond->lock();
        mRenderWaiting = false;
        mRenderingCond->unlock();
        try {
            mActivity->serviceRenderRequest(mRenderContext);
        }
        catch (std::exception e)
        {
            mLog.e("Rendering exception: %s", e.what());
            mRenderLooping = false;
        }
        mRenderingCond->lock();
        wait = !mRenderWaiting;
        mRenderWaiting = true;
        if (mRenderBlocking)
        {
            // Use same cond "in reverse"
            mRenderingCond->signal();
            mRenderBlocking = false;
        }
        mRenderingCond->unlock();
    }
}

void Application::requestRender()
{
    mRenderingCond->lock();
    requestRenderWhileLocked();
    mRenderingCond->unlock();
}

void Application::requestRenderWhileLocked(bool block)
{
    mRenderReason = reason;
    if (block)
        mRenderBlocking = true;
    if (mRenderWaiting)
        mRenderingCond->signal();
    else
        mRenderWaiting = true;
    if (block)
        mRenderingCond->wait();
}

void Application::changeActivity(Activity *new_act, bool del)
{
    mRenderingCond->lock();
    mActivity->requestRenderState(Activity::RENDER_STATE_FREE);
    requestRenderWhileLocked(true);
    if (del)
        delete mActivity;
    mActivity = new_act;
    mRenderingCond->unlock();
}

void Application::stop()
{
    mRenderingCond->lock();
    if (mRenderLooping)
    {
        mRenderLooping = false;
        mActivity->requestRenderState(Activity::RENDER_STATE_UNINITIALISED);
        requestRenderWhileLocked(true);
    }
    mRenderingCond->unlock();
}

}
