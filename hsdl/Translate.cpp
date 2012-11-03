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

#include "hsdl/Translate.h"

#include "hgame/Types.h"

#include "hsdl/Platform.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace hsdl {

using namespace std;

const char *Translate::operator()(const char *tag) const
{
    return mHash.at(tag);
}

Translate::Translate(Platform *platform) : mBuffer(0)
{
    char *filename;
    char ds = platform->getDirectorySeparator();
    asprintf(&filename, "%s%cstrings%cen", platform->getAssetsDirectory(),
            ds, ds);
    FILE *fp = fopen(filename, "r");
    if (fp)
    {
        fseek(fp, 0, SEEK_END);
        long len = ftell(fp);
        mBuffer = new char[len + 1];
        mBuffer[len] = 0;
        rewind(fp);
        fread(mBuffer, 1, len, fp);
        fclose(fp);
        int i = 0;
        while (i < len)
        {
            // Skip any extra terminators
            while (i < len && (mBuffer[i] == '\r' || mBuffer[i] == '\n'))
                ++i;
            if (i >= len)
                break;
            char *tag = mBuffer + i;
            char *msg = strchr(tag, ':');
            *msg = 0;
            ++msg;
            ++i;
            while (i < len && mBuffer[i] != '\r' && mBuffer[i] != '\n')
                ++i;
            if (i >= len)
                break;
            mBuffer[i++] = 0;
            mHash[tag] = msg;
        }
    }
    else
    {
        THROW(hgame::ErrnoException,
                "Couldn't open translation file '%s'", filename);
    }
    free(filename);
}

Translate::~Translate()
{
    delete mBuffer;
}

}
