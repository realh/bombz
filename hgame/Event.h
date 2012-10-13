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

// Event.h: Thread-safe event system

#ifndef HGAME_EVENT_H
#define HGAME_EVENT_H

#include "config.h"

#include <cstdlib>
#include <queue>

#include "hgame/Thread.h"
#include "hgame/Types.h"

namespace hgame {

// Uniquely identifies an event type, using 4-character strings
class EventQuark {
private:
    HUInt32 mValue;
public:
    inline EventQuark(const char *s)
    {
        mValue = (HUInt32) s[0] | (((HUInt32) s[1]) << 8) |
                (((HUInt32) s[2]) << 16) | (((HUInt32) s[3]) << 24);
    }

    inline EventQuark(const EventQuark &that)
    {
        mValue = that.mValue;
    }

    inline HUInt32 getValue() const
    {
        return mValue;
    }

    inline bool operator==(const EventQuark &that) const
    {
        return mValue == that.mValue;
    }
};

// Pool of events to reduce memory alloc activity
class EventPool {
public:
    static const int kPoolSize = 16;
private:
    void *mPool[kPoolSize];
    Mutex *mMutex;
public:
    EventPool(ThreadFactory *tf);

    ~EventPool();

    void *getEventMem();

    void returnEvent(class Event *ev);
};

class Event {
public:
    static const int kMaxSize = 60;
private:
    static EventPool *smPool;
    EventQuark mType;
    bool mPriority;
public:
    // Priority is whether event has higher priority than tick
    inline Event(const EventQuark &t, bool priority = false) :
            mType(t), mPriority(priority)
    {}

    // Returns to pool, subclasses may do other stuff too
    virtual void dispose();

    void *operator new(size_t sz);

    void operator delete(void *);

    inline static void setPool(EventPool *pool)
    {
        smPool = pool;
    }

    const EventQuark &getType() const
    {
        return mType;
    }

    bool getPriority() const
    {
        return mPriority;
    }
};

// Allows events to be posted and read
class EventQueue {
private:
    Cond *mCond;
    std::queue<Event *> mQueue;
    volatile int mWaiting;
public:
    EventQueue(ThreadFactory *tf);

    ~EventQueue();

    void pushEvent(Event *);

    // timeout of -1 means wait indefinitely, 0 means return immediately.
    // If no event on queue returns NULL.
    Event *getNextEvent(int timeout = -1);
};

}

#endif // HGAME_EVENT_H
