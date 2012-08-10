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

#include "sdl/Thread.h"

#include "sdl/Exception.h"

namespace sdl {

Mutex::Mutex()
{
    mMutex = SDL_CreateMutex();
}

Mutex::~Mutex()
{
    SDL_DestroyMutex(mMutex);
}

void Mutex::lock()
{
    if (SDL_mutexP(mMutex))
    {
        THROW(Exception, "Unable to lock mutex");
    }
}

void Mutex::release()
{
    if (SDL_mutexV(mMutex))
    {
        THROW(Exception, "Unable to release mutex");
    }
}

Cond::Cond(Mutex *mutex)
{
    mMutex = mutex ? mutex : new Mutex();
    mOwnMutex = mutex == 0;
    mCond = SDL_CreateCond();
}

Cond::~Cond()
{
    SDL_DestroyCond(mCond);
    if (mOwnMutex)
        delete mMutex;
}

void Cond::wait()
{
    if (SDL_CondWait(mCond, mMutex->getSDLMutex()))
    {
        THROW(Exception, "Error waiting for cond");
    }
}

bool Cond::waitTimeout(unsigned int ms)
{
    switch (SDL_CondWaitTimeout(mCond, mMutex->getSDLMutex(), ms))
    {
        case 0:
            return true;
        case SDL_MUTEX_TIMEDOUT:
            return false;
        default:
            THROW(Exception, "Error waiting for cond");
    }
    return false;
}

void Cond::signal()
{
    if (SDL_CondSignal(mCond))
    {
        THROW(Exception, "Error signalling cond");
    }
}

void Cond::broadcast()
{
    if (SDL_CondSignal(mCond))
    {
        THROW(Exception, "Error broadcasting on cond");
    }
}

hgame::Mutex *Cond::getMutex()
{
    return (hgame::Mutex *) mMutex;
}

Thread::Thread(hgame::Runnable *r, const char *name) :
        hgame::Thread(r, name), mThread(0), mRunning(false)
{
    mNameMutex = new Mutex();
}

Thread::~Thread()
{
    const char *name = 0;
    if (mRunning)
    {
        name = getName();
        // This will leak, but getting here means we're already bugged
        if (name)
            name = strdup(name);
    }
    SDL_KillThread(mThread);
    mThread = NULL;
    delete mNameMutex;
    if (mRunning)
    {
        THROW(Exception, "Thread '%s' destroyed without waiting", name);
    }
}

void Thread::start()
{
    if (mThread)
    {
        THROW(Exception, "Thread '%s' was already started", getName());
    }
    mThread = SDL_CreateThread(launch, this);
}

int Thread::wait()
{
    int result;
    if (!mThread)
    {
        THROW(Exception, "Waiting for unstarted thread '%s'", getName());
    }
    SDL_WaitThread(mThread, &result);
    return result;
}

const char *Thread::getImplementationName()
{
    mNameMutex->lock();
    if (!mName)
    {
        if (mThread)
            asprintf(&mName, "SDL thread %u", SDL_GetThreadID(mThread));
        else
            mName = strdup("Anonymous thread");
    }
    mNameMutex->release();
    return mName;
}
    
int Thread::launch(void *handle)
{
    Thread *thread = (Thread *) handle;
    thread->mRunning = true;
    int result = thread->mRunnable->run();
    thread->mRunning = false;
    return result;
}

hgame::Mutex *ThreadFactory::createMutex()
{
    return new Mutex();
}

hgame::Cond *ThreadFactory::createCond(hgame::Mutex *mutex)
{
    return new Cond((Mutex *) mutex);
}

hgame::Thread *ThreadFactory::createThread(hgame::Runnable *r, const char *name)
{
    return new Thread(r, name);
}

}
