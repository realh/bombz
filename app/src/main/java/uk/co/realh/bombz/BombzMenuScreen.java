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
import java.util.ArrayList;
import java.util.List;

import uk.co.realh.hgame.Event;
import uk.co.realh.hgame.Font;
import uk.co.realh.hgame.RenderContext;
import uk.co.realh.hgame.Screen;
import uk.co.realh.hgame.SimpleRect;
import uk.co.realh.hgame.TapEventHandler;
import uk.co.realh.hgame.TextWidget;
import uk.co.realh.hgame.TextureAtlas;
import uk.co.realh.hgame.WidgetGroup;

/**
 * @author Tony Houghton
 *
 */
public abstract class BombzMenuScreen extends BombzScreen {
	
	@SuppressWarnings("unused")
	private static final String TAG = "MenuScreen";
	
	protected static final int WIDGET_TOP = 6 * K.FRUSTUM_TILE_SIZE;
	
	protected WidgetGroup mWidgetGroup = new WidgetGroup();
	protected int mWidgetY = WIDGET_TOP;
	protected List<TextWidget> mTextWidgets = new ArrayList<TextWidget>();
	
	protected int mViewportLeft, mViewportTop;
	
	Screen mBackScreen;

	/**
	 * @param mgr
	 */
	public BombzMenuScreen(BombzGameManager mgr) {
		super(mgr);
	}
	
	public void addTextWidget(String text, TapEventHandler handler)
	{
		TextWidget w = new TextWidget(text,
				K.FRUSTUM_TILE_SIZE * K.N_COLUMNS / 2, mWidgetY, 0, -1, 0,
				handler);
		mWidgetGroup.addWidget(w);
		mTextWidgets.add(w);
		mWidgetY += (K.FRUSTUM_TILE_SIZE * 3) / 2;
	}

	@Override
	public void initRendering(RenderContext rctx, int w, int h)
			throws IOException {
		super.initRendering(rctx, w, h);
		mMgr.mTextures.loadTitleLogo(rctx);
		setupRendering(rctx, w, h);
		mWidgetGroup.initRendering(rctx, w, h);
	}
	
	@Override
	public void deleteRendering(RenderContext rctx) {
		mWidgetGroup.deleteRendering(rctx);
		super.deleteRendering(rctx);
	}
	
	private void setupRendering(RenderContext rctx, int w, int h)
			throws IOException
	{
		setupViewport(rctx, w, h);
		setupTileBatcher();
		setupWidgets(rctx, w, h);
	}
	
	private void setupWidgets(RenderContext rctx, int w, int h)
	{
		SimpleRect vp = rctx.getViewport();
		SimpleRect fr = rctx.get2DFrustum();
		int view_ts = (int) ((float) K.FRUSTUM_TILE_SIZE *
				(float) vp.h / (float) fr.h);
		Font font = mMgr.mSys.openFont(view_ts);
		font.setColour(K.MENU_TEXT_COLOUR >> 16,
				(K.MENU_TEXT_COLOUR >> 8) & 0xff,
				K.MENU_TEXT_COLOUR & 0xff);
		for (int n = 0; n < mTextWidgets.size(); ++n)
		{
			TextWidget tw = mTextWidgets.get(n);
			tw.setFont(font);
			tw.setShadowOffset(view_ts / 12);
		}
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
		setupRendering(rctx, w, h);
		mWidgetGroup.resizeRendering(rctx, w, h);
	}
	
	protected void setupViewport(RenderContext rctx, int w, int h)
	{
		int vpw = mMgr.mTextures.mViewportWidth;
		int vph = mMgr.mTextures.mViewportHeight;
		mViewportLeft = (w - vpw) / 2;
		mViewportTop = (h - vph) / 2;
		rctx.setViewport(mViewportLeft, mViewportTop, vpw, vph);
		rctx.set2DFrustum(0, K.N_COLUMNS * K.FRUSTUM_TILE_SIZE,
				K.N_ROWS * K.FRUSTUM_TILE_SIZE, 0);
	}
	
	@Override
	public void render(RenderContext rctx) {
		rctx.enableBlend(false);
		rctx.bindTexture(mMgr.mTextures.mTileAtlas);
		mMgr.mTextures.mTileBatcher.render(rctx);
		rctx.enableBlend(true);
		rctx.bindTexture(mMgr.mTextures.mLogoAtlas);
		mMgr.mTextures.mLogoSprite.render(rctx);
		TextureAtlas atlas = mWidgetGroup.getTextureAtlas();
		if (null != atlas) {
			rctx.bindTexture(atlas);
			mWidgetGroup.render(rctx);
			rctx.unbindTexture(atlas);
		} else {
			rctx.unbindTexture(mMgr.mTextures.mLogoAtlas);
		}
	}
	
	@Override
	public boolean handleEvent(Event ev) {
		if (Event.RESUME == ev.mCode) {
			mRCtx.requestRender();
		}
		else if (Event.TAP == ev.mCode)
		{
			for (int n = 0; n < mTextWidgets.size(); ++n)
			{
				if (mTextWidgets.get(n).handleTapEvent(ev))
					return true;
			}
		}
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
		setupRendering(rctx, rctx.getScreenWidth(), rctx.getScreenHeight());
		mWidgetGroup.replacedRenderer(rctx);
	}

	@Override
	public void replacingRenderer(RenderContext rctx) {
		mWidgetGroup.replacingRenderer(rctx);
		super.replacingRenderer(rctx);
	}

	@Override
	public boolean onBackPressed() {
		if (null == mBackScreen)
			mBackScreen = mMgr.getMasterMenuScreen();
		mMgr.setScreen(mBackScreen);
		return true;
	}
	
	/**
	 * Get X coord in frustum space from tap event.
	 * @param ev
	 * @return	X
	 */
	public int getEventFrustumX(Event ev) {
		return (ev.mDatum1 - mViewportLeft) *
					K.FRUSTUM_TILE_SIZE / mMgr.mTextures.mScrnTileSize;
	}
	
	/**
	 * Get Y coord in frustum space from tap event.
	 * @param ev
	 * @return	X
	 */
	public int getEventFrustumY(Event ev) {
		return (ev.mDatum2 - mViewportTop) *
					K.FRUSTUM_TILE_SIZE / mMgr.mTextures.mScrnTileSize;
	}
	
}
