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
	// 4 "live" pushers, 4 "dead"
	TextureRegion[] mPusherRegions = new TextureRegion[8];
	TextureRegion mBomb1Region, mBomb2Region;
	Sprite mPusherSprite, mBombSprite, mLogoSprite, mExplo00Sprite;
	TileBatcher mTileBatcher;
	int mControlsType;
	TextureAtlas mControlsAtlas;
	TextureRegion[] mControlsRegions = new TextureRegion[4];
	Sprite[] mControlsSprites = new Sprite[4];
	int mVpadWidth, mVpadHeight;
	
	// 11th "digit" is colon
	TextureRegion[] mYellowDigitRegions = new TextureRegion[11];
	TextureRegion[] mRedDigitRegions = new TextureRegion[11];
	TextureRegion mStar1Region, mStar2Region;
	TextureRegion mHourglassRegion;
	// 4 digits + colon + hourglass
	Sprite[] mClockSprites = new Sprite[6];
	Sprite mAlphaSprite;	// General purpose

	private Sys mSys;
	int mSrcTileSize;		// Source size
	int mViewportWidth, mViewportHeight;

	// Used to work out how well a source tile size matches screen size.
	// The higher the better the match.
	private static final int NO_MATCH = 0;
	private static final int INT_RATIO = 1;
	private static final int EXACT = 2;

	/**
	 * @param sys
	 * @param screen_w	Width of screen
	 * @param screen_h	Height of screen
	 * @param controlsType	One of controls constants
	 * @see uk.co.realh.hgame.K
	 * @throws IOException
	 */
	BombzTextures(Sys sys, int controlsType)
	{
		mSys = sys;
		setControlsType(controlsType);
	}

	final void setControlsType(int c)
	{
		if (mSys.usesTouchScreen())
			mControlsType = c;
		else
			mControlsType = 0;
	}

	void calculateTileSize(RenderContext rctx, int screen_w, int screen_h)
			throws IOException
	{
		mViewportHeight = screen_h - (screen_h % K.N_ROWS);
		mViewportWidth = (mViewportHeight * 4) / 3;
		int best_size = 0;
		int best_goodness = NO_MATCH;
		for (String s: mSys.listFolder("pngs"))
		{
			int size = Integer.parseInt(s);
			int total = size * K.N_ROWS;
			int goodness;
			if (total == mViewportHeight)
			{
				goodness = EXACT;
			}
			else
			{
				double ratio = (double) mViewportHeight / (double) total;
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
		Log.d(TAG, "Using source tile size " + mSrcTileSize +
				", viewport " + mViewportWidth + "x" + mViewportHeight);
		rctx.setNativeSize(best_goodness == EXACT);
	}

	private TextureAtlas loadAtlas(RenderContext rctx,
			String name, boolean alpha) throws IOException
	{
		rctx.enableBlend(alpha);
		InputStream fd = mSys.openAsset(
				"pngs/" + mSrcTileSize + "/" + name + ".png");
		Image img = mSys.loadPNG(fd, name);
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
				mTileRegions[n] = createTileRegion(n - 1);
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
					mTileRegions[((((c - start) & 4) == 0) ?
							Cell.BLANK : bomb1or2) - Cell.OFFSET];
		}
	}

	private TextureRegion createTileRegion(int n)
	{
		int x = (n % K.TILE_ATLAS_COLUMNS) * mSrcTileSize;
		int y = (n / K.TILE_ATLAS_COLUMNS) * mSrcTileSize;
		return mTileAtlas.createRegion(x, y, mSrcTileSize, mSrcTileSize);
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
		
		for (int n = 0; n < 8; ++n) {
			mPusherRegions[n] = mAlphaAtlas.createRegion(n * mSrcTileSize,
						3 * mSrcTileSize, mSrcTileSize, mSrcTileSize);
		}
		
		mBomb1Region = mAlphaAtlas.createRegion(3 * mSrcTileSize,
						2 * mSrcTileSize,
						mSrcTileSize, mSrcTileSize);
		mBomb2Region = mAlphaAtlas.createRegion(4 * mSrcTileSize,
						2 * mSrcTileSize,
						mSrcTileSize, mSrcTileSize);
		mPusherSprite = rctx.createSprite(mPusherRegions[K.FACING_UP],
					K.FRUSTUM_TILE_SIZE, K.FRUSTUM_TILE_SIZE);
		mBombSprite = rctx.createSprite(mBomb1Region,
					K.FRUSTUM_TILE_SIZE, K.FRUSTUM_TILE_SIZE);
		
		for (int n = 0; n < 11; ++n) {
			int x = 3 * mSrcTileSize + n * 3 * mSrcTileSize / 8;
			mYellowDigitRegions[n] = mAlphaAtlas.createRegion(
					x, 0, 3 * mSrcTileSize / 8, mSrcTileSize / 2);
			mRedDigitRegions[n] = mAlphaAtlas.createRegion(
					x, mSrcTileSize, 3 * mSrcTileSize / 8, mSrcTileSize / 2);
		}
		mHourglassRegion = mAlphaAtlas.createRegion(
				7 * mSrcTileSize, 2 * mSrcTileSize,
				mSrcTileSize * 3 / 8, mSrcTileSize / 2);
		for (int n = 0; n < 5; ++n) {
			mClockSprites[n] = rctx.createSprite(mYellowDigitRegions[10],
					3 * K.FRUSTUM_TILE_SIZE / 8, K.FRUSTUM_TILE_SIZE / 2);
		}
		mClockSprites[5] = rctx.createSprite(mHourglassRegion,
				3 * K.FRUSTUM_TILE_SIZE / 8, K.FRUSTUM_TILE_SIZE / 2);
		
		mStar1Region = mAlphaAtlas.createRegion(
				5 * mSrcTileSize, 2 * mSrcTileSize,
				mSrcTileSize / 2, mSrcTileSize / 2);
		mStar2Region = mAlphaAtlas.createRegion(
				6 * mSrcTileSize, 2 * mSrcTileSize,
				mSrcTileSize / 2, mSrcTileSize / 2);
	}

	void deleteAlphaTextures(RenderContext rctx)
	{
		mAlphaSprite = null;
		mStar1Region = null;
		mStar2Region = null;
		for (int n = 0; n < 6; ++n) {
			mClockSprites[n] = null;
		}
		mHourglassRegion = null;
		for (int n = 0; n < 11; ++n) {
			mYellowDigitRegions[n] = null;
			mRedDigitRegions[n] = null;
		}
		mPusherSprite = null;
		for (int n = 0; n < 8; ++n)
			mPusherRegions[n] = null;
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
				2 * K.FRUSTUM_TILE_SIZE, (K.FRUSTUM_TILE_SIZE * 3) / 2,
				16 * K.FRUSTUM_TILE_SIZE, 4 * K.FRUSTUM_TILE_SIZE);
	}

	void deleteLogoAtlas(RenderContext rctx)
	{
		mLogoSprite = null;
		if (null != mLogoAtlas)
			mLogoAtlas.dispose(rctx);
		mLogoAtlas = null;
	}

	void loadControls(RenderContext rctx) {
		Image img = null;
		if (0 != mControlsType)
		{
			rctx.enableBlend(true);
			img = mSys.loadResPNG("vpad");
			mControlsAtlas = rctx.uploadTexture(img, true);
		}
		switch (mControlsType)
		{
		case K.CONTROL_VPAD_LEFT:
		case K.CONTROL_VPAD_RIGHT:
			mVpadWidth = img.getWidth();
			mVpadHeight = img.getHeight();
			mControlsRegions[0] = mControlsAtlas.createRegion(0, 0,
					img.getWidth(), img.getHeight());
			mControlsSprites[0] = rctx.createSprite(mControlsRegions[0],
					0, 0, img.getWidth(), img.getHeight());
			for (int n = 1; n < 4; ++n)
			{
				mControlsSprites[n] = null;
				mControlsRegions[n] = null;
			}
			break;
		}
		if (null != img)
			img.dispose();
	}

	void deleteControls(RenderContext rctx) {
		for (int n = 0; n < 4; ++n)
		{
			mControlsSprites[n] = null;
			mControlsRegions[n] = null;
		}
		if (null != mControlsAtlas)
		{
			mControlsAtlas.dispose(rctx);
			mControlsAtlas = null;
		}
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
		deleteControls(rctx);
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
		if (null != mControlsAtlas)
		{
			deleteControls(rctx);
			loadControls(rctx);
		}
	}

}
