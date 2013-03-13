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
import uk.co.realh.hgame.SimpleRect;

/**
 * @author Tony Houghton
 *
 */
public class ControlMenuScreen extends BombzMenuScreen {
	
	private static final String TAG = "ControlMenuScreen";
	
	private static final int WIDGET_BOTTOM = 14 * K.FRUSTUM_TILE_SIZE;
	private int mCtrlType;
	
	private SimpleRect[] mBounds = new SimpleRect[4];

	/**
	 * @param mgr
	 */
	public ControlMenuScreen(BombzGameManager mgr) {
		super(mgr);
		mCtrlType = mMgr.mConfiguration.get("touchpad",
				K.CONTROL_VPAD_LEFT);
		int xspacing = (K.FRUSTUM_TILE_SIZE *
				(K.N_COLUMNS - -2 - 2 * K.CTRL_MENU_WIDTH)) / 3;
		for (int n = 0; n < 4; ++n) {
			int x;
			if (0 == n % 2)
				x = xspacing;
			else
				x = xspacing * 2 + K.CTRL_MENU_WIDTH * K.FRUSTUM_TILE_SIZE;
			int y;
			if (0 == n / 2) {
				y = WIDGET_TOP;
			} else {
				y = WIDGET_BOTTOM -
						(K.CTRL_MENU_HEIGHT + 1) * K.FRUSTUM_TILE_SIZE;
			}
			mBounds[n] = new SimpleRect(x, y,
					K.CTRL_MENU_WIDTH * K.FRUSTUM_TILE_SIZE,
					K.CTRL_MENU_HEIGHT * K.FRUSTUM_TILE_SIZE);
		}
	}

	@Override
	public void initRendering(RenderContext rctx, int w, int h)
			throws IOException {
		super.initRendering(rctx, w, h);
		mMgr.mTextures.loadCtrlMenu(rctx);
	}
	
	@Override
	public void render(RenderContext rctx) {
		super.render(rctx);
		BombzTextures t = mMgr.mTextures;
		rctx.bindTexture(t.mCtrlMenuAtlas);
		for (int n = 0; n < 4; ++n) {
			int r;
			if (n + 1 == mCtrlType)
				r = n;
			else
				r = n + 4;
			t.mCtrlMenuSprite.setTexture(t.mCtrlMenuRegions[r]);
			t.mCtrlMenuSprite.setPosition(mBounds[n].x, mBounds[n].y);
			t.mCtrlMenuSprite.render(rctx);
		}
	}

	@Override
	public void replacingRenderer(RenderContext rctx) {
		super.replacingRenderer(rctx);
		mMgr.mTextures.deleteCtrlMenu(rctx);
	}
	
	@Override
	public void replacedRenderer(RenderContext rctx) throws IOException {
		super.replacedRenderer(rctx);
		if (null == mMgr.mTextures.mCtrlMenuAtlas)
			mMgr.mTextures.loadCtrlMenu(rctx);
	}
	
	@Override
	public boolean handleEvent(Event ev) {
		boolean handled = false;
		if (Event.TAP == ev.mCode)
		{
			int x = getEventFrustumX(ev);
			int y = getEventFrustumY(ev);
			for (int n = 0; n < 4; ++n) {
				if (mBounds[n].isPointInRect(x, y)) {
					mCtrlType = n + 1;
					mMgr.mConfiguration.set("touchpad", mCtrlType);
					try {
						mMgr.mConfiguration.save();
					} catch (IOException e) {
						Log.w(TAG, "Unable to save controls configuration");
					}
					handled = true;
					mRCtx.requestRender();
					break;
				}
			}
		}
		if (!handled)
			return super.handleEvent(ev);
		return handled;

	}

	/**
	 * @see uk.co.realh.hgame.Renderer#getDescription()
	 */
	@Override
	public String getDescription() {
		return "ControlsMenu";
	}

}
