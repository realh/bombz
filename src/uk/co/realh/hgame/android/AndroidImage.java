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
import android.graphics.Rect;
import uk.co.realh.hgame.Image;

/**
 * @author Tony Houghton
 *
 */
public class AndroidImage implements Image {
	
	private Bitmap mBitmap;

	/**
	 * Create an hgame Image from an Android bitmap.
	 * 
	 * @param bitmap
	 */
	public AndroidImage(Bitmap bitmap)
	{
		mBitmap = bitmap;
	}
	
	public Bitmap getAndroidBitmap()
	{
		return mBitmap;
	}

	/**
	 * @see uk.co.realh.hgame.Image#getWidth()
	 */
	@Override
	public int getWidth() {
		return mBitmap.getWidth();
	}

	/**
	 * @see uk.co.realh.hgame.Image#getHeight()
	 */
	@Override
	public int getHeight() {
		return mBitmap.getHeight();
	}
	
	/**
	 * @see uk.co.realh.hgame.Image#createImage(int, int)
	 */
	@Override
	public Image createImage(int w, int h) {
		return new AndroidImage(Bitmap.createBitmap(w, h,
				mBitmap.getConfig()));
	}

	/*
	 * @see uk.co.realh.hgame.Image#getPixels()
	 */
	@Override
	public int[] getPixels() {
		int w = mBitmap.getWidth();
		int h = mBitmap.getHeight();
		int[] pix = new int[w * h];
		mBitmap.getPixels(pix, 0, 0, 0, 0, w, h);
		return pix;
	}

	/*
	 * @see uk.co.realh.hgame.Image#setPixels(int[])
	 */
	@Override
	public void setPixels(int[] pixels) {
		mBitmap.setPixels(pixels, 0, 0, 0, 0,
				mBitmap.getWidth(), mBitmap.getHeight());
		
	}
	
	/**
	 * @see uk.co.realh.hgame.Image#blit(uk.co.realh.hgame.Image,
	 * 									 int, int, int, int, int, int)
	 */
	@Override
	public void blit(Image src, int dst_x, int dst_y, int src_x, int src_y,
			int w, int h) {
		Canvas canv = new Canvas(mBitmap);
		Rect src_r = new Rect(src_x, src_y, src_x + w, src_y + h);
		Rect dst_r = new Rect(dst_x, dst_y, dst_x + w, dst_y + h);
		canv.drawBitmap(((AndroidImage) src).mBitmap, src_r, dst_r, null);
	}

	private static final float BLUR_SCALE_FACTOR = 0.05f;

	/**
	 * @see uk.co.realh.hgame.Image#blur()
	 */
	@Override
	public Image blur() {
		return new AndroidImage(Bitmap.createScaledBitmap(mBitmap,
				(int) ((float) mBitmap.getWidth() * BLUR_SCALE_FACTOR),
				(int) ((float) mBitmap.getHeight() * BLUR_SCALE_FACTOR),
				true));
	}
	/**
	 * @see uk.co.realh.hgame.Image#dispose()
	 */
	@Override
	public void dispose() {
		mBitmap.recycle();
	}

}
