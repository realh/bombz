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
import uk.co.realh.hgame.Sys;
import uk.co.realh.hgame.TextureAtlas;
import uk.co.realh.hgame.TextureRegion;

/**
 * @author Tony Houghton
 * TODO: Create sprites etc.
 */
public class BombzTextures {
	
	private static String TAG = "Textures";
	
	TextureAtlas mTileAtlas, mAlphaAtlas, mLogoAtlas;
	TextureRegion[] mRegions = new TextureRegion[BombzCell.N_CELLS];
	private Sys mSys;
	private int mTileSize;		// Source size
	
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
	BombzTextures(Sys sys, int screen_w, int screen_h) throws IOException
	{
		mSys = sys;
		calculateTileSize(screen_w, screen_h);
	}
	
	private void calculateTileSize(int screen_w, int screen_h)
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
		mTileSize = best_size;
		Log.d(TAG, "Using source tile size " + mTileSize);
	}
	
	private TextureAtlas loadAtlas(RenderContext rctx,
			String name, boolean alpha) throws IOException
	{
		InputStream fd = mSys.openAsset(
				"pngs/" + mTileSize + "/" + name + ".png");
		Image img = mSys.loadPNG(fd, "name");
		fd.close();
		TextureAtlas atlas = rctx.uploadTexture(img, true);
		img.dispose();
		return atlas;
	}
	
	void loadTiles(RenderContext rctx) throws IOException
	{
		if (null == mTileAtlas)
			mTileAtlas = loadAtlas(rctx, "tile_atlas", false);
	}
	
	void deleteTiles(RenderContext rctx)
	{
		if (null != mTileAtlas)
		{
			mTileAtlas.dispose();
			mTileAtlas = null;
		}
	}
	
	void loadAlphaTextures(RenderContext rctx) throws IOException
	{
		if (null == mAlphaAtlas)
			mAlphaAtlas = loadAtlas(rctx, "alpha_atlas", true);
	}
	
	void deleteAlphaTextures(RenderContext rctx)
	{
		if (null != mTileAtlas)
		{
			mAlphaAtlas.dispose();
			mAlphaAtlas = null;
		}
	}
	
	void loadTitleLogo(RenderContext rctx) throws IOException
	{
		if (null == mLogoAtlas)
			mLogoAtlas = loadAtlas(rctx, "title_logo", true);
	}
	
	void deleteLogoAtlas(RenderContext rctx)
	{
		if (null != mTileAtlas)
		{
			mLogoAtlas.dispose();
			mLogoAtlas = null;
		}
	}
	
	/**
	 * Call in a deleteRendering handler and at the start of initRendering.
	 * 
	 * @see uk.co.realh.hgame.Renderer
	 */
	void deleteAllTextures()
	{
		mTileAtlas = null;
		mAlphaAtlas = null;
		mLogoAtlas = null;
	}
	
	/**
	 * Call in a deleteRendering handler and at the start of initRendering.
	 * @throws IOException 
	 * 
	 * @see uk.co.realh.hgame.Renderer
	 */
	void onResize(RenderContext rctx) throws IOException
	{
		if (null != mTileAtlas)
		{
			mTileAtlas.dispose();
			mTileAtlas = null;
			loadTiles(rctx);
		}
		if (null != mAlphaAtlas)
		{
			mAlphaAtlas.dispose();
			mAlphaAtlas = null;
			loadAlphaTextures(rctx);
		}
		if (null != mLogoAtlas)
		{
			mLogoAtlas.dispose();
			mLogoAtlas = null;
			loadTitleLogo(rctx);
		}
	}
	
}
