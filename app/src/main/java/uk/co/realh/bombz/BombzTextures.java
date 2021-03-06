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
	TextureAtlas mControlsAtlas;
	TextureRegion mControlsRegion;
	Sprite mControlsSprite;
	int mVpadWidth, mVpadHeight;
	
	// 11th "digit" is colon
	TextureRegion[] mYellowDigitRegions = new TextureRegion[11];
	TextureRegion[] mRedDigitRegions = new TextureRegion[11];
	TextureRegion[] mMiniDigitRegions = new TextureRegion[11];
	TextureRegion mStar1Region, mStar2Region;
	TextureRegion mHourglassRegion;
	TextureRegion mMatchRegion;
	TextureRegion mCursorRegion;
	// 4 digits + colon + hourglass
	Sprite mAlphaSprite;	// General purpose
	
	TextureAtlas mCtrlMenuAtlas;
	TextureRegion mCtrlMenuRegions[] = new TextureRegion[8];
	Sprite mCtrlMenuSprite;

	private Sys mSys;
	int mSrcTileSize;		// Source size
	int mScrnTileSize;
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
	BombzTextures(Sys sys)
	{
		mSys = sys;
	}

	void calculateTileSize(RenderContext rctx, int screen_w, int screen_h)
			throws IOException
	{
		mViewportHeight = screen_h - (screen_h % K.N_ROWS);
		mViewportWidth = (mViewportHeight * 4) / 3;
		mScrnTileSize = mViewportHeight / K.N_ROWS;
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
		Log.d(TAG, "Screen tile size " + mScrnTileSize +
				", source tile size " + mSrcTileSize +
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
			mPusherRegions[n] = mAlphaAtlas.createRegion(
						(3 + (n % 4)) * mSrcTileSize,
						mSrcTileSize * (n / 4),
						mSrcTileSize, mSrcTileSize);
		}
		
		mBomb1Region = mAlphaAtlas.createRegion(7 * mSrcTileSize, 0,
						mSrcTileSize, mSrcTileSize);
		mBomb2Region = mAlphaAtlas.createRegion(8 * mSrcTileSize, 0,
						mSrcTileSize, mSrcTileSize);
		mPusherSprite = rctx.createSprite(mPusherRegions[K.FACING_UP],
					K.FRUSTUM_TILE_SIZE, K.FRUSTUM_TILE_SIZE);
		mBombSprite = rctx.createSprite(mBomb1Region,
					K.FRUSTUM_TILE_SIZE, K.FRUSTUM_TILE_SIZE);
		
		int dw = mSrcTileSize * K.DIGIT_MUL / K.DIGIT_DIV;
		int dh = mSrcTileSize;
		for (int n = 0; n < 11; ++n) {
			int x = n * mSrcTileSize * K.DIGIT_MUL / K.DIGIT_DIV;
			mYellowDigitRegions[n] = mAlphaAtlas.createRegion(
					x, 3 * mSrcTileSize, dw, dh);
			mRedDigitRegions[n] = mAlphaAtlas.createRegion(
					x, 4 * mSrcTileSize, dw, dh);
			mMiniDigitRegions[n] = mAlphaAtlas.createRegion(
					3 * mSrcTileSize + n * dw / 2, 2 * mSrcTileSize,
					dw / 2, dh / 2);
		}
		mHourglassRegion = mAlphaAtlas.createRegion(
				8 * mSrcTileSize, 2 * mSrcTileSize,
				dw, mSrcTileSize);
		
		mStar1Region = mAlphaAtlas.createRegion(
				7 * mSrcTileSize, mSrcTileSize, dw / 2, dw / 2);
		mStar2Region = mAlphaAtlas.createRegion(
				8 * mSrcTileSize, mSrcTileSize, dw / 2, dw / 2);
		
		mMatchRegion = mAlphaAtlas.createRegion(9 * mSrcTileSize, 0,
						mSrcTileSize, mSrcTileSize);
		mCursorRegion = mAlphaAtlas.createRegion(9 * mSrcTileSize,
				mSrcTileSize, mSrcTileSize, mSrcTileSize);
	}

	void deleteAlphaTextures(RenderContext rctx)
	{
		mAlphaSprite = null;
		mStar1Region = null;
		mStar2Region = null;
		mHourglassRegion = null;
		mMatchRegion = null;
		mCursorRegion = null;
		for (int n = 0; n < 11; ++n) {
			mMiniDigitRegions[n] = null;
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

	void loadControls(RenderContext rctx) throws IOException {
		rctx.enableBlend(true);
		mVpadWidth = (int) ((float) mSys.getDisplayDPI() / K.DPI_TO_VPAD);
		mVpadHeight = mVpadWidth;
		Log.d(TAG, "Vpad size " + mVpadWidth);
		mControlsAtlas = loadAtlas(rctx, "vpad", true);
		mControlsRegion = mControlsAtlas.createRegion(0, 0,
				mSrcTileSize * 16 / 3, mSrcTileSize * 16 / 3);
		mControlsSprite = rctx.createSprite(mControlsRegion,
				0, 0, mVpadWidth, mVpadHeight);
	}

	void deleteControls(RenderContext rctx) {
		mControlsSprite = null;
		mControlsRegion = null;
		if (null != mControlsAtlas)
		{
			mControlsAtlas.dispose(rctx);
			mControlsAtlas = null;
		}
	}
	
	void loadCtrlMenu(RenderContext rctx) throws IOException
	{
		rctx.enableBlend(true);
		mCtrlMenuAtlas = loadAtlas(rctx, "vpad_menu", true);
		for (int n = 0; n < 8; ++n)
		{
			mCtrlMenuRegions[n] = mCtrlMenuAtlas.createRegion(
					(n % 4) * K.CTRL_MENU_WIDTH * mSrcTileSize,
					(n / 4) * K.CTRL_MENU_HEIGHT * mSrcTileSize,
					K.CTRL_MENU_WIDTH * mSrcTileSize,
					K.CTRL_MENU_HEIGHT * mSrcTileSize);
		}
		mCtrlMenuSprite = rctx.createSprite(mCtrlMenuRegions[0],
				K.CTRL_MENU_WIDTH * K.FRUSTUM_TILE_SIZE,
				K.CTRL_MENU_HEIGHT * K.FRUSTUM_TILE_SIZE);
	}

	void deleteCtrlMenu(RenderContext rctx)
	{
		mCtrlMenuSprite = null;
		for (int n = 0; n < 4; ++n)
		{
			mCtrlMenuRegions[n] = null;
		}
		if (null != mCtrlMenuAtlas)
			mCtrlMenuAtlas.dispose(rctx);
		mCtrlMenuAtlas = null;
	}

	/**
	 * Call in a deleteRendering handler and at the start of initRendering.
	 *
	 * @see uk.co.realh.hgame.Renderer
	 */
	void deleteAllTextures(RenderContext rctx)
	{
		deleteCtrlMenu(rctx);
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
		if (null != mCtrlMenuAtlas)
		{
			deleteCtrlMenu(rctx);
			loadCtrlMenu(rctx);
		}
	}

}
