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
import uk.co.realh.hgame.Sprite;
import uk.co.realh.hgame.VButtons;
import uk.co.realh.hgame.VPad;

/**
 * @author Tony Houghton
 *
 */
public class BombzGameScreen extends BombzScreen implements DInput {
	
	private static final String TAG = "GameScreen";
	
	BombzLevel mLevel;
	private Pusher mPusher;
	TimeLimit mTimeLimit;
	private int mScreenWidth;
	private int mScreenHeight;
	
	private int mCtrlType;
	
	private SimpleRect mTilesViewport = new SimpleRect();
	private SimpleRect mControlsViewports[] = {
			new SimpleRect(), new SimpleRect(),
			new SimpleRect(), new SimpleRect()
	};
	private SimpleRect mTilesFrustum = new SimpleRect();
	private SimpleRect mControlsFrustum = new SimpleRect();
	
	private SimpleRect mMatchViewport = new SimpleRect();
	private SimpleRect mMatchFrustum = new SimpleRect();
	
	// Ratio of vpad inner/outer radii
	private final static float VPAD_RATIO = 0.375f;
	private final ScreenButtonSource mButtons;
	private final ButtonFeedback mFeedback;
	private VPad mVPad;
	private VButtons mVButtons;
	
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
		mLevel = new BombzLevel(mgr);
		mPusher = new Pusher(mgr, mLevel, this);
		mTimeLimit = new TimeLimit(mgr);
		loadCurrentLevel();
	}
	
	public final void loadLevel(int level) throws IOException {
		BufferedReader fd = new BufferedReader(new InputStreamReader(
				mMgr.mSys.openAsset(String.format("levels/%02d", level))));
		mLevel.load(fd);
		fd.close();
		mPusher.reset();
		mMgr.mStats.startAttempt(level,
				mLevel.countDetonators(), mLevel.mTimeLimit);
		mTimeLimit.setTimeLeft(mLevel.mTimeLimit);
		mMsSinceTick = 0;
	}
	
	public final void loadCurrentLevel() throws IOException {
		loadLevel(mMgr.mCurrentLevel);
	}

	private void setupViewports(int w, int h) {
		mButtons.removeButtons();
		int vpw = mMgr.mTextures.mViewportWidth;
		int vph = mMgr.mTextures.mViewportHeight;
		mTilesFrustum.setRect(0, 0,
				K.N_COLUMNS * K.FRUSTUM_TILE_SIZE,
				K.N_ROWS * K.FRUSTUM_TILE_SIZE);
		mCtrlType = mMgr.mConfiguration.get("touchpad",
				K.CONTROL_VPAD_LEFT);
		switch (mCtrlType) {
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
			setupVButtons(0, mTilesViewport.x,
					mTilesViewport.x + vpw, w,
					w, h);
			setTimeLimitOnLeft();
			break;
		case K.CONTROL_VBUTTONS_RIGHT:
			mTilesViewport.setRect((w - vpw) / 3, 0, vpw, vph);
			setupVButtons(mTilesViewport.x + vpw, w,
					0, mTilesViewport.x,
					w, h);
			setTimeLimitOnRight(vpw);
			break;
		}
		mMatchFrustum.setRect(0, 0, K.FRUSTUM_TILE_SIZE, K.FRUSTUM_TILE_SIZE);
	}
	
	private void setupVPad(int w, int h) {
		mVButtons = null;
		SimpleRect r = mControlsViewports[0];
		BombzTextures t = mMgr.mTextures;
		if (mCtrlType == K.CONTROL_VPAD_LEFT)
		{
			r.x = (mTilesViewport.x - t.mVpadWidth) / 2;
			if (r.x < w / K.CONTROL_XPADDING)
				r.x = w / K.CONTROL_XPADDING;
		}
		else // VPAD_RIGHT
		{
			r.x = (mTilesViewport.x - t.mViewportWidth) / 2;
			r.x = (t.mViewportWidth + w -
					t.mVpadWidth) / 2;
			if (w - r.x - t.mVpadWidth < w / K.CONTROL_XPADDING)
				r.x = w - t.mVpadWidth - w / K.CONTROL_XPADDING;
		}
		r.y = h / 2;
		if (r.y + t.mVpadHeight < h / K.CONTROL_YPADDING)
			r.y = h - t.mVpadHeight - h / K.CONTROL_YPADDING;
		r.w = t.mVpadWidth;
		r.h = t.mVpadHeight;
		t.mControlsSprite.setSize(r.w, r.h);
		mControlsFrustum.setRect(0, 0, t.mVpadWidth, t.mVpadHeight);
		if (null == mVPad)
			mVPad = new VPad(mFeedback);
		mVPad.setDimensions(r.x, r.y,
				t.mVpadWidth / 2,
				(int) (VPAD_RATIO * t.mVpadWidth / 2));
		mButtons.addOnScreenButton(mVPad);
		Log.d(TAG, String.format("Vpad type %d, rect %s",
				mCtrlType, r.toString()));
	}
	
	/**
	 * Set up controls with 4 separate buttons.
	 * 
	 * @param hL	Left bound of horizontals
	 * @param hR	Right bound of horizontals
	 * @param vL	Left bound of verticals
	 * @param vR	Right bound of verticals
	 * @param w		Screen width
	 * @param h		Screen height
	 */
	private void setupVButtons(int hL, int hR, int vL, int vR, int w, int h) {
		mVPad = null;
		BombzTextures t = mMgr.mTextures;
		int bw = t.mVpadWidth / 2;
		int hw = t.mVpadWidth;
		hL = (hR + hL - hw) / 2;
		if (hL < w / K.CONTROL_XPADDING)
			hL = w / K.CONTROL_XPADDING;
		else if (hL + hw > w - w / K.CONTROL_XPADDING)
			hL = w - w / K.CONTROL_XPADDING - hw;
		hR = hL + hw;
		int vw = t.mVpadWidth * 5 / 8;
		vL = (vR + vL - vw) / 2;
		if (vL < w / K.CONTROL_XPADDING)
			vL = w / K.CONTROL_XPADDING;
		else if (vL + vw > w - w / K.CONTROL_XPADDING)
			vL = w - w / K.CONTROL_XPADDING - vw;
		vR = vL + vw;
		int vh = t.mVpadWidth;
		int vT = h / 2;
		if (vT + vh > h - h / K.CONTROL_YPADDING)
			vT = h - h / K.CONTROL_YPADDING - vh;
		int vB = vT + vh;
		int hh = t.mVpadWidth / 2;
		int hT = vB;
		if (hT + hh > h - h / K.CONTROL_YPADDING)
			hT = h - h / K.CONTROL_YPADDING - hh;
		if (null == mVButtons)
			mVButtons = new VButtons(mFeedback);
		mVButtons.setLeftBounds(hL, hT, bw, bw);
		mVButtons.setRightBounds(hL + bw, hT, bw, bw);
		int ul, dl;
		if (hL < vL) {
			ul = vL + bw / 4;
			dl = vL;
		} else {
			ul = vL;
			dl = vL + bw / 4;
		}
		mVButtons.setUpBounds(ul, vT, bw, bw);
		mVButtons.setDownBounds(dl, vT + bw, bw, bw);
		mButtons.addOnScreenButton(mVButtons);
		mControlsFrustum.setRect(0, 0, bw, bw);
		mMgr.mTextures.mControlsSprite.setSize(bw, bw);
		mControlsViewports[0].setRect(hL, hT, bw, bw);
		mControlsViewports[1].setRect(hL + bw, hT, bw, bw);
		mControlsViewports[2].setRect(ul, vT, bw, bw);
		mControlsViewports[3].setRect(dl, vT + bw, bw, bw);
	}
	
	private void setTimeLimitOnLeft() {
		int s = mMgr.mTextures.mSrcTileSize;
		int w = mTimeLimit.getViewportWidth();
		int x = mTilesViewport.x - w;
		int h = mTimeLimit.getViewportHeight();
		Log.d(TAG, "TimeLimit viewport w " + w + " x " + x);
		if (x < 0)
			x = 0;
		mTimeLimit.setViewport(x, s / 4, w, h);
		x += (w - s) / 2;
		if (x + s > mTilesViewport.x)
			x = mTilesViewport.x - s;
		if (x < 0)
			x = 0;
		mMatchViewport.setRect(x, h + s / 4, s, s);
	}
	
	private void setTimeLimitOnRight(int vpw) {
		int s = mMgr.mTextures.mSrcTileSize;
		int w = mTimeLimit.getViewportWidth();
		int x = mTilesViewport.x + mTilesViewport.w;
		int x0 = x;
		int h = mTimeLimit.getViewportHeight();
		if (x + w > vpw)
			x = vpw - w;
		mTimeLimit.setViewport(x, mMgr.mTextures.mSrcTileSize / 4,
				w, mTimeLimit.getViewportHeight());
		x += (w - s) / 2;
		if (x < x0)
			x = x0;
		if (x + s > vpw)
			x = vpw - s;
		mMatchViewport.setRect(x, h + s / 4, s, s);
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
		Log.d(TAG, "initRendering(" + w + ", " + h + ")");
		setupViewports(w, h);
		mVPad.reset();
	}
	
	@Override
	public void resizeRendering(RenderContext rctx, int w, int h)
			throws IOException {
		super.resizeRendering(rctx, w, h);
		mScreenWidth = w;
		mScreenHeight = h;
		Log.d(TAG, "resizeRendering(" + w + ", " + h + ")");
		mTimeLimit.initRendering(rctx, w, h);
		setupViewports(w, h);
		mVPad.reset();
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
		mLevel.renderExplo(rctx);
		mTimeLimit.render(rctx);
		if (mPusher.mHaveMatch) {
			Sprite spr;
			rctx.setViewport(mMatchViewport);
			rctx.set2DFrustum(mMatchFrustum);
			if (null == mMgr.mTextures.mAlphaSprite) {
				spr = mMgr.mTextures.mAlphaSprite = rctx.createSprite(
						mMgr.mTextures.mMatchRegion,
						0, 0, K.FRUSTUM_TILE_SIZE, K.FRUSTUM_TILE_SIZE);
			} else {
				spr = mMgr.mTextures.mAlphaSprite;
				spr.setTexture(mMgr.mTextures.mMatchRegion);
				spr.setPositionAndSize(0, 0,
						K.FRUSTUM_TILE_SIZE, K.FRUSTUM_TILE_SIZE);
			}
			spr.render(rctx);
		}
		switch (mCtrlType) {
		case K.CONTROL_VPAD_LEFT:
		case K.CONTROL_VPAD_RIGHT:
			rctx.bindTexture(mMgr.mTextures.mControlsAtlas);
			rctx.setViewport(mControlsViewports[0]);
			rctx.set2DFrustum(mControlsFrustum);
			mMgr.mTextures.mControlsSprite.render(rctx);
			break;
		case K.CONTROL_VBUTTONS_LEFT:
		case K.CONTROL_VBUTTONS_RIGHT:
			rctx.bindTexture(mMgr.mTextures.mControlsAtlas);
			rctx.set2DFrustum(mControlsFrustum);
			for (int n = 0; n < 4; ++n) {
				rctx.setViewport(mControlsViewports[n]);
				mMgr.mTextures.mControlsSprite.render(rctx);
			}
			break;
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
			mMgr.setScreenBlankTimeout(K.EXTENDED_IDLE);
			if (null != mRCtx)
				mRCtx.requestRender();
		}
		else if (ev.mCode == Event.PAUSE) {
			mMgr.disableTicks();
			mMgr.setScreenBlankTimeout(0);
			mMgr.setScreen(mMgr.getPauseScreen());
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
		mButtons.removeButtons();
		if (null != mVPad)
			mVPad.reset();
		if (null != mVButtons)
			mVButtons.reset();
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
		Log.d(TAG, "replacedRenderer(" +
				mScreenWidth + ", " + mScreenHeight + ")");
		setupViewports(mScreenWidth, mScreenHeight);
		if (null != mVPad)
			mVPad.reset();
		if (null != mVButtons)
			mVButtons.reset();
	}

	/**
	 * @see uk.co.realh.hgame.DInput#pollDInput()
	 */
	@Override
	public int pollDInput() {
		int d = 0;
		if (null != mVPad)
			d = mVPad.pollDInput();
		if (null != mVButtons)
			d |= mVButtons.pollDInput();
		return d;
	}
	
	@Override
	public boolean onBackPressed() {
		BombzPauseScreen s = mMgr.getPauseScreen();
		mMgr.setScreen(s);
		s.mBackScreen = null;
		return true;
	}
	
}
