/*
 * Copyright (c) 2012, Tony Houghton <h@realh.co.uk>
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

// HGame - a simple cross-platform game framework

// FontHelper - Wrapper for Java functions for native Font

package uk.co.realh.hgame.handroid;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.Typeface;

public class FontHelper {
	
	private Typeface mTypeface = Typeface.DEFAULT_BOLD;
	private Paint mPaint = new Paint();
	
	public FontHelper(int size)
	{
		mPaint.setTypeface(mTypeface);
		mPaint.setTextSize(size);
		mPaint.setTextAlign(Paint.Align.LEFT);
	}
	
	public Bitmap render(int colour, String text)
	{
		Rect bounds = new Rect();
		mPaint.getTextBounds(text, 0, text.length(), bounds);
		mPaint.setColor(colour);
		Bitmap bmp = Bitmap.createBitmap(bounds.width(), bounds.height(),
				Bitmap.Config.ARGB_8888);
		// bmp.eraseColor(0);	Enable in case create doesn't do same
		Canvas canv = new Canvas(bmp);
		canv.drawText(text, 0, 0, mPaint);
		return bmp;
	}

}
