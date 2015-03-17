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
 * @author Tony Houghton
 *
 */
public class TextWidget extends Widget {
	
	private String mText;
	private int mShadow;
	private Font mFont;

	/**
	 * @param text
	 * @param x
	 * @param y
	 * @param xalign
	 * @param yalign
	 * @param shadow	Shadow offset
	 * @see Widget#Widget(int, int, int, int)
	 * @see Font#render(String, int)
	 */
	public TextWidget(String text,
			int x, int y, int xalign, int yalign,
			int shadow)
	{
		super(x, y, xalign, yalign);
		mText = text;
		mShadow = shadow;
	}
	
	/**
	 * @param text
	 * @param x
	 * @param y
	 * @param xalign
	 * @param yalign
	 * @param shadow	Shadow offset
	 * @param handler
	 * @see Widget#Widget(int, int, int, int)
	 * @see Font#render(String, int)
	 */
	public TextWidget(String text,
			int x, int y, int xalign, int yalign,
			int shadow, TapEventHandler handler)
	{
		super(x, y, xalign, yalign, handler);
		mText = text;
		mShadow = shadow;
	}

	/**
	 * Font is set on fly because size may change with render context.
	 * 
	 * @param font
	 */
	public void setFont(Font font)
	{
		mFont = font;
	}

	@Override
	public Image getImage() {
		return mFont.render(mText, mShadow);
	}
	
	public void setShadowOffset(int shadow)
	{
		mShadow = shadow;
	}

}
