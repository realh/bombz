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

// Screen.h: A game's screen

#ifndef HGAME_ACTIVITY_H
#define HGAME_ACTIVITY_H

#include "config.h"

#include "hgame/Application.h"
#include "hgame/RendererWithState.h"

namespace hgame {

class Screen : public RendererWithState, public Runnable {
    // Note we implement Runnable. The run() function should consist mainly of
    // a main loop which sets the mRunning flag when starting and regularly
    // checks it, exiting the loop when it's false.
    // It will usually have its own thread, so it should
    // catch any unhandled exceptions at the top-level.
protected:
    Application *mApplication;
    Log &mLog;
    volatile bool mRunning;
    char *mName;
public:
    Screen(Application *app, const char *name);

    virtual ~Screen();

    inline Application *getApplication()
    {
        return mApplication;

    }

    inline RenderContext *getRenderContext()
    {
        return mApplication->getRenderContext();

    }

    inline Platform *getPlatform()
    {
        return mApplication->getPlatform();
    }

    // Return a 0-terminated array of ints in pairs of x, y;
    // sorted with best mode first;
    // must be able to delete[] result
    virtual int *getBestModes() = 0;
};

}

#endif // HGAME_ACTIVITY_H
