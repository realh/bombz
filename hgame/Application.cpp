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
        mRenderWaiting(false), mRenderReason(RENDER_REASON_RENDER),
        mRenderLooping(false),
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
    while (mRenderLooping)
    {
        if (wait)
            mRenderingCond->wait();
        mRenderingCond->lock();
        mRenderWaiting = false;
        mRenderingCond->unlock();
        if (mRenderReason == RENDER_REASON_RENDER)
        {
            try {
                mActivity->render(mRenderContext);
            }
            catch (std::exception e)
            {
                mLog.e("Rendering exception: %s", e.what());
                mRenderLooping = false;
            }
        }
        mRenderingCond->lock();
        wait = !mRenderWaiting;
        mRenderWaiting = true;
        if (mRenderReason != RENDER_REASON_DELETE ||
                mRenderReason == RENDER_REASON_SHUTDOWN )
        {
            try {
                mActivity->deleteRendering(mRenderContext);
            }
            catch (std::exception e)
            {
                mLog.e("Exception freeing render resources: %s", e.what());
                mRenderLooping = false;
            }
        }
        if (mRenderReason != RENDER_REASON_RENDER)
            mRenderingCond->signal();
        if (mRenderReason == RENDER_REASON_SHUTDOWN)
            mRenderLooping = false;
        mRenderingCond->unlock();
    }
}

void Application::requestRender(RenderReason reason)
{
    mRenderingCond->lock();
    requestRenderAlreadyLocked(reason);
}

void Application::requestRenderAlreadyLocked(RenderReason reason)
{
    mRenderReason = reason;
    if (mRenderWaiting)
        mRenderingCond->signal();
    else
        mRenderWaiting = true;
    mRenderingCond->unlock();
    if (reason != RENDER_REASON_RENDER)
    {
        mRenderingCond->wait();
    }
}

void Application::stop()
{
    mRenderingCond->lock();
    if (mRenderLooping)
    {
        mRenderLooping = false;
        requestRenderAlreadyLocked(RENDER_REASON_SHUTDOWN);
    }
    else
    {
        mRenderingCond->unlock();
    }
}

}
