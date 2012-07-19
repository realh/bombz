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

#include "Log.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace hgame {

// For some reason these aren't in std::; use wrappers in case they are one day
static std::FILE *cstderr = stderr;

inline static char *cstrdup(const char *s) { return strdup(s); }

inline static int cvasprintf(char **sout, const char *fmt, std::va_list ap)
{
    return vasprintf(sout, fmt, ap);
}

static int casprintf(char **sout, const char *fmt, ...)
{
    std::va_list ap;
    va_start(ap, fmt);
    int result = vasprintf(sout, fmt, ap);
    va_end(ap);
    return result;
}

 
void Log::logWrite(Level level, const Location *loc, const char *message)
{
    std::fprintf(cstderr, "%-7s '%s' at %s: %s\n",
            getLevelName(level), tag, loc->getString(), message);
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
    
void Log::log(Level level, Location *loc, const char *format, ...)
{
    if (level <= priority)
    {
        std::va_list ap;
        va_start(ap, format);
        log(level, loc, format, ap);
        va_end(ap);
    }
    delete loc;
}
    
void Log::log(Level level, Location *loc, const char *format, std::va_list ap)
{
    if (level <= priority)
    {
        char *s;
        cvasprintf(&s, format, ap);
        logWrite(level, loc, s);
        std::free(s);
    }
    else
    {
        delete loc;
    }
}

#define IMPLEMENT_LOG_LEVEL(l) \
    std::va_list ap; \
    va_start(ap, format); \
    log(l, loc, format, ap); \
    va_end(ap);

void Log::f(Location *loc, const char * format, ...)
{
    IMPLEMENT_LOG_LEVEL(FATAL);
}

void Log::e(Location *loc, const char * format, ...)
{
    IMPLEMENT_LOG_LEVEL(ERROR);
}

void Log::w(Location *loc, const char * format, ...)
{
    IMPLEMENT_LOG_LEVEL(WARNING);
}

void Log::i(Location *loc, const char * format, ...)
{
    IMPLEMENT_LOG_LEVEL(INFO);
}

void Log::d(Location *loc, const char * format, ...)
{
    IMPLEMENT_LOG_LEVEL(DEBUG);
}

void Log::v(Location *loc, const char * format, ...)
{
    IMPLEMENT_LOG_LEVEL(VERBOSE);
}
        
Log::Location::~Location()
{
}
        
Log::LocationStr::LocationStr(const char *s)
{
    s = cstrdup(s);
}

Log::LocationStr::LocationStr(const char *srcfile, int line, const char *func)
{
    if (func)
        casprintf(&s, "line %d of file %s (function %s)", line, srcfile, func);
    else
        casprintf(&s, "line %d of file %s", line, srcfile);
}
        
Log::LocationStr::~LocationStr()
{
    std::free(s);
}
        
void Log::LocationStr::setString(const char *s)
{
    std::free(this->s);
    this->s = cstrdup(s);
}

const char *Log::LocationStr::getString() const
{
    return s;
}

}
