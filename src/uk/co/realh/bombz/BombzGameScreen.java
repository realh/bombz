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

import uk.co.realh.hgame.ButtonFeedback;
import uk.co.realh.hgame.DInput;
import uk.co.realh.hgame.Event;
import uk.co.realh.hgame.Log;
import uk.co.realh.hgame.RenderContext;
import uk.co.realh.hgame.ScreenButtonSource;
import uk.co.realh.hgame.SimpleRect;
import uk.co.realh.hgame.VPad;

/**
 * @author Tony Houghton
 *
 */
public class BombzGameScreen extends BombzScreen implements DInput {
	
	private static final String TAG = "GameScreen";
	
	private BombzLevel mLevel;
	private Pusher mPusher;
	private TimeLimit mTimeLimit;
	private int mScreenWidth;
	private int mScreenHeight;
	
	private int mCurrentLevel;
	
	private SimpleRect mTilesViewport = new SimpleRect();
	private SimpleRect mControlsViewports[] = {
			new SimpleRect(), new SimpleRect()
	};
	private SimpleRect mTilesFrustum = new SimpleRect();
	private SimpleRect mControlsFrustums[] = {
			new SimpleRect(), new SimpleRect()
	};
	
	// Ratio of vpad inner/outer radii
	private final static float VPAD_RATIO = 0.375f;
	private final ScreenButtonSource mButtons;
	private final ButtonFeedback mFeedback;
	private VPad mVPad;
	
	private int mMsSinceTick;	// ms since last TimeLimit update

	/**
	 * @param mgr
	 * @param sbs		ScreenButtonSource
	 * @param feedback	Haptic feedback service for vpad etc, may be null
	 * @throws IOException 
	 */
	public BombzGameScreen(BombzGameManager mgr, ScreenButtonSource sbs,
			ButtonFeedback feedback)
			throws IOException {
		super(mgr);
		mButtons = sbs;
		mFeedback = feedback;
		mLevel = new BombzLevel(mgr.mTextures);
		mPusher = new Pusher(mgr, mLevel, this);
		mTimeLimit = new TimeLimit(mgr.mTextures);
		mCurrentLevel = mMgr.mSavedGame.get("level", 1);
		loadLevel(mCurrentLevel);
	}
	
	final void loadLevel(int level) throws IOException {
		BufferedReader fd = new BufferedReader(new InputStreamReader(
				mMgr.mSys.openAsset(String.format("levels/%02d", level))));
		mLevel.load(fd);
		fd.close();
		mPusher.reset();
		mTimeLimit.setTimeLeft(mLevel.mTimeLimit);
		mMsSinceTick = 0;
	}

	private void setupViewports(int w, int h) {
		mButtons.removeButtons();
		int vpw = mMgr.mTextures.mViewportWidth;
		int vph = mMgr.mTextures.mViewportHeight;
		mTilesFrustum.setRect(0, 0,
				K.N_COLUMNS * K.FRUSTUM_TILE_SIZE,
				K.N_ROWS * K.FRUSTUM_TILE_SIZE);
		switch (mMgr.mTextures.mControlsType) {
		case K.CONTROL_NONE:
			mTilesViewport.setRect((w - vpw) / 2, (h - vph) / 2, vpw, vph);
			setTimeLimitOnLeft();
			break;
		case K.CONTROL_VPAD_LEFT:
			mTilesViewport.setRect(w - vpw, 0, vpw, vph);
			setupVPad(w, h);
			setTimeLimitOnLeft();
			break;
		case K.CONTROL_VPAD_RIGHT:
			mTilesViewport.setRect(0, 0, vpw, vph);
			setupVPad(w, h);
			setTimeLimitOnRight(vpw);
			break;
		case K.CONTROL_VBUTTONS_LEFT:
			mTilesViewport.setRect((w - vpw) * 2 / 3, 0, vpw, vph);
			mVPad = null;
			setTimeLimitOnLeft();
			break;
		case K.CONTROL_VBUTTONS_RIGHT:
			mTilesViewport.setRect((w - vpw) / 3, 0, vpw, vph);
			mVPad = null;
			setTimeLimitOnRight(vpw);
			break;
		}
	}
	
	private void setupVPad(int w, int h) {
		SimpleRect r = mControlsViewports[0];
		if (mMgr.mTextures.mControlsType == K.CONTROL_VPAD_LEFT)
		{
			r.x = (mTilesViewport.x - mMgr.mTextures.mVpadWidth) / 2;
			if (r.x < w / K.CONTROL_XPADDING)
				r.x = K.CONTROL_XPADDING;
		}
		else // VPAD_RIGHT
		{
			r.x = (mTilesViewport.x - mMgr.mTextures.mViewportWidth) / 2;
			r.x = (mMgr.mTextures.mViewportWidth + w -
					mMgr.mTextures.mVpadWidth) / 2;
			if (w - r.x - mMgr.mTextures.mVpadWidth < w / K.CONTROL_XPADDING)
				r.x = w - mMgr.mTextures.mVpadWidth - K.CONTROL_XPADDING;
		}
		r.y = h / 2;
		if (r.y + mMgr.mTextures.mVpadHeight < K.CONTROL_YPADDING)
			r.y = h - mMgr.mTextures.mVpadHeight - h / K.CONTROL_YPADDING;
		r.w = mMgr.mTextures.mVpadWidth;
		r.h = mMgr.mTextures.mVpadHeight;
		mControlsFrustums[0].setRect(0, 0, mMgr.mTextures.mVpadWidth,
				mMgr.mTextures.mVpadHeight);
		if (null == mVPad)
			mVPad = new VPad(mFeedback);
		mVPad.setDimensions(r.x, r.y,
				mMgr.mTextures.mVpadWidth / 2,
				(int) (VPAD_RATIO * mMgr.mTextures.mVpadWidth / 2));
		mButtons.addOnScreenButton(mVPad);
	}
	
	private void setTimeLimitOnLeft() {
		int w = mTimeLimit.getViewportWidth();
		int x = mTilesViewport.x - w;
		if (x < 0)
			x = 0;
		mTimeLimit.setViewport(x, 0, w, mTimeLimit.getViewportHeight());
	}
	
	private void setTimeLimitOnRight(int vpw) {
		int w = mTimeLimit.getViewportWidth();
		int x = mTilesViewport.x + mTilesViewport.w;
		if (x + w > vpw)
			x = vpw - w;
		mTimeLimit.setViewport(x, 0, w, mTimeLimit.getViewportHeight());
	}
	
	@Override
	public void initRendering(RenderContext rctx, int w, int h)
			throws IOException {
		super.initRendering(rctx, w, h);
		mScreenWidth = w;
		mScreenHeight = h;
		mMgr.mTextures.loadAlphaTextures(rctx);
		mMgr.mTextures.loadControls(rctx);
		mTimeLimit.initRendering(rctx,  w,  h);
		setupViewports(w, h);
	}
	
	@Override
	public void resizeRendering(RenderContext rctx, int w, int h)
			throws IOException {
		super.resizeRendering(rctx, w, h);
		mScreenWidth = w;
		mScreenHeight = h;
		setupViewports(w, h);
	}

	@Override
	public void render(RenderContext rctx) {
		rctx.setViewport(mTilesViewport);
		rctx.set2DFrustum(mTilesFrustum);
		rctx.enableBlend(false);
		rctx.bindTexture(mMgr.mTextures.mTileAtlas);
		mLevel.render(rctx);
		rctx.enableBlend(true);
		rctx.bindTexture(mMgr.mTextures.mAlphaAtlas);
		mPusher.render(rctx);
		mTimeLimit.render(rctx);
		if (0 != mMgr.mTextures.mControlsType)
		{
			rctx.bindTexture(mMgr.mTextures.mControlsAtlas);
			rctx.setViewport(mControlsViewports[0]);
			rctx.set2DFrustum(mControlsFrustums[0]);
			mMgr.mTextures.mControlsSprites[0].render(rctx);
		}
	}

	/* (non-Javadoc)
	 * @see uk.co.realh.hgame.Screen#handleEvent(uk.co.realh.hgame.Event)
	 */
	@Override
	public boolean handleEvent(Event ev) {
		// Apparently can't use dynamic constants in switch case
		if (ev.mCode == Event.RESUME) {
			mMgr.enableTicks(K.TICK_INTERVAL);
			if (null != mRCtx)
				mRCtx.requestRender();
		}
		return false;
	}

	/* (non-Javadoc)
	 * @see uk.co.realh.hgame.Screen#step()
	 */
	@Override
	public void step() {
		if (null == mRCtx)
		{
			Log.d(TAG, "Ignoring step because no rctx");
			return;
		}
		boolean update = mLevel.step();
		update = mPusher.step() || update;
		mMsSinceTick += K.TICK_INTERVAL;
		if (mMsSinceTick >=1000)
			mMsSinceTick = 0;
		if (0 == mMsSinceTick) {
			update = true;
			mTimeLimit.tick();
		}
		if (update)
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
	public void replacingRenderer(RenderContext rctx) {
		mMgr.mTextures.deleteControls(rctx);
	}

	@Override
	public void replacedRenderer(RenderContext rctx) throws IOException {
		super.replacedRenderer(rctx);
		mMgr.mTextures.deleteLogoAtlas(rctx);
		mScreenWidth = rctx.getScreenWidth();
		mScreenHeight = rctx.getScreenHeight();
		mMgr.mTextures.loadAlphaTextures(rctx);
		mMgr.mTextures.loadControls(rctx);
		mTimeLimit.initRendering(rctx,
				rctx.getScreenWidth(), rctx.getScreenHeight());
		setupViewports(mScreenWidth, mScreenHeight);
	}

	/**
	 * @see uk.co.realh.hgame.DInput#pollDInput()
	 */
	@Override
	public int pollDInput() {
		if (null != mVPad)
			return mVPad.pollDInput();
		return 0;
	}
}
