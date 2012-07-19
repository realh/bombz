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

#include <cstdarg>

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
    
    // Abstract class to encapsulate information about where an error occured.
    class Location
    {
    public:
        virtual ~Location();
        virtual const char *getString() const = 0;
        inline operator const char *() const
        {
            return getString();
        }
    };
    
    // Default implementation writes info to stderr.
    // Derived classes can re-implement this
    virtual void logWrite(Level level, const Location *loc,
            const char *message);

public:
    // Formats a message before passing it on to logWrite.
    // loc is not const because log() deletes it
    virtual void log(Level level, Location *loc, const char *format, ...);
    virtual void log(Level level, Location *loc,
            const char *format, std::va_list ap);
    
    void f(Location *loc, const char *format, ...);
    void e(Location *loc, const char *format, ...);
    void w(Location *loc, const char *format, ...);
    void i(Location *loc, const char *format, ...);
    void d(Location *loc, const char *format, ...);
    void v(Location *loc, const char *format, ...);
    
    // Simple Location where only info is a string
    class LocationStr : public Location
    {
    protected:
        char *s;
    public:
        LocationStr(const char *s);
        // Most likely called with args (__FILE__, __LINE__
        //                               [, __PRETTY_FUNCTION__])
        LocationStr(const char *srcfile, int line, const char *func = 0);
        ~LocationStr();
        void setString(const char *s);
        const char *getString() const;
    };

private:
    char *tag;
    Level priority;

};

}

// Convenience macros
#define HGAME_LOG(lgr, lvl, fmt...) (lgr)->log((lvl), \
        new hgame::Log::LocationStr(__FILE__, __LINE__, __PRETTY_FUNCTION__), \
        fmt)

#define HGAME_F(lgr, fmt...) HGAME_LOG((lgr), hgame::Log::FATAL, fmt)
#define HGAME_E(lgr, fmt...) HGAME_LOG((lgr), hgame::Log::ERROR, fmt)
#define HGAME_W(lgr, fmt...) HGAME_LOG((lgr), hgame::Log::WARNING, fmt)
#define HGAME_I(lgr, fmt...) HGAME_LOG((lgr), hgame::Log::INFO, fmt)
#define HGAME_D(lgr, fmt...) HGAME_LOG((lgr), hgame::Log::DEBUG, fmt)
#define HGAME_V(lgr, fmt...) HGAME_LOG((lgr), hgame::Log::VERBOSE, fmt)

#endif // HGAME_LOG_H
