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

import uk.co.realh.hgame.RenderContext;
import uk.co.realh.hgame.TextureRegion;
import uk.co.realh.hgame.TileBatcher;

/**
 * @author Tony Houghton
 *
 */
public class TimeLimit {

	private BombzGameManager mMgr;
	private BombzTextures mTextures;
	private int mVpX, mVpY, mVpW, mVpH;
	private int mSeconds;
	private TileBatcher mBatcher;
	private int mScrnTileSize;
	private int[] mDigits = new int[4]; 
	
	public TimeLimit(BombzGameManager mgr) {
		mMgr = mgr;
		mTextures = mgr.mTextures;
	}
	
	public int getViewportWidth() {
		return 6 * mScrnTileSize * K.DIGIT_MUL / K.DIGIT_DIV;
	}
	
	public int getViewportHeight() {
		return mScrnTileSize;
	}
	
	public void setViewport(int x, int y, int w, int h) {
		mVpX = x;
		mVpY = y;
		mVpW = w;
		mVpH = h;
	}
	
	public void setTimeLeft(int secs) {
		mSeconds = secs;
		int mins = secs / 60;
		secs %= 60;
		mDigits[0] = mins / 10;
		mDigits[1] = mins % 10;
		mDigits[2] = secs / 10;
		mDigits[3] = secs % 10;
		// Can't setTextures() yet because might not have called initRendering
	}

	public int getTimeLeft() {
		return mSeconds;
	}
	
	public void tick() {
		--mSeconds;
		if (mSeconds < 0)
			return;
		if (0 == mDigits[3]) {
			mDigits[3] = 9;
			if (0 == mDigits[2]) {
				mDigits[2] = 5;
				if (0 == mDigits[1]) {
					mDigits[1] = 9;
					if (0 != mDigits[0]) {
						--mDigits[0];
					}
				} else {
					--mDigits[1];
				}
			} else {
				--mDigits[2];
			}
		} else {
			--mDigits[3];
		}
		if (mSeconds <= 10) {
			if (0 == (mSeconds % 2))
				mMgr.mAudio.playEffect(mMgr.mPip1Sample, 0.25f);
			else
				mMgr.mAudio.playEffect(mMgr.mPip2Sample, 0.75f);
		}
		setTextures();
	}

	public void initRendering(RenderContext rctx, int w, int h) {
		mBatcher = rctx.createTileBatcher(6, 1,
				K.FRUSTUM_TILE_SIZE * K.DIGIT_MUL / K.DIGIT_DIV,
				K.FRUSTUM_TILE_SIZE);
		mScrnTileSize = (h - (h % K.N_ROWS)) / K.N_ROWS;
		mBatcher.setTextureAt(mTextures.mHourglassRegion, 0, 0);
		setTextures();
	}
	
	public void render(RenderContext rctx) {
		int w = K.FRUSTUM_TILE_SIZE * 6 * K.DIGIT_MUL / K.DIGIT_DIV;
		rctx.setViewport(mVpX, mVpY, mVpW, mVpH);
		rctx.set2DFrustum(0, w, K.FRUSTUM_TILE_SIZE, 0);
		mBatcher.render(rctx);
	}
	
	private void setTextures() {
		TextureRegion[] regs = (mSeconds <= 10) ? mTextures.mRedDigitRegions :
				mTextures.mYellowDigitRegions;
		mBatcher.setTextureAt(regs[mDigits[0]], 1, 0);
		mBatcher.setTextureAt(regs[mDigits[1]], 2, 0);
		mBatcher.setTextureAt(regs[10], 3, 0);
		mBatcher.setTextureAt(regs[mDigits[2]], 4, 0);
		mBatcher.setTextureAt(regs[mDigits[3]], 5, 0);
	}
	
}
