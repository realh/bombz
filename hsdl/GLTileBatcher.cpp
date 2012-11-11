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

#include "hsdl/GLTileBatcher.h"

#include "hgl/GLTextureRegion.h"

#include <cstring>

/*
#include <cstdio>
using namespace std;
*/

namespace hsdl {

GLTileBatcher::GLTileBatcher(GLRenderContext *rc,
        int nColumns, int nRows, int tile_size) :
        hgame::TileBatcher(nColumns, nRows, tile_size),
        mRc(rc)
{
    // Interleaving vertices and text coords may allow slightly more
    // efficient memory access.
    // 4 points per quad * 2 (x, y) * 2 (screen & tex coords)
    int nCoords = nColumns * nRows * 4 * 2 * 2;
    mVertices = new float[nCoords];
    int i = 0;
    for (int y = 0; y < nRows; ++y)
    {
        for (int x = 0; x < nColumns; ++x)
        {
            mVertices[i++] = float(x * tile_size);
            mVertices[i++] = float(y * tile_size);
            mVertices[i++] = float(x * tile_size);
            mVertices[i++] = float((y + 1) * tile_size);
            mVertices[i++] = float((x + 1) * tile_size);
            mVertices[i++] = float((y + 1) * tile_size);
            mVertices[i++] = float((x + 1) * tile_size);
            mVertices[i++] = float(y * tile_size);
            i += 8;
        }
    }
}

GLTileBatcher::~GLTileBatcher()
{
    delete mVertices;
}

void GLTileBatcher::setTextureAt(hgame::TextureRegion *tex, int x, int y)
{
    std::memcpy(mVertices + (y * mNColumns + x) * 16 + 8,
            ((hgl::GLTextureRegion *) tex)->getCoords(), 8 * sizeof(float));
}

void GLTileBatcher::render(hgame::RenderContext *rc)
{
    /*
    int n;
    for (n = 0; n < 8; n += 2)
    {
        fprintf(stderr, "(%6.1f, %6.1f)  ", mVertices[n], mVertices[n + 1]);
    }
    fprintf(stderr, "\n");
    for (n = 8; n < 16; n += 2)
    {
        fprintf(stderr, "(%6.4f, %6.4f)  ", mVertices[n], mVertices[n + 1]);
    }
    fprintf(stderr, "\n");
    */

    // Do one row at a time because there may be a limit to how many
    // verts we can do in one go
    int nVerts = mNColumns * 4;
    // stride is 4 * 2 * 2 coords * 4 bytes per float
    for (int y = 0; y < mNRows; ++y)
    {
        glVertexPointer(2, GL_FLOAT, 64, mVertices + y * mNColumns * 16);
        glTexCoordPointer(2, GL_FLOAT, 64, mVertices + y * mNColumns * 16 + 8);
        glDrawArrays(GL_QUADS, 0, nVerts);
    }
}

}
