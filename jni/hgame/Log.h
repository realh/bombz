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

// Log.h: Interface for logging loosely based on Android's API

#ifndef HGAME_LOG_H
#define HGAME_LOG_H

#include "config.h"

#include <cstdarg>

#include "hgame/Thread.h"
#include "hgame/Types.h"

namespace hgame {

class Log
{
public:
    enum Level {
        FATAL,      // Causes abort
        ERROR,
        WARNING,
        INFO,
        DEBUG,
        VERBOSE
    };

    static const char *getLevelName(Level level);

    // Only messages with given priority or higher will be logged
    Log(const char *tag, Level priority = VERBOSE);

    virtual ~Log();

    virtual void logWrite(Level level, const char *message) = 0;

    // Formats a message before passing it on to logWrite while mutex is locked.
    virtual void log(Level level, const char *format, ...)
            H_GNUC_PRINTF(3, 4);
    virtual void log(Level level, const char *format, std::va_list ap);

    void f(const char *format, ...) H_GNUC_PRINTF(2, 3);
    void e(const char *format, ...) H_GNUC_PRINTF(2, 3);
    void w(const char *format, ...) H_GNUC_PRINTF(2, 3);
    void i(const char *format, ...) H_GNUC_PRINTF(2, 3);
#ifdef NDEBUG
    inline void d(const char *format, ...) const H_GNUC_PRINTF(2, 3)
    {
        (void) format;
    }

    void v(const char *format, ...) const H_GNUC_PRINTF(2, 3)
    {
        (void) format;
    }
#else
    void d(const char *format, ...) H_GNUC_PRINTF(2, 3);
    void v(const char *format, ...) H_GNUC_PRINTF(2, 3);
#endif

protected:
    // Derived constructors must initialise mutex lazily
    static Mutex *smMutex;
    char *mTag;
private:
    Mutex *mMutex;
    Level mPriority;
};

}

#endif // HGAME_LOG_H
