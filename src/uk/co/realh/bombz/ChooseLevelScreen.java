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

import uk.co.realh.hgame.RenderContext;
import uk.co.realh.hgame.Sprite;

/**
 * @author Tony Houghton
 *
 */
public class ChooseLevelScreen extends BombzMenuScreen {

	@SuppressWarnings("unused")
	private static final String TAG = "ChooseLevel";
	
	private static final int COLUMNS = 10;
	private static final int ROWS = 5;
	private static final int TOP = WIDGET_TOP - K.FRUSTUM_TILE_SIZE / 2;
	private static final int BOTTOM = 14 * K.FRUSTUM_TILE_SIZE;
	
	/**
	 * @param mgr
	 */
	public ChooseLevelScreen(BombzGameManager mgr) {
		super(mgr);
		mWidgetY = BOTTOM;
		/*
		addTextWidget(mMgr.mSys.translate("Back"),
				new BackTappedListener());
		*/
	}
	
	@Override
	public void initRendering(RenderContext rctx, int w, int h)
			throws IOException {
		super.initRendering(rctx, w, h);
		setupRendering(rctx);
	}
	
	private void setupRendering(RenderContext rctx) throws IOException {
		BombzTextures t = mMgr.mTextures;
		t.loadAlphaTextures(rctx);
		t.mAlphaSprite = rctx.createSprite(t.mBomb1Region,
					K.FRUSTUM_TILE_SIZE, K.FRUSTUM_TILE_SIZE);
	}
	
	@Override
	public void render(RenderContext rctx ) {
		super.render(rctx);
		int bestLevel = mMgr.mSavedGame.get("highest_completed", 0);
		BombzTextures t = mMgr.mTextures;
		Sprite s = t.mAlphaSprite;
		rctx.bindTexture(t.mAlphaAtlas);
		int d1 = 1;
		int d10 = 0;
		for (int n = 0; n < K.N_LEVELS; ++n) {
			int x = (n % COLUMNS) * 16 * K.FRUSTUM_TILE_SIZE / COLUMNS +
					9 * K.FRUSTUM_TILE_SIZE / 4;
			int y = (n / COLUMNS) * (BOTTOM - TOP) / ROWS + TOP;
			s.setTexture((n > bestLevel) ? t.mBomb1Region : t.mBomb2Region);
			s.setPositionAndSize(x, y,
					K.FRUSTUM_TILE_SIZE, K.FRUSTUM_TILE_SIZE);
			s.render(rctx);
			int y0 = y + 3 * K.FRUSTUM_TILE_SIZE / 4;
			s.setTexture(t.mMiniDigitRegions[d10]);
			s.setPositionAndSize(x + K.FRUSTUM_TILE_SIZE / 8, y0,
					K.FRUSTUM_TILE_SIZE * K.DIGIT_MUL / K.DIGIT_DIV / 2,
					K.FRUSTUM_TILE_SIZE / 2);
			s.render(rctx);
			s.setTexture(t.mMiniDigitRegions[d1]);
			s.setPosition(x + K.FRUSTUM_TILE_SIZE / 2, y0);
			s.render(rctx);
			int score = mMgr.mSavedGame.get("score_" + (n + 1), 0);
			if (score != 0 || (n + 1) == bestLevel) {
				y0 = y + K.FRUSTUM_TILE_SIZE / 4;
				s.setTexture(score > 0 ? t.mStar2Region : t.mStar1Region);
				s.setPositionAndSize(x, y0,
						3 * K.FRUSTUM_TILE_SIZE / 8,
						3 * K.FRUSTUM_TILE_SIZE / 8);
				s.render(rctx);
				if (score == 1)
					s.setTexture(t.mStar1Region);
				s.setPosition(x + 6 * K.FRUSTUM_TILE_SIZE / 16,
						y0 - K.FRUSTUM_TILE_SIZE / 8);
				s.render(rctx);
				if (score == 2)
					s.setTexture(t.mStar1Region);
				s.setPosition(x + 12 * K.FRUSTUM_TILE_SIZE / 16, y0);
				s.render(rctx);
			}
			if (++d1 == 10)
			{
				d1 = 0;
				++d10;
			}
		}
	}

	@Override
	public void replacedRenderer(RenderContext rctx) throws IOException {
		super.replacedRenderer(rctx);
		if (null == mMgr.mTextures.mAlphaAtlas)
			setupRendering(rctx);
	}
	
	/*
	private class BackTappedListener implements TapEventHandler {
		@Override
		public boolean handleTapEvent(Event e) {
			Log.d(TAG, "Back tapped");
			mMgr.setScreen(mMgr.getMainMenuScreen());
			return true;
		}
	}
	*/

	/**
	 * @see uk.co.realh.hgame.Renderer#getDescription()
	 */
	@Override
	public String getDescription() {
		return "ChooseLevelScreen";
	}

}
