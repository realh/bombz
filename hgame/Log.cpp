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

#include "hgame/cstdext.h"

namespace hgame {

void Log::logWrite(Level level, const char *message)
{
    std::fprintf(cstderr, "%-7s '%s': %s\n",
            getLevelName(level), tag, message);
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
    this->tag = cstrdup(tag);
    this->priority = priority;
}

Log::~Log()
{
    // Not delete because we used strdup()
    std::free(tag);
}
    
void Log::log(Level level, const char *format, ...)
{
    if (level <= priority)
    {
        std::va_list ap;
        va_start(ap, format);
        log(level, format, ap);
        va_end(ap);
    }
}
    
void Log::log(Level level, const char *format, std::va_list ap)
{
    if (level <= priority)
    {
        char *s;
        cvasprintf(&s, format, ap);
        logWrite(level, s);
        std::free(s);
    }
}

#define IMPLEMENT_LOG_LEVEL(l) \
    std::va_list ap; \
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
        
Throwable::Throwable(const char *file, int line, const char *func,
            const char *desc, ...)
{
    va_list ap;
    va_start(ap, desc);
    Throwable(file, line, func, desc, ap);
    va_end(ap);
}

Throwable::Throwable(const char *file, int line, const char *func,
            int errno_code, const char *desc, ...)
{
    va_list ap;
    va_start(ap, desc);
    Throwable(file, line, func, errno_code, desc, ap);
    va_end(ap);
}

Throwable::Throwable(const char *file, int line, const char *func,
            const char *desc, std::va_list ap)
{
    char *tmp;
    cvasprintf(&tmp, desc, ap);
    casprintf(&repr, "Exception '%s' in %s at %s/%d: %s",
            getClassName(), func, file, line, tmp);
    std::free(tmp);
}

Throwable::Throwable(const char *file, int line, const char *func,
            int errno_code, const char *desc, std::va_list ap)
{
    char *tmp;
    cvasprintf(&tmp, desc, ap);
    casprintf(&repr, "Errno exception '%s' in %s at %s/%d: %s - %s",
            getClassName(), func, file, line, tmp, std::strerror(errno_code));
    std::free(tmp);
}

Throwable::~Throwable()
{
    std::free(repr);
}

const char *Throwable::getClassName() const
{
    return "Throwable";
}

}
