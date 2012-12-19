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

#include "handroid/Thread.h"

#include <errno.h>
#include <time.h>

#include "hgame/Types.h"

namespace handroid {

Mutex::Mutex()
{
	pthread_mutex_init(&mMutex, 0);
}

Mutex::~Mutex()
{
    pthread_mutex_destroy(&mMutex);
}

void Mutex::lock()
{
	int r = pthread_mutex_lock(&mMutex);
    if (r)
    {
        THROW(hgame::ErrnoException, r, "Unable to lock mutex");
    }
}

void Mutex::release()
{
	int r = pthread_mutex_unlock(&mMutex);
    if (r)
    {
        THROW(hgame::ErrnoException, r, "Unable to unlock mutex");
    }
}

Cond::Cond(Mutex *mutex)
{
    mMutex = mutex ? mutex : new Mutex();
    mOwnMutex = mutex == 0;
    pthread_cond_init(&mCond, 0);
}

Cond::~Cond()
{
	pthread_cond_destroy(&mCond);
}

void Cond::wait()
{
    if (pthread_cond_wait(&mCond, mMutex->getPthreadMutex()))
    {
        THROW(hgame::ErrnoException, "Error waiting for cond");
    }
}

bool Cond::waitTimeout(unsigned int ms)
{
	struct timeval now;
	struct timespec timeout;
	gettimeofday(&now, 0);
	ms += now.tv_sec;
	timeout.tv_sec = now.tv_sec + ms / 1000;
	timeout.tv_nsec = (ms % 1000) * 1000000;
	int r = pthread_cond_timedwait(&mCond,
			mMutex->getPthreadMutex(), &timeout);
    switch (r)
    {
        case 0:
            return true;
        case ETIMEDOUT:
            return false;
        default:
            THROW(hgame::ErrnoException, r, "Error waiting for cond");
    }
    return false;
}

void Cond::signal()
{
	pthread_cond_signal(&mCond);
}

void Cond::broadcast()
{
	pthread_cond_broadcast(&mCond);
}

hgame::Mutex *Cond::getMutex()
{
    return (hgame::Mutex *) mMutex;
}


pthread_key_t Thread::smCurrentThreadKey;
bool Thread::smKeyIsSet = false;
pthread_mutex_t Thread::smKeyMutex = PTHREAD_MUTEX_INITIALIZER;

Thread::Thread(JavaVM *jvm, hgame::Runnable *r, const char *name) :
        hgame::Thread(r, name), mThread(0), mRunning(false), mJVM(jvm)
{
	pthread_mutex_lock(&smKeyMutex);
	if (!smKeyIsSet)
	{
		pthread_key_create(&smCurrentThreadKey, 0);
		smKeyIsSet = true;
	}
	pthread_mutex_unlock(&smKeyMutex);
}

Thread::~Thread()
{
    const char *name = 0;
    if (mRunning)
    {
        // This will leak, but getting here means we're already bugged
        name = strdup(getName());
        pthread_kill(mThread, SIGKILL);
    }
    if (mRunning)
    {
        THROW(hgame::Throwable,
        		"Thread '%s' destroyed without waiting", name);
    }
}

void Thread::start()
{
    if (mThread)
    {
        THROW(hgame::Throwable, "Thread '%s' was already started", getName());
    }
    pthread_create(&mThread, 0, launch, reinterpret_cast<void *>(this));
}

int Thread::wait()
{
    void *result;
    if (!mThread)
    {
        THROW(hgame::Throwable,
        		"Waiting for unstarted thread '%s'", getName());
    }
    pthread_join(mThread, &result);
    return int(result);
}

void *Thread::launch(void *handle)
{
	pthread_setspecific(smCurrentThreadKey, handle);
    Thread *thread = reinterpret_cast<Thread *>(handle);
    thread->mRunning = true;
    void *result = reinterpret_cast<void *>(thread->mRunnable->run());
    thread->mRunning = false;
    thread->mJVM->DetachCurrentThread();
    return result;
}

Thread *Thread::getCurrentThread()
{
	void *result = 0;
	pthread_mutex_lock(&smKeyMutex);
	if (smKeyIsSet)
		result = pthread_getspecific(smCurrentThreadKey);
	else
		result = 0;
	pthread_mutex_unlock(&smKeyMutex);
	return reinterpret_cast<Thread *>(result);
}


hgame::Mutex *ThreadFactory::createMutex()
{
    return new Mutex();
}

hgame::Cond *ThreadFactory::createCond(hgame::Mutex *mutex)
{
    return new Cond(dynamic_cast<Mutex *>(mutex));
}

hgame::Thread *ThreadFactory::createThread(hgame::Runnable *r,
		const char *name)
{
    return new Thread(mJVM, r, name);
}

}
