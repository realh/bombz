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

#include <cassert>
#include <cstdlib>

#include "SDL.h"

#include "hgame/Activity.h"
#include "hgame/TapEvent.h"

#include "hsdl/Application.h"
#include "hsdl/Exception.h"
#include "hsdl/Log.h"
#include "hsdl/Platform.h"
#include "hsdl/Thread.h"
#if ENABLE_OPENGL == 1
#include "hsdl/GLRenderContext.h"
#else
#error "OpenGL is currently compulsory"
#endif

namespace hsdl {

SafeRunnable::SafeRunnable(Application *app, const char *name) :
        mStopped(false), mApplication(app), mName(name),
        mLog(*(app->createLog(name)))
{
}

int SafeRunnable::run()
{
    int result = 1;
    try {
        result = runSafely();
    }
    catch (std::exception e)
    {
        mLog.e("Exception: %s", e.what());
        result = 1;
    }
    if (result)
    {
        mStopped = true;
        mApplication->stop();
    }
    return result;
}

void SafeRunnable::stop()
{
    mStopped = true;
}

int ActivityRunnable::runSafely()
{
    int result = 1;
    hgame::Activity *act;
    while (!mStopped && (act = mApplication->getActivity()) != 0)
    {
        result = act->run();
    }
    return result;
}

int EventRunnable::runSafely()
{
    while (!mStopped)
    {
        SDL_Event sdl_ev;
        if (SDL_WaitEvent(&sdl_ev))
        {
            switch (sdl_ev.type)
            {
                case SDL_QUIT:
                    mStopped = true;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (mApplication->getTapEventsEnabled() &&
                            sdl_ev.button.button == SDL_BUTTON_LEFT)
                    {
                        mApplication->pushEvent(
                                new hgame::TapEvent(sdl_ev.button.x,
                                        sdl_ev.button.y));
                    }
                    break;
            }
        }
        else
        {
            THROW(Exception, "SDL_WaitEvent error");
        }
    }
    return 0;
}

void EventRunnable::stop()
{
    mStopped = true;
    SDL_Event ev;
    ev.type = SDL_QUIT;
    if (SDL_PushEvent(&ev))
    {
        mLog.e("Unable to push SDL_QuitEvent");
        std::exit(1);
    }
}

Application::Application(int argc, char **argv) :
        hgame::Application(new Platform(argc, argv), "SDLApp",
                new ThreadFactory()),
        mLastTick(SDL_GetTicks()),
        mSavedEvent(0),
        mActivityRunnable(this),
        mActivityThread(0),
        mEventRunnable(this),
        mEventThread(0)
{
    hgame::Event::setPool(new hgame::EventPool(mThreadFactory));
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        THROW(Exception, "Init failed");
    mActivityThread = mThreadFactory->createThread(&mActivityRunnable,
            "Activity thread");
    mEventThread = mThreadFactory->createThread(&mEventRunnable,
            "Event thread");
}

Application::~Application()
{
    SDL_Quit();
    delete mActivityThread;
    delete mEventThread;
    mSavedEvent->dispose();
}

void Application::start()
{
    assert(mActivity != 0);
    try {
        createRenderContext();
        mActivity->requestRenderState(hgame::Activity::RENDER_STATE_RENDERING);
    }
    catch (std::exception e)
    {
        mLog.f("Exception initialising render context: %s", e.what());
        std::exit(1);
    }
    mEventThread->start();
    mActivityThread->start();
    renderLoop();
    delete mRenderContext;
    mActivityThread->wait();
    mEventThread->wait();
}

void Application::createRenderContext()
{
    mRenderContext = new GLRenderContext(mActivity->getBestModes());
}

hgame::Event *Application::getNextEvent(int tick_period_ms)
{
    hgame::Event *result;
    if (mSavedEvent)
    {
        result = mSavedEvent;
        mSavedEvent = 0;
        return result;
    }
    int timeout = tick_period_ms - (SDL_GetTicks() - mLastTick);
    if (timeout < 0)
        timeout = 0;
    result = mEvQueue.getNextEvent(timeout);
    Uint32 now = SDL_GetTicks();
    if (now - mLastTick >= (Uint32) tick_period_ms)
    {
        if (!result || !result->getPriority())
        {
            mLastTick = now;
            mSavedEvent = result;
            return new hgame::TickEvent();
        }
    }
    return result;
}

void Application::stop()
{
    pushEvent(new hgame::Event(hgame::EVENT_STOP, true));
    mActivityRunnable.stop();
    mEventRunnable.stop();
    hgame::Application::stop();
}

}
