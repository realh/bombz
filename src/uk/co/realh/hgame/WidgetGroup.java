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

package uk.co.realh.hgame;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * @author Tony Houghton
 *
 */
public class WidgetGroup implements Renderer, TapEventHandler {

	@SuppressWarnings("unused")
	private static final String TAG = "WidgetGroup";
	
	private List<Widget> mWidgets = new ArrayList<Widget>();
	private AtlasMaker mAtlasMaker;

	public TextureAtlas getTextureAtlas()
	{
		return mAtlasMaker.mAtlas;
	}

	public void addWidget(Widget w)
	{
		mWidgets.add(w);
	}

	@Override
	public boolean handleTapEvent(Event e)
	{
		for (int n = 0; n < mWidgets.size(); ++n)
		{
			if (mWidgets.get(n).handleTapEvent(e))
				return true;
		}
		return false;
	}

	/**
	 * @see uk.co.realh.hgame.Renderer#initRendering(uk.co.realh.hgame.RenderContext, int, int)
	 */
	@Override
	public void initRendering(RenderContext rctx, int w, int h)
			throws IOException {
		List<Image> imgs = new ArrayList<Image>();
		for (int n = 0; n < mWidgets.size(); ++n)
		{
			imgs.add(mWidgets.get(n).getImage());
		}
		mAtlasMaker = new AtlasMaker(rctx, imgs);
		float scale_x = rctx.getScaleFactorX();
		float scale_y = rctx.getScaleFactorY();
		for (int n = 0; n < mWidgets.size(); ++n)
		{
			mWidgets.get(n).initRendering(rctx,
					mAtlasMaker.mRegions.get(n),
					(int) ((float) imgs.get(n).getWidth() / scale_x),
					(int) ((float) imgs.get(n).getHeight() / scale_y));
			imgs.get(n).dispose();
		}
	}

	/**
	 * @see uk.co.realh.hgame.Renderer#deleteRendering(uk.co.realh.hgame.RenderContext)
	 */
	@Override
	public void deleteRendering(RenderContext rctx)
	{
		mWidgets = null;
		if (null != mAtlasMaker)
		{
			mAtlasMaker.mAtlas.dispose(rctx);
			mAtlasMaker = null;
		}
	}

	/**
	 * @see uk.co.realh.hgame.Renderer#resizeRendering(uk.co.realh.hgame.RenderContext, int, int)
	 */
	@Override
	public void resizeRendering(RenderContext rctx, int w, int h)
			throws IOException {
		if (null != mAtlasMaker)
		{
			mAtlasMaker.mAtlas.dispose(rctx);
			mAtlasMaker = null;
		}
		initRendering(rctx, w, h);
	}

	/**
	 * @see uk.co.realh.hgame.Renderer#render(uk.co.realh.hgame.RenderContext)
	 */
	@Override
	public void render(RenderContext rctx) {
		rctx.bindTexture(mAtlasMaker.mAtlas);
		for (int n = 0; n < mWidgets.size(); ++n)
			mWidgets.get(n).render(rctx);
	}

	/**
	 * @see uk.co.realh.hgame.Renderer#replacingRenderer(uk.co.realh.hgame.RenderContext)
	 */
	@Override
	public void replacingRenderer(RenderContext rctx) {
		deleteRendering(rctx);
	}

	/**
	 * @see uk.co.realh.hgame.Renderer#replacedRenderer(uk.co.realh.hgame.RenderContext)
	 */
	@Override
	public void replacedRenderer(RenderContext rctx) throws IOException {
		if (null == mAtlasMaker) {
			initRendering(rctx, rctx.getScreenWidth(), rctx.getScreenHeight());
		}
	}

	/**
	 * @see uk.co.realh.hgame.Renderer#getDescription()
	 */
	@Override
	public String getDescription() {
		return "WidgetGroup";
	}

}
