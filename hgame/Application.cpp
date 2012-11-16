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
#include "hgame/Screen.h"

namespace hgame {

const EventQuark EVENT_TICK("TICK");
const EventQuark EVENT_RENDER_CONTEXT_CREATED("RCUP");
const EventQuark EVENT_RENDER_CONTEXT_DESTROYED("RCDN");
const EventQuark EVENT_PAUSE("PAUS");
const EventQuark EVENT_RESUME("RESM");
const EventQuark EVENT_STOP("STOP");

Application::Application(Platform *platform, const char *log_name,
        ThreadFactory *thread_fact) :
        mLog(*(platform->createLog(log_name))),
        mPlatform(platform), mRenderContext(0),
        mThreadFactory(thread_fact), mScreen(0),
        mRenderingCond(createCond()),
        mRenderBlocking(false), mRenderLooping(false), mRenderWaiting(false),
        mEvQueue(thread_fact)
{
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
    bool wait = !mRenderWaiting;
    mRenderLooping = true;
    // Have to do an extra loop if wait is false even if mRenderLooping is false
    // because it means stop() was called during last loop
    while (mRenderLooping || !wait)
    {
        if (wait)
        {
            mRenderWaiting = true;
            mRenderingCond->wait();
        }
        wait = !mRenderWaiting;
        mRenderWaiting = false;
        mRenderingCond->unlock();
        try {
            mScreen->serviceRenderRequest(mRenderContext);
        }
        catch (std::exception &e)
        {
            mLog.e("Rendering exception: %s", e.what());
            mRenderLooping = false;
        }
        mRenderingCond->lock();
        if (mRenderBlocking)
        {
            // Use same cond "in reverse"
            mRenderingCond->signal();
            mRenderBlocking = false;
        }
    }
    mRenderingCond->unlock();
}

void Application::requestRender(bool block)
{
    mRenderingCond->lock();
    requestRenderWhileLocked(block);
    mRenderingCond->unlock();
}

void Application::requestRenderWhileLocked(bool block)
{
    if (block)
        mRenderBlocking = true;
    if (mRenderWaiting)
    {
        mRenderingCond->signal();
    }
    else
    {
        mRenderWaiting = true;
    }
    if (block)
    {
        mRenderingCond->wait();
    }
}

void Application::setScreen(Screen *new_scrn)
{
    mRenderingCond->lock();
    bool old_scrn = mScreen != 0;
    Screen::RenderState rs = Screen::RENDER_STATE_UNINITIALISED;
    if (old_scrn)
    {
        rs = mScreen->getRenderState();
        mScreen->requestRenderState(Screen::RENDER_STATE_REPLACE_SCREEN);
        requestRenderWhileLocked(true);
    }
    mScreen = new_scrn;
    if (old_scrn)
    {
        mScreen->requestRenderState(rs);
        requestRenderWhileLocked(true);
    }
    mRenderingCond->unlock();
}

void Application::stop()
{
    mRenderingCond->lock();
    if (mRenderLooping)
    {
        mRenderLooping = false;
        if (mScreen)
            mScreen->requestRenderState(Screen::RENDER_STATE_UNINITIALISED);
        requestRenderWhileLocked(true);
    }
    mRenderingCond->unlock();
}

}
