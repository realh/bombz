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

// RenderContext.h: Abstract base class for texture-based rendering

#ifndef HGAME_RENDER_CONTEXT_H
#define HGAME_RENDER_CONTEXT_H

#include "config.h"

#include "hgame/Image.h"
#include "hgame/Log.h"
#include "hgame/TextureAtlas.h"

namespace hgame {

class Sprite;
class TextureRegion;
class TileBatcher;

class RenderContext {
private:
    const TextureAtlas *mBoundTexture;
    bool mNeedScaling;
    int mIndex;
    static int smIndex;
protected:
    Log &mLog;
    int mWidth;
    int mHeight;
    int mViewportWidth;
    int mViewportHeight;
public:
    RenderContext(Log *log);
    virtual ~RenderContext();

    int getWidth() const
    {
        return mWidth;
    }

    int getHeight() const
    {
        return mHeight;
    }

    // Viewport size is based on modes negotiation, not setViewport()
    int getViewportWidth() const
    {
        return mViewportWidth;
    }

    int getViewportHeight() const
    {
        return mViewportHeight;
    }

    // img dimensions must be powers of 2 in case using OpenGL
    virtual TextureAtlas *uploadTexture(Image *img) = 0;

    // Whether tiles need resizing to fit screen
    inline bool getNeedScaling() const
    {
        return mNeedScaling;
    }

    inline void setNeedScaling(bool f)
    {
        mNeedScaling = f;
    }

    virtual Sprite *createSprite(TextureRegion *texture,
            int width, int height) = 0;

    virtual TileBatcher *createTileBatcher(int nColumns, int nRows,
            int tile_size) = 0;

    // w and h are desired number of tiles across and down.
    // Returns tile size in pixels. Currently only supports square tiles
    // and pixels.
    int calculateTileSize(int w, int h);

    inline const TextureAtlas *getBoundTexture()
    {
        return mBoundTexture;
    }

    inline void bindTexture(const TextureAtlas *tex)
    {
        if (mBoundTexture != tex)
        {
            doBindTexture(tex);
            mBoundTexture = tex;
        }
    }

    // This can be used to detect whether RenderContext has really changed
    // in an initRendering request. -1 may be used to mean "no previous rc".
    inline int getIndex() const
    {
        return mIndex;
    }

    // Therefore this must be called whenever there is a RenderContext change
    inline void setIndex(int i = 0)
    {
        mIndex = i ? i : ++smIndex;
    }

    // Sets viewport for 2D rendering.
    virtual void setViewport2D(int left, int top, int width, int height) = 0;

    // Update display after rendering is complete
    virtual void flipBuffers() = 0;
protected:
    virtual void doBindTexture(const TextureAtlas *tex) = 0;
};

}

#endif // HGAME_RENDER_CONTEXT_H
