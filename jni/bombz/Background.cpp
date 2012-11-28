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

// Bombz - 2D puzzle game

#include "bombz/Background.h"

#include "bombz/Level.h"

namespace bombz {

void Background::initRendering(hgame::RenderContext *rc)
{
}

void Background::deleteRendering(hgame::RenderContext *rc)
{
}

void Background::render(hgame::RenderContext *rc)
{
    int n, x, y;
    for (n = 0, y = 0; y < Level::kHeight; ++y)
    {
        for (x = 0; x < Level::kWidth; ++x, ++n)
        {
            int t = Tiles::BLANK;
            if (y == 0 && x == 0)
                t = Tiles::CHROME00;
            else if (y == 0 && x == Level::kWidth - 1)
                t = Tiles::CHROME00 + 2;
            else if (y == Level::kHeight - 1 && x == 0)
                t = Tiles::CHROME00 + 4;
            else if (y == Level::kHeight - 1 && x == Level::kWidth - 1)
                t = Tiles::CHROME00 + 5;
            else if (y == 0 || y == Level::kHeight - 1)
                t = Tiles::CHROME00 + 1;
            else if (x == 0 || x == Level::kWidth - 1)
                t = Tiles::CHROME00 + 3;
            mTiles->setTileAt(t, x, y);
        }
    }
    mTiles->render(rc);
}

}
