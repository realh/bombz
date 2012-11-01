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

// Log.cpp: Simple logging implementation

#include "hgame/Log.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace hgame {

using namespace std;

void Log::logWrite(Level level, const char *message)
{
    fprintf(stderr, "%-7s '%s': %s\n", getLevelName(level), mTag, message);
    if (level == FATAL)
        abort();
}

const char *Log::getLevelName(Level level) const
{
    switch (level)
    {
        case FATAL:
            return "FATAL";
        case ERROR:
            return "ERROR";
        case WARNING:
            return "WARNING";
        case INFO:
            return "INFO";
        case DEBUG:
            return "DEBUG";
        case VERBOSE:
            return "VERBOSE";
    }
    return NULL;
}

Log::Log(const char *tag, Level priority)
{
    mTag = strdup(tag);
    mPriority = priority;
}

Log::~Log()
{
    // Not delete because we used strdup()
    free(mTag);
}

void Log::log(Level level, const char *format, ...)
{
    if (level <= mPriority)
    {
        va_list ap;
        va_start(ap, format);
        log(level, format, ap);
        va_end(ap);
    }
}

void Log::log(Level level, const char *format, va_list ap)
{
    if (level <= mPriority)
    {
        char *s;
        vasprintf(&s, format, ap);
        logWrite(level, s);
        free(s);
    }
}

#define IMPLEMENT_LOG_LEVEL(l) \
    va_list ap; \
    va_start(ap, format); \
    log(l, format, ap); \
    va_end(ap);

void Log::f(const char * format, ...)
{
    IMPLEMENT_LOG_LEVEL(FATAL);
}

void Log::e(const char * format, ...)
{
    IMPLEMENT_LOG_LEVEL(ERROR);
}

void Log::w(const char * format, ...)
{
    IMPLEMENT_LOG_LEVEL(WARNING);
}

void Log::i(const char * format, ...)
{
    IMPLEMENT_LOG_LEVEL(INFO);
}

void Log::d(const char * format, ...)
{
    IMPLEMENT_LOG_LEVEL(DEBUG);
}

void Log::v(const char * format, ...)
{
    IMPLEMENT_LOG_LEVEL(VERBOSE);
}

}
