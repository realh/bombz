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

/**
 * An on-screen object which responds to tap events.
 * 
 * @author Tony Houghton
 *
 */
public abstract class Widget implements TapEventHandler {
	
	private TapEventHandler mHandler;
	private Sprite mSprite;
	private int mX0, mY0, mX1, mY1;
	private final int mRefX, mRefY, mXAlign, mYAlign;
	
	/**
	 * Params are used for dynamic positioning.
	 * 
	 * @param x			Position in frustum
	 * @param y			Position in frustum
	 * @param xalign	<0 = left, 0 = centre, >0 = right
	 * @param yalign	<0 = top, 0 = centre, >0 = bottom
	 */
	public Widget(int x, int y, int xalign, int yalign)
	{
		mRefX = x;
		mRefY = x;
		mXAlign = xalign;
		mYAlign = yalign;
	}

	public Widget(int x, int y, int xalign, int yalign,
			TapEventHandler handler)
	{
		this(x, y, xalign, yalign);
		mHandler = handler;
	}
	
	/**
	 * Coordinates are in space of physical screen/touchable area.
	 * 
	 * @param x
	 * @param y
	 * @param w
	 * @param h
	 */
	public void setSensitiveArea(int x, int y, int w, int h)
	{
		mX0 = x;
		mY0 = y;
		mX1 = x + w;
		mY1 = y + h;
	}
	
	public void setHandler(TapEventHandler handler)
	{
		mHandler = handler;
	}

	/**
	 * @param rctx
	 * @param region
	 * @param w			Width of sprite in frustum coords
	 * @param h			Height of sprite in frustum coords
	 */
	public void initRendering(RenderContext rctx, TextureRegion region,
			int w, int h)
	{
		SimpleRect vp = rctx.getViewport();
		SimpleRect fr = rctx.get2DFrustum();
		float scale_x = (float) vp.w / (float) fr.w;
		float scale_y = (float) vp.h / (float) fr.h;
		int vx = vp.x;
		int vy = vp.y;
		int x = mRefX;
		if (mXAlign == 0)
			x -= w / 2;
		else
		if (mXAlign > 0)
			x += w;
		int y = mRefY;
		if (mYAlign == 0)
			y -= h / 2;
		else
		if (mYAlign > 0)
			y += h;
		mSprite = rctx.createSprite(region, x, y, w, h);
		setSensitiveArea(
				(int) ((float) x * scale_x) + vx,
				(int) ((float) y * scale_y) + vy,
				(int) ((float) w * scale_x),
				(int) ((float) h * scale_y));
	}

	/**
	 * Returns false if event isn't a tap over this widget's touchable area.
	 */
	@Override
	public boolean handleTapEvent(Event e) {
		if (null == mHandler || e.mCode != Event.TAP ||
				e.mDatum1 < mX0 || e.mDatum1 >= mX1 ||
				e.mDatum2 < mY0 || e.mDatum2 >= mY1)
		{
			return false;
		}
		return mHandler.handleTapEvent(e);
	}
	
	public void render(RenderContext rctx)
	{
		mSprite.render(rctx);
	}

	public abstract Image getImage();
}
