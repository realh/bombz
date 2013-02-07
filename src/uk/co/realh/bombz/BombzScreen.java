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

import uk.co.realh.hgame.Log;
import uk.co.realh.hgame.RenderContext;
import uk.co.realh.hgame.Screen;

/**
 * @author Tony Houghton
 *
 */
public abstract class BombzScreen implements Screen {
	
	private static final String TAG = "BombzScreen";
	
	protected BombzGameManager mMgr;

	public BombzScreen(BombzGameManager mgr) {
		mMgr = mgr;
	}

	@Override
	public void initRendering(RenderContext rctx, int w, int h)
			throws IOException {
		rctx.enable2DTextures(true);
		mMgr.mTextures.calculateTileSize(rctx, w, h);
		// All Bombz screens need the tiles
		mMgr.mTextures.deleteTiles(rctx);
		mMgr.mTextures.loadTiles(rctx);
	}
	
	@Override
	public void deleteRendering(RenderContext rctx) {
		mMgr.mTextures.deleteAllTextures(rctx);
	}

	@Override
	public void resizeRendering(RenderContext rctx, int w, int h)
			throws IOException {
		mMgr.mTextures.onResize(rctx, w, h);
	}

	/**
	 * No-op because we don't know which textures to delete without
	 * knowing what next Screen will be.
	 */
	@Override
	public void replacingRenderer(RenderContext rctx) {
		Log.d(TAG, "Replacing renderer " + getDescription());
	}

	@Override
	public void replacedRenderer(RenderContext rctx) throws IOException {
		Log.d(TAG, "New renderer " + getDescription());
		if (null == mMgr.mTextures.mTileAtlas)
			mMgr.mTextures.loadTiles(rctx);
	}

}
