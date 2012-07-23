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

#include "sdl/Image.h"

#include "hgame/Log.h"
#include "hgame/Platform.h"

#include "sdl/Exception.h"

namespace sdl {

Image::~Image()
{
    SDL_FreeSurface(surface);
}
    
Image::Format Image::getFormat() const
{
    const SDL_PixelFormat *fmt = surface->format;
    hgame::Platform::Endianness endn = hgame::Platform::getEndianness();
    switch (fmt->BitsPerPixel)
    {
        case 32:
            if (endn == hgame::Platform::HGAME_LITTLE_ENDIAN &&
                    fmt->Rmask == 0xff00 &&
                    fmt->Gmask == 0xff0000 &&
                    fmt->Bmask == 0xff000000)
            {
                if (fmt->Amask == 0xff)
                    return hgame::Image::ARGB_8888;
                else
                    return hgame::Image::RGB_888;
            }
            else if (endn == hgame::Platform::HGAME_BIG_ENDIAN &&
                    fmt->Rmask == 0xff0000 &&
                    fmt->Gmask == 0xff00 &&
                    fmt->Bmask == 0xff)
            {
                if (fmt->Amask == 0xff000000)
                    return hgame::Image::ARGB_8888;
                else
                    return hgame::Image::RGB_888;
            }
            else if (endn == hgame::Platform::HGAME_LITTLE_ENDIAN &&
                    fmt->Rmask == 0xff &&
                    fmt->Gmask == 0xff00 &&
                    fmt->Bmask == 0xff0000)
            {
                if (fmt->Amask == 0xff000000)
                    return hgame::Image::RGBA_8888;
                else
                    return hgame::Image::RGB_888;
            }
            else if (endn == hgame::Platform::HGAME_BIG_ENDIAN &&
                    fmt->Rmask == 0xff000000 &&
                    fmt->Gmask == 0xff0000 &&
                    fmt->Bmask == 0xff00)
            {
                if (fmt->Amask == 0xff)
                    return hgame::Image::RGBA_8888;
                else
                    return hgame::Image::RGB_888;
            }
            break;
        case 24:
            if ((endn == hgame::Platform::HGAME_LITTLE_ENDIAN &&
                    fmt->Rmask == 0xff &&
                    fmt->Gmask == 0xff00 &&
                    fmt->Bmask == 0xff0000) ||
                (endn == hgame::Platform::HGAME_BIG_ENDIAN &&
                    fmt->Rmask == 0xff0000 &&
                    fmt->Gmask == 0xff00 &&
                    fmt->Bmask == 0xff))
            {
                return hgame::Image::RGB_888;
            }
            break;
        case 16:
            if ((endn == hgame::Platform::HGAME_LITTLE_ENDIAN &&
                    fmt->Amask == 0xf &&
                    fmt->Rmask == 0xf0 &&
                    fmt->Gmask == 0xf00 &&
                    fmt->Bmask == 0xf000) ||
                (endn == hgame::Platform::HGAME_BIG_ENDIAN &&
                    fmt->Amask == 0xf000 &&
                    fmt->Rmask == 0xf00 &&
                    fmt->Gmask == 0xf0 &&
                    fmt->Bmask == 0xf))
            {
                return hgame::Image::ARGB_4444;
            }
            else if ((endn == hgame::Platform::HGAME_LITTLE_ENDIAN &&
                    fmt->Amask == 0xf000 &&
                    fmt->Rmask == 0xf &&
                    fmt->Gmask == 0xf0 &&
                    fmt->Bmask == 0xf00) ||
                (endn == hgame::Platform::HGAME_BIG_ENDIAN &&
                    fmt->Amask == 0xf &&
                    fmt->Rmask == 0xf000 &&
                    fmt->Gmask == 0xf00 &&
                    fmt->Bmask == 0xf0))
            {
                return hgame::Image::RGBA_4444;
            }
            else if ((endn == hgame::Platform::HGAME_LITTLE_ENDIAN &&
                    fmt->Rmask == 0x1f &&
                    fmt->Gmask == 0x3e0 &&
                    fmt->Bmask == 0xfc00) ||
                (endn == hgame::Platform::HGAME_BIG_ENDIAN &&
                    fmt->Rmask == 0xfc00 &&
                    fmt->Gmask == 0x3e0 &&
                    fmt->Bmask == 0xfc00))
            {
                return hgame::Image::RGB_565;
            }
            break;
    }
    THROW(Exception, "Unknown pixel format");
    return UNKNOWN;
}

int Image::getWidth() const
{
    return surface->w;
}

int Image::getHeight() const
{
    return surface->h;
}

}
