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

// Image.h: Wrapper for Android Bitmap

#ifndef HANDROID_IMAGE_H
#define HANDROID_IMAGE_H

#include "config.h"

#include <android/bitmap.h>

#include "hgame/Image.h"

#include "handroid/Types.h"

namespace handroid {

class Platform;

class Image : public hgame::Image {
private:
    // mJEnv and mPixAddr are only valid while locked
    Platform *mPlatform;
    JNIEnv *mJEnv;
    jobject mBitmap;
    AndroidBitmapInfo mInfo;
    void *mPixAddr;
public:
    // If jenv is not given it's obtained from platform
    Image(Platform *platform, jobject bitmap, JNIEnv *jenv = 0);
    ~Image();

    int getWidth() const;
    int getHeight() const;

    inline HUInt32 getStride() const
    {
        return (int) mInfo.stride;
    }

    inline HUInt32 getFormat() const
    {
        return mInfo.format;
    }

    // Use with care, only while locked!
    inline void *getRawPixelData()
    {
        return mPixAddr;
    }

    hgame::Colour getColourAt(int x, int y);
    void setColourAt(int x, int y, hgame::Colour colour);

    HUInt8 getAlphaAt(int x, int y);
    void setAlphaAt(int x, int y, HUInt8 alpha);

    hgame::Image *createImage(int w, int h);

    // Already have a usable blit in base class due to SDL_BlitSurface
    // being broken, so might as well use that instead of JNI
    /*
    void blit(hgame::Image *src, int dest_x, int dest_y,
            int src_x, int src_y, int w, int h);
    */

    void lock();
    void unlock();
private:
    int getBpp() const;     // bytes per pixel
    void *getPixelAddr(int x, int y);
    HUInt32 getPixelRawValue(int x, int y, void **pAddr = 0);
    void setPixelRawValue(int x, int y, HUInt32 pix);
    void setPixelRawValue(void *addr, HUInt32 pix);
};

}

#endif // HANDROID_IMAGE_H
