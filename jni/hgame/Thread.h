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

// Thread.h: Threading interface

#ifndef HGAME_THREAD_H
#define HGAME_THREAD_H

#include "config.h"

#include <cstring>

namespace hgame {

class Mutex {
public:
    virtual void lock() = 0;
    virtual void release() = 0;
    void unlock()
    {
        release();
    }
    virtual ~Mutex()
    {}
};

class Cond {
public:
    virtual void wait() = 0;
    // Returns false on timeout
    virtual bool waitTimeout(unsigned int ms) = 0;
    virtual void signal() = 0;
    virtual void broadcast() = 0;
    virtual ~Cond()
    {}
    virtual Mutex *getMutex() = 0;
    void lock() { getMutex()->lock(); }
    void release() { getMutex()->release(); }
    void unlock() { getMutex()->release(); }
};

class Runnable {
public:
    virtual int run() = 0;
    virtual ~Runnable()
    {}
};

class Thread {
public:
    Thread(Runnable *r, const char *name = 0) :
            mName(strdup(name)), mRunnable(r)
    {}
    virtual void start() = 0;
    virtual int wait() = 0;
    virtual ~Thread();
    const char *getName()
    {
        return mName;
    }
protected:
    char *mName;
    Runnable *mRunnable;
};

class ThreadFactory {
public:
	virtual ~ThreadFactory()
    {}
    virtual Mutex *createMutex() = 0;
    virtual Cond *createCond(Mutex *mutex = 0) = 0;
    virtual Thread *createThread(Runnable *r, const char *name) = 0;
};

}

#endif // HGAME_THREAD_H
