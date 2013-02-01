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

import uk.co.realh.hgame.Event;
import uk.co.realh.hgame.Log;
import uk.co.realh.hgame.RenderContext;
import uk.co.realh.hgame.TextureRegion;

/**
 * @author Tony Houghton
 *
 */
public abstract class BombzMenuScreen extends BombzScreen {
	
	private static final String TAG = "MenuScreen";

	/**
	 * @param mgr
	 */
	public BombzMenuScreen(BombzGameManager mgr) {
		super(mgr);
	}

	@Override
	public void initRendering(RenderContext rctx, int w, int h)
			throws IOException {
		super.initRendering(rctx, w, h);
		mMgr.mTextures.loadTitleLogo(rctx);
		setupViewport(rctx, w, h);
		setupTileBatcher();
		rctx.requestRender();
	}
	
	private void setupTileBatcher()
	{
		//int n = 0;
		for (int y = 0; y < K.N_ROWS; ++y) {
			for (int x = 0; x < K.N_COLUMNS; ++x) {
				int c;
				if (y == 0)
				{
					if (x == 0)
						c = Cell.CHROME00;
					else if (x == K.N_COLUMNS - 1)
						c = Cell.CHROME00 + 2;
					else
						c = Cell.CHROME00 + 1;
				}
				else if (y == K.N_ROWS - 1)
				{
					if (x == 0)
						c = Cell.CHROME00 + 4;
					else if (x == K.N_COLUMNS - 1)
						c = Cell.CHROME00 + 5;
					else
						c = Cell.CHROME00 + 1;
				}
				else if (x == 0 || x == K.N_COLUMNS - 1)
				{
					c = Cell.CHROME00 + 3;
				}
				/**
				else if (n < Cell.N_CELLS)
				{
					c = Cell.BLANK + (n++);
				}
				*/
				else
				{
					c = Cell.BLANK;
				}
				mMgr.mTextures.mTileBatcher.setTextureAt(
						mMgr.mTextures.mTileRegions[c - Cell.OFFSET], x, y);
			}
		}
	}
	
	@Override
	public void resizeRendering(RenderContext rctx, int w, int h)
			throws IOException {
		super.resizeRendering(rctx, w, h);
		setupViewport(rctx, w, h);
		setupTileBatcher();
		rctx.requestRender();
	}
	
	protected void setupViewport(RenderContext rctx, int w, int h)
	{
		int vpw = mMgr.mTextures.mViewportWidth;
		int vph = mMgr.mTextures.mViewportHeight;
		rctx.setViewport((w - vpw) / 2, (h - vph) / 2, vpw, vph);
		rctx.set2DFrustum(0, K.N_COLUMNS * K.FRUSTUM_TILE_SIZE,
				0, K.N_ROWS * K.FRUSTUM_TILE_SIZE);
	}
	
	@Override
	public void render(RenderContext rctx) {
		Log.d(TAG, "Rendering MenuScreen");
		rctx.enableBlend(false);
		rctx.bindTexture(mMgr.mTextures.mTileAtlas);
		mMgr.mTextures.mTileBatcher.render(rctx);
		rctx.enableBlend(true);
		rctx.bindTexture(mMgr.mTextures.mLogoAtlas);
		mMgr.mTextures.mLogoSprite.render(rctx);
	}
	
	@Override
	public boolean handleEvent(Event ev) {
		return false;
	}

	@Override
	public void step() {
	}

	@Override
	public void replacedRenderer(RenderContext rctx) throws IOException {
		super.replacedRenderer(rctx);
		if (null == mMgr.mTextures.mLogoAtlas)
			mMgr.mTextures.loadTitleLogo(rctx);
	}

}
