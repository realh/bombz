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

// TileBatcher: Render a tiled area in batches for efficiency

#ifndef HGLES_GL_TILE_BATCHER_H
#define HGLES_GL_TILE_BATCHER_H

#include "config.h"

#include "hgame/TileBatcher.h"
#include "hgame/Types.h"

#include "hgl/GLRenderContext.h"

namespace hgles {

class GLTileBatcher : public hgame::TileBatcher {
private:
    typedef hgame::HUInt8 IndexType;
    hgl::GLRenderContext *mRc;
    float *mVertices;
    float *mTexCoords;
    IndexType *mIndices;
public:
    GLTileBatcher(hgl::GLRenderContext *rc,
            int nColumns, int nRows, int tile_size);
    ~GLTileBatcher();

    // All regions must come from the same atlas
    void setTextureAt(hgame::TextureRegion *tex, int x, int y);
    void render(hgame::RenderContext *rc);
};

}

#endif // HGLES_GL_TILE_BATCHER_H
