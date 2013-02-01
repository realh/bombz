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

package uk.co.realh.hgame.android;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.Typeface;
import uk.co.realh.hgame.Font;
import uk.co.realh.hgame.Image;
import uk.co.realh.hgame.ShadowMaker;

/**
 * @author Tony Houghton
 *
 */
public class AndroidFont implements Font {
	
	Paint mPaint = new Paint();

	/**
	 * @param size	in pixels
	 */
	public AndroidFont(int size) {
		mPaint.setTypeface(Typeface.DEFAULT_BOLD);
		mPaint.setTextSize(size);
	}

	@Override
	public void setColour(int r, int g, int b) {
		mPaint.setARGB(255, r, g, b);
	}

	/**
	 * @see uk.co.realh.hgame.Font#render(java.lang.String)
	 */
	@Override
	public Image render(String text, int shadow_offset) {
		Rect bounds = new Rect();
		mPaint.getTextBounds(text, 0, text.length(), bounds);
		Bitmap.Config config = Bitmap.Config.ARGB_8888;
		Bitmap bmp = Bitmap.createBitmap(bounds.width(), bounds.height(),
				config);
		Canvas canvas = new Canvas(bmp);
		canvas.drawText(text, 0, 0, mPaint);
		Image img = new AndroidImage(bmp);
		if (shadow_offset != 0)
		{
			Image tmp = img;
			img = ShadowMaker.makeShadow(tmp, shadow_offset);
			tmp.dispose();
		}
		return img;
	}

}
