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

// Image.h: Some sort of bitmap surface that can be used to upload
//          textures to OpenGL

#ifndef HGAME_IMAGE_H
#define HGAME_IMAGE_H

#include "config.h"

namespace hgame {

class Image {
public:

    // Create a new blank image in the same format as the current one
    virtual Image *createImage(int w, int h, bool alpha = true) = 0;
    
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
    
    // Used for creating drop shadows
    virtual unsigned int getAlphaAt(int x, int y) const = 0;
    virtual void setAlphaAt(int x, int y, unsigned int alpha) = 0;
    
    // Copies part of one image into current image
    virtual void blit(Image *src, int dest_x, int dest_y,
            int src_x, int src_y, int w, int h) = 0;

    virtual ~Image();
};

}

#endif // HGAME_IMAGE_H
