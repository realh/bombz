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
import java.io.InputStreamReader;

import uk.co.realh.hgame.Event;
import uk.co.realh.hgame.Log;
import uk.co.realh.hgame.RenderContext;

/**
 * @author Tony Houghton
 *
 */
public class BombzGameScreen extends BombzScreen {
	
	private static final String TAG = "GameScreen";
	
	private BombzLevel mLevel;
	private RenderContext mRCtx;
	private int mScreenWidth;
	private int mScreenHeight;
	
	private int mCurrentLevel;

	/**
	 * @param mgr
	 * @throws IOException 
	 */
	public BombzGameScreen(BombzGameManager mgr) throws IOException {
		super(mgr);
		mLevel = new BombzLevel(mgr.mTextures);
		mCurrentLevel = mMgr.mSavedGame.get("level", 1);
		loadLevel(mCurrentLevel);
	}
	
	final void loadLevel(int level) throws IOException {
		BufferedReader fd = new BufferedReader(new InputStreamReader(
				mMgr.mSys.openAsset(String.format("levels/%02d", level))));
		mLevel.load(fd);
	}

	@Override
	public void initRendering(RenderContext rctx, int w, int h)
			throws IOException {
		super.initRendering(rctx, w, h);
		mScreenWidth = w;
		mScreenHeight = h;
	}
	
	@Override
	public void resizeRendering(RenderContext rctx, int w, int h)
			throws IOException {
		super.resizeRendering(rctx, w, h);
		mScreenWidth = w;
		mScreenHeight = h;
	}

	@Override
	public void render(RenderContext rctx) {
		Log.d(TAG, "Rendering");
		int vpw = mMgr.mTextures.mViewportWidth;
		int vph = mMgr.mTextures.mViewportHeight;
		rctx.setViewport((mScreenWidth - vpw) / 2,
				(mScreenHeight - vph) / 2, vpw, vph);
		rctx.set2DFrustum(0, K.N_COLUMNS * K.FRUSTUM_TILE_SIZE,
				K.N_ROWS * K.FRUSTUM_TILE_SIZE, 0);
		rctx.bindTexture(mMgr.mTextures.mTileAtlas);
		mLevel.render(rctx);
	}

	/* (non-Javadoc)
	 * @see uk.co.realh.hgame.Screen#handleEvent(uk.co.realh.hgame.Event)
	 */
	@Override
	public boolean handleEvent(Event ev) {
		Log.d(TAG, "Event " + Event.quarkToString(ev.mCode));
		return false;
	}

	/* (non-Javadoc)
	 * @see uk.co.realh.hgame.Screen#step()
	 */
	@Override
	public void step() {
		if (null == mRCtx)
			return;
		if (mLevel.step())
			mRCtx.requestRender();
	}

	/**
	 * @see uk.co.realh.hgame.Renderer#getDescription()
	 */
	@Override
	public String getDescription() {
		return "BombzGameScreen";
	}

	@Override
	public void replacedRenderer(RenderContext rctx) throws IOException {
		super.replacedRenderer(rctx);
		mMgr.mTextures.deleteLogoAtlas(rctx);
		mScreenWidth = rctx.getScreenWidth();
		mScreenHeight = rctx.getScreenHeight();
	}
}
