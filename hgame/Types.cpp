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

// Types.cpp: Implementation of some types

#include "hgame/Types.h"

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace hgame {

using namespace std;

Throwable::Throwable() : mRepr(0) {}

Throwable::Throwable(const char *file, int line, const char *func,
        const char *desc, ...) throw()
{
    va_list ap;
    va_start(ap, desc);
    Throwable(file, line, func, desc, ap);
    va_end(ap);
}

Throwable::Throwable(const char *file, int line, const char *func,
        const char *desc, va_list ap) throw()
{
    char *tmp;
    vasprintf(&tmp, desc, ap);
    asprintf(&mRepr, "Exception '%s' in %s at %s/%d: %s",
            getClassName(), func, file, line, tmp);
    free(tmp);
}

Throwable::~Throwable() throw()
{
    free(mRepr);
}

const char *Throwable::getClassName() const throw()
{
    return "Throwable";
}

const char *Throwable::what() const throw()
{
    return mRepr;
}

ExceptionWithCode::ExceptionWithCode() : mCode(0) {}

ExceptionWithCode::ExceptionWithCode(const char *file, int line,
        const char *func, int code, const char *desc, ...) throw()
{
    va_list ap;
    va_start(ap, desc);
    ExceptionWithCode(file, line, func, code, desc, ap);
    va_end(ap);
}

ExceptionWithCode::ExceptionWithCode(const char *file, int line,
        const char *func, int code, const char *desc, va_list ap) throw() :
        mCode(code)
{
    char *tmp;
    vasprintf(&tmp, desc, ap);
    asprintf(&mRepr, "'%s' (%d) in %s at %s/%d: %s",
            getClassName(), code, func, file, line, tmp);
    free(tmp);
}

const char *ExceptionWithCode::getClassName() const throw()
{
    return "ExceptionWithCode";
}

ErrnoException::ErrnoException(const char *file, int line, const char *func,
        const char *desc, ...) throw()
{
    va_list ap;
    va_start(ap, desc);
    ErrnoException(file, line, func, desc, ap);
    va_end(ap);
}

ErrnoException::ErrnoException(const char *file, int line, const char *func,
        int errno_code, const char *desc, ...) throw()
{
    va_list ap;
    va_start(ap, desc);
    ErrnoException(file, line, func, errno_code, desc, ap);
    va_end(ap);
}

ErrnoException::ErrnoException(const char *file, int line, const char *func,
        const char *desc, va_list ap) throw()
{
    mCode = errno;
    char *tmp;
    vasprintf(&tmp, desc, ap);
    asprintf(&mRepr, "'%s' in %s at %s/%d: %s - %s",
            getClassName(), func, file, line, tmp, strerror(errno));
    free(tmp);
}

ErrnoException::ErrnoException(const char *file, int line, const char *func,
        int errno_code, const char *desc, va_list ap) throw()
{
    mCode = errno_code;
    char *tmp;
    vasprintf(&tmp, desc, ap);
    asprintf(&mRepr, "'%s' in %s at %s/%d: %s - %s",
            getClassName(), func, file, line, tmp, strerror(errno_code));
    free(tmp);
}

const char *ErrnoException::getClassName() const throw()
{
    return "ErrnoException";
}

}
