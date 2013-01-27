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
import uk.co.realh.hgame.RenderContext;

/**
 * @author Tony Houghton
 *
 */
public abstract class BombzMenuScreen extends BombzScreen {

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
	}
	
	@Override
	public void resizeRendering(RenderContext rctx, int w, int h)
			throws IOException {
		super.resizeRendering(rctx, w, h);
		setupViewport(rctx, w, h);
	}
	
	protected void setupViewport(RenderContext rctx, int w, int h)
	{
		int batch_w = K.N_COLUMNS * mMgr.mTextures.mSrcTileSize;
		int batch_h = K.N_ROWS * mMgr.mTextures.mSrcTileSize;
		rctx.setViewport((w - batch_w) / 2, 0, batch_w, batch_h);
		rctx.set2DFrustum(0, K.N_COLUMNS * K.FRUSTUM_TILE_SIZE,
				K.N_ROWS * K.FRUSTUM_TILE_SIZE, 0);
	}
	
	@Override
	public void render(RenderContext rctx) {
		rctx.enableBlend(false);
		mMgr.mTextures.mTileBatcher.render(rctx);
		rctx.enableBlend(true);
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
