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

// Thread.h: Threading with SDL

#ifndef HSDL_THREAD_H
#define HSDL_THREAD_H

#include "config.h"

#include "SDL_thread.h"

#include "hgame/Thread.h"

namespace hsdl {

class Mutex : public hgame::Mutex {
private:
    SDL_mutex *mMutex;
public:
    Mutex();
    ~Mutex();
    void lock();
    void release();
    SDL_mutex *getSDLMutex() { return mMutex; }
};

class Cond : public hgame::Cond {
private:
    SDL_cond *mCond;
    bool mOwnMutex;
    Mutex *mMutex;
public:
    // If mutex not given, one will be created
    Cond(Mutex *mutex = 0);
    ~Cond();
    void wait();
    // Returns false on timeout
    bool waitTimeout(unsigned int ms);
    void signal();
    void broadcast();
    hgame::Mutex *getMutex();
};

class Thread : public hgame::Thread {
private:
    SDL_Thread *mThread;
    bool mRunning;
public:
    Thread(hgame::Runnable *r, const char *name);
    ~Thread();
    void start();
    int wait();
private:
    static int launch(void *thread);
};

class ThreadFactory : public hgame::ThreadFactory {
public:
    hgame::Mutex *createMutex();
    // If mutex not given, one will be created
    hgame::Cond *createCond(hgame::Mutex *mutex = 0);
    hgame::Thread *createThread(hgame::Runnable *r, const char *name = 0);
};

}

#endif // HSDL_THREAD_H
