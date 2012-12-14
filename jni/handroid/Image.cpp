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

#include "handroid/Image.h"

#include "hgame/Log.h"

#include "handroid/Platform.h"

namespace hsdl {

Image::Image(Platform *platform, jobject bitmap) :
        mPlatform(platform), mBitmap(bitmap)
{
    mJEnv = mPlatform->getJNIEnv();
    mJEnv->NewGlobalRef(mBitmap);
    AndroidBitmap_getInfo(mJEnv, mBitmap, &mInfo);
}

Image::~Image()
{
    mJEnv = mPlatform->getJNIEnv();
    mJEnv->DeleteGlobalRef(mBitmap);
}

int Image::getWidth() const
{
    return mInfo.width;
}

int Image::getHeight() const
{
    return mInfo.height;
}

hgame::Image *Image::createImage(int w, int h)
{
    mJEnv = mPlatform->getJNIEnv();
    jclass img_class = mJEnv->FindClass("android/graphics/Bitmap");
    jmethodID ctor = 0;
    if (img_class)
        ctor = mJEnv->GetMethodID(img_class, "<init>", "(IIL)L");
    jmethodID get_config = 0;
    if (ctor)
        get_config = mJEnv->GetMethodID(img_class, "getConfig", "(V)L");
    jobject config = 0;
    if (get_config)
        config = mJEnv->CallObjectMethod(mBitmap, get_config);
    jobject bitmap = 0;
    if (config)
        bitmap = mJEnv->NewObject(helper_class, ctor, w, h, config);
    if (bitmap)
        return new Image(mPlatform, bitmap);
    else if (mJEnv->ExceptionOccurred())
        THROW(JavaException, "Image::createImage failed");
    return 0;
}

hgame::Colour Image::getColourAt(int x, int y)
{
    HUInt32 r = 0, g = 0, b = 0, a = 0;
    HUInt32 v = getPixelRawValue(x, y);
    switch (mInfo.format)
    {
        case ANDROID_BITMAP_FORMAT_RGBA_8888:
            r = (v >> 24) & 0xff;
            g = (v >> 16) & 0xff;
            b = (v >> 8) & 0xff;
            a = v & 0xff;
            break;
        case ANDROID_BITMAP_FORMAT_RGBA_565:
            r = ((v >> 11) & 0x1f) << 3;
            g = ((v >> 6) & 0x3f) << 2;
            b = (v & 0x1f) << 3;
            a = 0xff;
            break;
        case ANDROID_BITMAP_FORMAT_RGBA_4444:
            r = ((v >> 12) & 0xf) << 4;
            g = ((v >> 8) & 0xf) << 4;
            b = ((v >> 4) & 0xf) << 4;
            a = (v & 0xf) << 4;
            break;
        default:
            THROW(hgame::Throwable,
                    "Image::getColourAt: unsupported bitmap format %d",
                    mInfo.format);
            break;
    }
    return hgame::Colour(r, g, b, a);
}

void Image::setColourAt(int x, int y, hgame::Colour colour)
{
    HUInt32 v = 0;
    HUInt32 r = colour.getRed();
    HUInt32 g = colour.getGreen();
    HUInt32 b = colour.getBlue();
    HUInt32 a = colour.getAlpha();
    switch (mInfo.format)
    {
        case ANDROID_BITMAP_FORMAT_RGBA_8888:
            v = (r << 24) | (g << 16) | (b << 8) | a;
            break;
        case ANDROID_BITMAP_FORMAT_RGBA_565:
            v = ((r >> 3) << 11) | ((g >> 2) << 5) | (g >> 3);
            break;
        case ANDROID_BITMAP_FORMAT_RGBA_4444:
            v = ((r >> 4) << 12) | ((g >> 4) << 8) | (b & 0xf0) | (a >> 4);
            break;
        default:
            THROW(hgame::Throwable,
                    "Image::setColourAt: unsupported bitmap format %d",
                    mInfo.format);
            break;
    }
    setPixelRawValue(x, y, v);
}

HUInt8 Image::getAlphaAt(int x, int y)
{
    HUInt32 v = getPixelRawValue(x, y);
    switch (mInfo.format)
    {
        case ANDROID_BITMAP_FORMAT_RGBA_8888:
            return v & 0xff;
            break;
        case ANDROID_BITMAP_FORMAT_RGBA_565:
            return 0xff;
            break;
        case ANDROID_BITMAP_FORMAT_RGBA_4444:
            return (v & 0xf) << 4;
            break;
        default:
            THROW(hgame::Throwable,
                    "Image::getAlphaAt: unsupported bitmap format %d",
                    mInfo.format);
            break;
    }
    return 0xff;
}

void Image::setAlphaAt(int x, int y, HUInt8 alpha)
{
    void *addr;
    HUInt32 v = getPixelRawValue(x, y, &addr);
    switch (mInfo.format)
    {
        case ANDROID_BITMAP_FORMAT_RGBA_8888:
            v = (v & 0xffffff00) | alpha;
            break;
        case ANDROID_BITMAP_FORMAT_RGBA_565:
            THROW(hgame::Throwable,
                    "Image::setAlphaAt: RGB_565 does not support alpha");
            return;
        case ANDROID_BITMAP_FORMAT_RGBA_4444:
            v = (v & 0xfff0) | (alpha > 4);
            break;
        default:
            THROW(hgame::Throwable,
                    "Image::setAlphaAt: unsupported bitmap format %d",
                    mInfo.format);
            break;
    }
    setPixelRawValue(addr, v);
}

void Image::setPixelRawValue(int x, int y, HUInt32 pix)
{
    setPixelRawValue(getPixelAddr(x, y), pix);
}

void Image::setPixelRawValue(void *addr, HUInt32 pix)
{
    if (!addr)
        return;
    switch (mInfo.format)
    {
        case ANDROID_BITMAP_FORMAT_RGBA_8888:
            *((UInt32 *) addr) = pix;
            break;
        case ANDROID_BITMAP_FORMAT_RGBA_565:
        case ANDROID_BITMAP_FORMAT_RGBA_4444:
            *((UInt16 *) addr) = pix;
            break;
        default:
            THROW(hgame::Throwable,
                    "Image::setPiexelRawValue: unsupported bitmap format %d",
                    mInfo.format);
            break;
    }
}

void *Image::getPixelAddr(int x, int y)
{
    if (x < 0 || x >= mSurface->w || y < 0 || y >= mSurface->h)
        return 0;
    return (void *) (((char *) mSurface->pixels) + mSurface->pitch * y +
            x * mSurface->format->BytesPerPixel);
}

Uint32 Image::getPixelRawValue(int x, int y, void **pAddr)
{
    SDL_PixelFormat *fmt = mSurface->format;
    void *ppix = getPixelAddr(x, y);
    if (pAddr)
        *pAddr = ppix;
    if (!ppix)
        return 0;
    if (fmt->BitsPerPixel >= 24)
    {
        return *((Uint32 *) ppix);
    }
    else if (fmt->BitsPerPixel >= 15)
    {
        return (Uint32) *((Uint16 *) ppix);
    }
    return (Uint32) *((Uint8 *) ppix);
}

void Image::lock()
{
    if (SDL_LockSurface(mSurface))
    {
        THROW(Exception, "Unable to lock surface");
    }
}

void Image::unlock()
{
    SDL_UnlockSurface(mSurface);
}

}
