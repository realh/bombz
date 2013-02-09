/*
 * Copyright (c) 2013, Tony Houghton <h@realh.co.uk>
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

/**
 * HGame - a simple game app framework targeted at Android.
 *
 * @author Tony Houghton &lt;h@realh.co.uk&gt;
 *
 * Distributed under the terms of the Simplified BSD Licence.
 * See the source code for details.
 */
package uk.co.realh.bombz;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.Random;

import uk.co.realh.hgame.RenderContext;

/**
 * @author Tony Houghton
 *
 */
public class BombzLevel {
	
	private byte[] mTiles = new byte[K.N_COLUMNS * K.N_ROWS];
	private byte[] mTmpTiles = new byte[K.N_COLUMNS * K.N_ROWS];
	private BombzTextures mTextures;
	private Random mRandom = new Random();
	
	int mNBombs;
	int mStartX;
	int mStartY;
	int mTimeLimit;
	
	private boolean mBombActivity;
	
	public BombzLevel(BombzTextures textures) {
		mTextures = textures;
	}
	
	public void render(RenderContext rctx) {
		int n = 0;
		for (int y = 0; y < K.N_ROWS; ++y) {
			for (int x = 0; x < K.N_COLUMNS; ++x) {
				mTextures.mTileBatcher.setTextureAt(
						mTextures.mTileRegions[((int) mTiles[n]) - Cell.OFFSET],
						x, y);
				++n;
			}
			
		}
		mTextures.mTileBatcher.render(rctx);
	}

	public void load(BufferedReader fd) throws IOException {
	    int n = 0;
		for (int y = 0; y < K.N_ROWS; ++y) {
			String s = fd.readLine();
			for (int x = 0; x < K.N_COLUMNS; ++x) {
	            switch (s.charAt(x))
	            {
	                case ' ':
	                    mTiles[n] = Cell.BLANK;
	                    break;
	                case '.':
	                    mTiles[n] = Cell.EARTH;
	                    break;
	                case 'D':
	                    mTiles[n] = Cell.MATCH;
	                    break;
	                case 'B':
	                    mTiles[n] = Cell.PICKET;
	                    break;
	                case 'O':
	                    mTiles[n] = Cell.BOMB1;
	                    ++mNBombs;
	                    break;
	                case '0':
	                    mTiles[n] = Cell.BOMB1_FUSED_FIRST;
	                    ++mNBombs;
	                    mBombActivity = true;
	                    break;
	                case 'S':
	                    mTiles[n] = Cell.BLANK;
	                    mStartX = x;
	                    mStartY = y;
	                    break;
	                case 'X':
	                    mTiles[n] = Cell.CHROME15;
	                    break;
	                default:
	                    mTiles[n] = Cell.BLANK;
	                    break;
	            }
	            ++n;
	        }
	    }
	    mTimeLimit = Integer.parseInt(fd.readLine());
	    prettify();
	}
	
	public byte getTileAt(int x, int y)
	{
		if (x < 0 || x >= K.N_COLUMNS || y < 0 || y >= K.N_ROWS)
			return Cell.OUTSIDE;
		return mTiles[y * K.N_COLUMNS + x];
	}
	
	/**
	 * @param x
	 * @param y
	 * @param t			Tile type (Cell constant)
	 */
	public void setTileAt(int x, int y, int t)
	{
		mTiles[y * K.N_COLUMNS + x] = (byte) t;
	}
	
	/**
	 * @param x
	 * @param y
	 * @param t			Tile type (Cell constant)
	 * @param activate	Whether to set mBombActivity flag
	 */
	public void setTileAt(int x, int y, int t, boolean activate)
	{
		mTiles[y * K.N_COLUMNS + x] = (byte) t;
		if (activate)
			mBombActivity = true;
	}
	
	public void setBlankAt(int x, int y)
	{
		mTiles[y * K.N_COLUMNS + x] = Cell.BLANK;
	}
	
	public boolean isChromeAt(int x, int y)
	{
		byte t = getTileAt(x, y);
		return t >= Cell.CHROME00 && t <= Cell.CHROME15;
	}
	
	/**
	 * @return	Whether anything happened, so screen refresh required
	 */
	public boolean step() {
	    if (!mBombActivity)
	        return false;
	    mBombActivity = false;
	    for (int y = 0; y < K.N_ROWS; ++y)
	    {
	        for (int x = 0; x < K.N_COLUMNS; ++x)
	        {
	            byte c = getTileAt(x, y);
	            if ((c >= Cell.BOMB1_FUSED_FIRST &&
	                        c < Cell.BOMB1_FUSED_LAST) ||
	                    (c >= Cell.BOMB2_FUSED_FIRST &&
	                        c < Cell.BOMB2_FUSED_LAST) ||
	                    (c > Cell.EXPLO00 && c < Cell.EXPLO11))
	            {
	                setTileAt(x, y, c + 1);
	                mBombActivity = true;
	            }
	            else if (c == Cell.PRE_EXPLO)
	            {
	                setTileAt(x, y, Cell.EXPLO00 + 1);
	                mBombActivity = true;
	            }
	            else if (c == Cell.EXPLO00)
	            {
	                mBombActivity = true;
	                for (int y0 = (y >= 1 ? y - 1 : y);
	                        y0 <= (y < K.N_ROWS - 1 ? y + 1 : y);
	                        ++y0)
	                {
	                    for (int x0 = (x >= 1 ? x - 1 : x);
	                            x0 <= (x < K.N_COLUMNS - 1 ? x + 1 : x);
	                            ++x0)
	                    {
	                        setExploAt(x0, y0, y0 < y || (y0 == y && x0 <= x));
	                    }
	                }
	            }
	            else if (c == Cell.BOMB1_FUSED_LAST ||
	                    c == Cell.BOMB2_FUSED_LAST)
	            {
	                --mNBombs;
	                mBombActivity = true;
	                setTileAt(x, y, Cell.EXPLO00);
	            }
	            else if (c == Cell.EXPLO11)
	            {
	                setBlankAt(x, y);
	            }
	        }
	    }
	    return true;
	}
	
	/**
	 * @param x
	 * @param y
	 * @param behind	Whether target tile has already been processed by step
	 */
	private void setExploAt(int x, int y, boolean behind) {
	    int c = getTileAt(x, y);
	    if (c == Cell.BOMB1 ||
	            (c >= Cell.BOMB1_FUSED_FIRST &&
	            c <= Cell.BOMB1_FUSED_LAST))
	    {
	        if (behind)
	            setTileAt(x, y, Cell.BOMB1_FUSED_LAST);
	        else
	            setTileAt(x, y, Cell.BOMB1_FUSED_LAST - 1);
	    }
	    else if (c == Cell.BOMB2 ||
	            (c >= Cell.BOMB2_FUSED_FIRST &&
	            c <= Cell.BOMB2_FUSED_LAST))
	    {
	        if (behind)
	            setTileAt(x, y, Cell.BOMB2_FUSED_LAST);
	        else
	            setTileAt(x, y, Cell.BOMB2_FUSED_LAST - 1);
	    }
	    else if (c < Cell.CHROME00 || c > Cell.CHROME15)
	    {
	        if (behind)
	            setTileAt(x, y, Cell.EXPLO00 + 1);
	        else
	            setTileAt(x, y, Cell.PRE_EXPLO);
	    }
	}

	private void prettify() {
		hollowChrome();
		shapeChrome();
		randomiseBombs();
	}
	
	private void swapTiles()
	{
		byte[] tmp = mTmpTiles;
		mTmpTiles = mTiles;
		mTiles = tmp;
	}
	
	private void hollowChrome() {
	    int n = 0;
	    for (int y = 0; y < K.N_ROWS; ++y)
	    {
	        for (int x = 0; x < K.N_COLUMNS; ++x, ++n)
	        {
	            if (isChromeAt(x, y))
	            {
	                boolean nonChrome = false;
	                for (int y0 = y - 1; !nonChrome && y0 <= y + 1; ++y0)
	                {
	                    for (int x0 = x - 1; !nonChrome && x0 <= x + 1; ++x0)
	                    {
	                        if (!isChromeAt(x0, y0))
	                        {
	                            nonChrome = true;
	                        }
	                    }
	                }
	                mTmpTiles[n] = nonChrome ? Cell.CHROME15 : Cell.BLANK;
	            }
	            else
	            {
	                mTmpTiles[n] = mTiles[n];
	            }
	        }
	    }
	    swapTiles();
	}

	private void shapeChrome() {
	    int n = 0;
	    final byte C = Cell.CHROME00;
	    for (int y = 0; y < K.N_ROWS; ++y)
	    {
	        for (int x = 0; x < K.N_COLUMNS; ++x, ++n)
	        {
	            if (isChromeAt(x, y))
	            {
	                boolean left = isChromeAt(x - 1, y);
	                boolean right = isChromeAt(x + 1, y);
	                boolean above = isChromeAt(x, y - 1);
	                boolean below = isChromeAt(x, y + 1);
	                if (left)
	                {
	                    if (right)
	                    {
	                        if (above)
	                        {
	                            if (below)
	                            {
	                                //   |
	                                // --+--
	                                //   |
	                                mTmpTiles[n] = C + 8;
	                            }
	                            else
	                            {
	                                //   |
	                                // --+--
	                                //
	                                mTmpTiles[n] = C + 11;
	                            }
	                        }
	                        else	// left && right && !above
	                        {
	                            if (below)
	                            {
	                                //
	                                // --+--
	                                //   |
	                                mTmpTiles[n] = C + 13;
	                            }
	                            else
	                            {
	                                //
	                                // --+--
	                                //
	                                mTmpTiles[n] = C + 1;
	                            }
	                        }
	                    }
	                    else	// left && !right
	                    {
	                        if (above)
	                        {
	                            if (below)
	                            {
	                                //   |
	                                // --+
	                                //   |
	                                mTmpTiles[n] = C + 12;
	                            }
	                            else
	                            {
	                                //   |
	                                // --+
	                                //
	                                mTmpTiles[n] = C + 5;
	                            }
	                        }
	                        else	// left && !right && !above
	                        {
	                            if (below)
	                            {
	                                //
	                                // --+
	                                //   |
	                                mTmpTiles[n] = C + 2;
	                            }
	                            else
	                            {
	                                //
	                                // --+
	                                //
	                                mTmpTiles[n] = C + 9;
	                            }
	                        }
	                    }
	                }
	                else	// !left
	                {
	                    if (right)
	                    {
	                        if (above)
	                        {
	                            if (below)
	                            {
	                                //   |
	                                //   +--
	                                //   |
	                                mTmpTiles[n] = C + 14;
	                            }
	                            else
	                            {
	                                //   |
	                                //   +--
	                                //
	                                mTmpTiles[n] = C + 4;
	                            }
	                        }
	                        else	// !left && right && !above
	                        {
	                            if (below)
	                            {
	                                //
	                                //   +--
	                                //   |
	                                mTmpTiles[n] = C;
	                            }
	                            else
	                            {
	                                //
	                                //   +--
	                                //
	                                mTmpTiles[n] = C + 7;
	                            }
	                        }
	                    }
	                    else	// !left && !right
	                    {
	                        if (above)
	                        {
	                            if (below)
	                            {
	                                //   |
	                                //   +
	                                //   |
	                                mTmpTiles[n] = C + 3;
	                            }
	                            else
	                            {
	                                //   |
	                                //   +
	                                //
	                                mTmpTiles[n] = C + 10;
	                            }
	                        }
	                        else	// !left && right && !above
	                        {
	                            if (below)
	                            {
	                                //
	                                //   +
	                                //   |
	                                mTmpTiles[n] = C + 6;
	                            }
	                            else
	                            {
	                                //
	                                //   +
	                                //
	                                mTmpTiles[n] = C + 15;
	                            }
	                        }
	                    }
	                }
	            }
	            else
	            {
	                mTmpTiles[n] = mTiles[n];
	            }
	        }
	    }
	    swapTiles();
	    disconnectTs();
	}
	
	/**
	 * If we have two parallel rows or columns they will be unnecessarily
	 * linked with T sections ie for a nicer look we can replace:
	 *
	 *    ===========         ===========
	 *    ==+=+=+=+==         ===========
	 *      | | | |     with
	 *    ==+=+=+=+==         ===========
	 *    ===========         ===========
	 */
	private void disconnectTs() {
	    // Don't need to use tmpLvl for this
	    int n = 0;
	    final byte C = Cell.CHROME00;
	    for (int y = 0; y < K.N_ROWS; ++y)
	    {
	        for (int x = 0; x < K.N_COLUMNS; ++x, ++n)
	        {
	            int tile = getTileAt(x, y);
	            if (tile == C + 13)
	            {
	                // --+--
	                //   |
	                if (getTileAt(x, y + 1) == C + 11)
	                {
	                    //   |
	                    // --+--
	                    mTiles[n] = C + 1;	                // --+--
	                    mTiles[n + K.N_COLUMNS] = C + 1;  // --+--
	                }
	            }
	            else if (tile == C + 14)
	            {
	                // |
	                // +--
	                // |
	                if (getTileAt(x + 1, y) == C + 12)
	                {
	                    //   |
	                    // --+
	                    //   |
	                    mTiles[n] = C + 3;      //   ||
	                    mTiles[n + 1] = C + 3;  //   ++
	                                            //   ||
	                }
	            }
	        }
	    }
	}
	
	private void randomiseBombs() {
	    int n = 0;
	    for (int y = 0; y < K.N_ROWS; ++y)
	    {
	        for (int x = 0; x < K.N_COLUMNS; ++x, ++n)
	        {
	            if (mTiles[n] == Cell.BOMB1 && mRandom.nextBoolean())
	                mTiles[n] = Cell.BOMB2;
	            else if (mTiles[n] == Cell.BOMB1_FUSED_FIRST && mRandom.nextBoolean())
	                mTiles[n] = Cell.BOMB2_FUSED_FIRST;
	        }
	    }
	}

	/**
	 * @param x				Target pos
	 * @param y				Target pos
	 * @param dx			Direction
	 * @param dy			Direction
	 * @param have_match	Player has match
	 * @return				Whether move is possible
	 */
	boolean canMoveTo(int x, int y, int dx, int dy,
			boolean have_match)
	{
		return canMoveTo(x, y, dx, dy, have_match, false);
	}
	
	/**
	 * @param x				Target pos
	 * @param y				Target pos
	 * @param dx			Direction
	 * @param dy			Direction
	 * @param have_match	Player has match
	 * @param bomb			Test is whether bomb can move here
	 * @return				Whether move is possible
	 */
	private boolean canMoveTo(int x, int y, int dx, int dy,
			boolean have_match, boolean bomb)
	{
	    byte c = getTileAt(x, y);
	    switch (c)
	    {
        case Cell.OUTSIDE:
        	return false;
        case Cell.BLANK:
            return true;
        case Cell.EARTH:
        case Cell.MATCH:
            return !bomb;
        case Cell.PICKET:
            return false;
        case Cell.BOMB1:
        case Cell.BOMB2:
            if (bomb)
                return false;
            else if (have_match)
                return true;
            else
                return canMoveTo(x + dx, y + dy, dx, dy, false, true);
		default:
            if (c >= Cell.EXPLO00 && c <= Cell.EXPLO11)
                return true;
            break;
	    }
	    return false;
	}
	
}
