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

#include "config.h"

#include <cstring>

#include "hgame/Event.h"
#include "hgame/Log.h"

namespace hgame {

EventPool::EventPool(ThreadFactory *tf) :
        mMutex(tf->createMutex())
{
    memset(mPool, 0, kPoolSize * sizeof(void *));
}

EventPool::~EventPool()
{
    for (int n = 0; n < kPoolSize; ++n)
    {
        if (mPool[n])
            delete[] (char *) mPool[n];
    }
    delete mMutex;
}

void *EventPool::getEventMem()
{
    mMutex->lock();
    void *result = 0;
    for (int n = 0; n < kPoolSize; ++n)
    {
        if (mPool[n])
        {
            result = mPool[n];
            mPool[n] = 0;
            break;
        }
    }
    if (!result)
        result = new char[Event::kMaxSize + sizeof(EventQuark)];
    mMutex->release();
    return result;
}

void EventPool::returnEvent(Event *ev)
{
    mMutex->lock();
    int n;
    for (n = 0; n < kPoolSize; ++n)
    {
        if (!mPool[n])
        {
            mPool[n] = ev;
            break;
        }
    }
    if (n == kPoolSize)
        delete[] (char *) ev;
    mMutex->release();
}

EventPool *Event::smPool = 0;

void Event::dispose()
{
    smPool->returnEvent(this);
}

void *Event::operator new(size_t sz)
{
    if (sz > Event::kMaxSize + sizeof(EventQuark))
    {
        THROW(Throwable, "Event is too big, increase Event::kMaxSize");
    }
    if (!smPool)
    {
        THROW(Throwable, "Event pool not initialised");
    }
    return smPool->getEventMem();
}

void Event::operator delete(void *addr)
{
    delete[] (char *) addr;
}

EventQueue::EventQueue(ThreadFactory *tf) :
        mCond(tf->createCond()), mWaiting(0)
{
}

EventQueue::~EventQueue()
{
    delete mCond;
}

void EventQueue::pushEvent(Event *ev)
{
    mCond->lock();
    mQueue.push(ev);
    if (mWaiting)
        mCond->broadcast();
    mCond->release();
}

Event *EventQueue::getNextEvent(int timeout)
{
    Event *result = 0;
    mCond->lock();
    if (mQueue.empty())
    {
        if (timeout != 0)
        {
            ++mWaiting;
            if (timeout == -1)
            {
                mCond->wait();
                result = mQueue.front();
                mQueue.pop();
            }
            else
            {
                if (mCond->waitTimeout(timeout))
                {
                    result = mQueue.front();
                    mQueue.pop();
                }
            }
            --mWaiting;
        }
    }
    else
    {
        result = mQueue.front();
        mQueue.pop();
    }
    mCond->release();
    return result;
}

}
