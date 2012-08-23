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

#include "hsdl/Application.h"

#include "SDL.h"

#include "hgame/Activity.h"

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

Application::Application(int argc, char **argv) :
        hgame::Application(new Log("SDLApp"),
                new Platform(argc, argv),
                0, new ThreadFactory())
{
    mRenderingCond = createCond();
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        THROW(Exception, "Init failed");
}

Application::~Application()
{
    SDL_Quit();
}

void Application::start()
{
    mActivity->start();
    while (true) // FIXME: Need some sort of stop condition
    {
        mRenderingCond->wait();
        mRenderingCond->getMutex()->lock();
        mRenderer->run();
        mRenderingCond->getMutex()->unlock();
    }
}

void Application::startRendering(hgame::Runnable *r)
{
    mRenderingCond->getMutex()->lock();
    mRenderer = r;
    mRenderingCond->signal();
    mRenderingCond->getMutex()->unlock();
}

int Application::awaitRendering()
{
    // Just wait for cond to be unlocked after running mRenderer
    mRenderingCond->getMutex()->lock();
    mRenderingCond->getMutex()->unlock();
    return 0;
}

}
