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
#include <exception>

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
    
    const char *getLevelName(Level level) const;
    
    // Only messages with given priority or higher will be logged
    Log(const char *tag, Level priority = DEBUG);
    
    virtual ~Log();
    
    // Default implementation writes info to stderr.
    // Derived classes can re-implement this
    virtual void logWrite(Level level, const char *message);

public:
    // Formats a message before passing it on to logWrite.
    virtual void log(Level level, const char *format, ...);
    virtual void log(Level level, const char *format, std::va_list ap);
    
    void f(const char *format, ...);
    void e(const char *format, ...);
    void w(const char *format, ...);
    void i(const char *format, ...);
    void d(const char *format, ...);
    void v(const char *format, ...);
    
private:
    char *tag;
    Level priority;
};

// An exception class which includes info about where it was thrown
class Throwable : public std::exception {
protected:
    char *repr;
    virtual const char *getClassName() const throw();
    
    // Allows subclasses to construct more complex strings, must set repr
    // in constructor
    Throwable();
public:
    Throwable(const char *file, int line, const char *func,
            const char *desc, ...) throw();
    Throwable(const char *file, int line, const char *func,
            int errno_code, const char *desc, ...) throw();
    Throwable(const char *file, int line, const char *func,
            const char *desc, std::va_list ap) throw();
    Throwable(const char *file, int line, const char *func,
            int errno_code, const char *desc, std::va_list ap) throw();
    ~Throwable() throw();
    virtual const char *what() const throw();
};

}

#define THROW(typ, args...) \
        throw typ(__FILE__, __LINE__, __PRETTY_FUNCTION__, args)

#endif // HGAME_LOG_H
