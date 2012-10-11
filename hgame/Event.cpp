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

namespace hgame {

void Event::dispose() {}

EventQueue::EventQueue(ThreadFactory *tf) :
        mCond(tf->createCond()), mWaiting(0)
{
    memset(mPool, 0, kPoolSize * sizeof(void *));
}

EventQueue::~EventQueue()
{
    for (int n = 0; n < kPoolSize; ++n)
    {
        if (mPool[n])
            delete[] (char *) mPool[n];
    }
    delete mCond;
}

void *EventQueue::getEventMem()
{
    mCond->lock();
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
        result = new char[sizeof(Event)];
    mCond->release();
    return result;
}

void EventQueue::disposeOfEvent(Event *ev)
{
    ev->dispose();
    mCond->lock();
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
    mCond->release();
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
