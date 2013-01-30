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

import java.io.IOException;
import java.io.InputStream;

import uk.co.realh.hgame.Image;
import uk.co.realh.hgame.Log;
import uk.co.realh.hgame.RenderContext;
import uk.co.realh.hgame.Sprite;
import uk.co.realh.hgame.Sys;
import uk.co.realh.hgame.TextureAtlas;
import uk.co.realh.hgame.TextureRegion;
import uk.co.realh.hgame.TileBatcher;

/**
 * @author Tony Houghton
 * TODO: Create sprites etc.
 */
public class BombzTextures {
	
	private static String TAG = "Textures";
	
	TextureAtlas mTileAtlas, mAlphaAtlas, mLogoAtlas;
	TextureRegion[] mTileRegions = new TextureRegion[Cell.N_CELLS];
	TextureRegion[] mHeroRegions = new TextureRegion[4];
	TextureRegion mBomb1Region, mBomb2Region;
	Sprite mHeroSprite, mBombSprite, mLogoSprite, mExplo00Sprite;
	TileBatcher mTileBatcher;
	private Sys mSys;
	int mSrcTileSize;		// Source size
	
	// Used to work out how well a source tile size matches screen size.
	// The higher the better the match.
	private static final int NO_MATCH = 0;
	private static final int INT_RATIO = 1;
	private static final int EXACT = 2;
	
	/**
	 * @param sys
	 * @param screen_w	Width of screen
	 * @param screen_h	Height of screen
	 * @throws IOException 
	 */
	BombzTextures(Sys sys)
	{
		mSys = sys;
	}
	
	void calculateTileSize(RenderContext rctx, int screen_w, int screen_h)
			throws IOException
	{
		screen_h -= screen_h % (K.N_ROWS * K.DELTA_PIX);
		int best_size = 0;
		int best_goodness = NO_MATCH;
		for (String s: mSys.listFolder("pngs"))
		{
			int size = Integer.parseInt(s);
			int total = size * K.N_ROWS;
			int goodness;
			if (total == screen_h)
			{
				goodness = EXACT;
			}
			else
			{
				double ratio = (double) screen_h / (double) total;
				if (ratio == (double) ((int) ratio) ||
						(1.0 / ratio) == (double) ((int) (1.0 / ratio)))
				{
					goodness = INT_RATIO;
				}
				else
				{
					goodness = NO_MATCH;
				}
			}
			if ((goodness > best_goodness) ||
					((goodness == best_goodness) && size > best_size))
			{
				best_size = size;
				best_goodness = goodness;
			}
		}
		mSrcTileSize = best_size;
		Log.d(TAG, "Using source tile size " + mSrcTileSize);
		rctx.setNativeSize(best_goodness == EXACT);
	}
	
	private TextureAtlas loadAtlas(RenderContext rctx,
			String name, boolean alpha) throws IOException
	{
		rctx.enableBlend(false);
		InputStream fd = mSys.openAsset(
				"pngs/" + mSrcTileSize + "/" + name + ".png");
		Image img = mSys.loadPNG(fd, "name");
		fd.close();
		TextureAtlas atlas = rctx.uploadTexture(img, true);
		img.dispose();
		return atlas;
	}
	
	void loadTiles(RenderContext rctx) throws IOException
	{
		mTileAtlas = loadAtlas(rctx, "tile_atlas", false);
		for (int c = Cell.BLANK; c <= Cell.CHROME15; ++c)
		{
			int n = c - Cell.OFFSET;
			if (c < Cell.EXPLO00)
				mTileRegions[n] = createTileRegion(n);
			else if (c == Cell.EXPLO00)
				mTileRegions[n] = mTileRegions[Cell.BLANK - Cell.OFFSET];
			else
				mTileRegions[n] = createTileRegion(n);
		}
		createFusedRegions(Cell.BOMB1);
		createFusedRegions(Cell.BOMB2);
		mTileBatcher = rctx.createTileBatcher(K.N_COLUMNS, K.N_ROWS,
				K.FRUSTUM_TILE_SIZE, K.FRUSTUM_TILE_SIZE);
	}
	
	private void createFusedRegions(int bomb1or2)
	{
		int start = (bomb1or2 == Cell.BOMB2) ?
				Cell.BOMB2_FUSED_FIRST : Cell.BOMB1_FUSED_FIRST;
		for (int c = start;
				c <= start + Cell.BOMB1_FUSED_LAST - Cell.BOMB1_FUSED_FIRST;
				++c)
		{
			int n = c - Cell.OFFSET;
			mTileRegions[n] =
					mTileRegions[(((n & 4) == 0) ? Cell.BLANK : bomb1or2) -
					             Cell.OFFSET];
		}
	}
	
	private TextureRegion createTileRegion(int n)
	{
		int x = (n % K.TILE_ATLAS_COLUMNS) * mSrcTileSize;
		int y = (n / K.TILE_ATLAS_COLUMNS) * mSrcTileSize;
		return mTileAtlas.createRegion(x, y,
				x + mSrcTileSize, y + mSrcTileSize);
	}
	
	void deleteTiles(RenderContext rctx)
	{
		mTileBatcher = null;
		for (int n = 0; n < Cell.N_CELLS; ++n)
			mTileRegions[n] = null;
		if (null != mTileAtlas)
			mTileAtlas.dispose(rctx);
		mTileAtlas = null;
	}
	
	void loadAlphaTextures(RenderContext rctx) throws IOException
	{
		rctx.enableBlend(true);
		mAlphaAtlas = loadAtlas(rctx, "alpha_atlas", true);
		mExplo00Sprite = rctx.createSprite(mAlphaAtlas.createRegion(0, 0,
				3 * mSrcTileSize, 3 * mSrcTileSize),
				3 * K.FRUSTUM_TILE_SIZE, 3 * K.FRUSTUM_TILE_SIZE);
		mHeroRegions[K.FACING_LEFT] =
				mAlphaAtlas.createRegion(3 * mSrcTileSize, 0,
						mSrcTileSize, mSrcTileSize);
		mHeroRegions[K.FACING_RIGHT] =
				mAlphaAtlas.createRegion(4 * mSrcTileSize, 0,
						mSrcTileSize, mSrcTileSize);
		mHeroRegions[K.FACING_UP] =
				mAlphaAtlas.createRegion(3 * mSrcTileSize,
						mSrcTileSize, 
						mSrcTileSize, mSrcTileSize);
		mHeroRegions[K.FACING_DOWN] =
				mAlphaAtlas.createRegion(4 * mSrcTileSize,
						mSrcTileSize, 
						mSrcTileSize, mSrcTileSize);
		mBomb1Region = mAlphaAtlas.createRegion(3 * mSrcTileSize,
						2 * mSrcTileSize, 
						mSrcTileSize, mSrcTileSize);
		mBomb2Region = mAlphaAtlas.createRegion(4 * mSrcTileSize,
						2 * mSrcTileSize, 
						mSrcTileSize, mSrcTileSize);
		mHeroSprite = rctx.createSprite(mHeroRegions[K.FACING_UP],
					K.FRUSTUM_TILE_SIZE, K.FRUSTUM_TILE_SIZE);
		mBombSprite = rctx.createSprite(mBomb1Region,
					K.FRUSTUM_TILE_SIZE, K.FRUSTUM_TILE_SIZE);
	}
	
	void deleteAlphaTextures(RenderContext rctx)
	{
		mHeroSprite = null;
		for (int n = 0; n < 4; ++n)
			mHeroRegions[n] = null;
		mBombSprite = null;
		mBomb1Region = null;
		mBomb2Region = null;
		mExplo00Sprite = null;
		if (null != mAlphaAtlas)
			mAlphaAtlas.dispose(rctx);
		mAlphaAtlas = null;
	}
	
	void loadTitleLogo(RenderContext rctx) throws IOException
	{
		rctx.enableBlend(true);
		mLogoAtlas = loadAtlas(rctx, "title_logo", true);
		mLogoSprite = rctx.createSprite(mLogoAtlas.createRegion(
				0, 0, 16 * mSrcTileSize, 4 * mSrcTileSize),
				2 * K.FRUSTUM_TILE_SIZE, 2 * K.FRUSTUM_TILE_SIZE,
				16 * K.FRUSTUM_TILE_SIZE, 4 * K.FRUSTUM_TILE_SIZE);
	}
	
	void deleteLogoAtlas(RenderContext rctx)
	{
		mLogoSprite = null;
		if (null != mLogoAtlas)
			mLogoAtlas.dispose(rctx);
		mLogoAtlas = null;
	}
	
	/**
	 * Call in a deleteRendering handler and at the start of initRendering.
	 * 
	 * @see uk.co.realh.hgame.Renderer
	 */
	void deleteAllTextures(RenderContext rctx)
	{
		deleteTiles(rctx);
		deleteAlphaTextures(rctx);
		deleteLogoAtlas(rctx);
	}
	
	/**
	 * @throws IOException 
	 * @see uk.co.realh.hgame.Renderer
	 */
	void onResize(RenderContext rctx, int w, int h) throws IOException
	{
		calculateTileSize(rctx, w, h);
		if (null != mTileAtlas)
		{
			deleteTiles(rctx);
			loadTiles(rctx);
		}
		if (null != mAlphaAtlas)
		{
			deleteAlphaTextures(rctx);
			loadAlphaTextures(rctx);
		}
		if (null != mLogoAtlas)
		{
			deleteLogoAtlas(rctx);
			loadTitleLogo(rctx);
		}
	}
	
}
