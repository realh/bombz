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
public class ShadowMaker {
	
	@SuppressWarnings("unused")
	private static final String TAG = "Shadow";

	public static final double OPACITY = 0.4;
	
	private static final int FACTOR = (int) (OPACITY * ((float) (1 << 24)));

	public static Image makeShadow(Image src, int offset)
	{
		int src_w = src.getWidth();
		int src_h = src.getHeight();
		Image grey = src.createImage(src_w, src_h);
		int[] pix = src.getPixels();
		for (int n = 0; n < pix.length; ++n)
		{
			pix[n] = ((pix[n] >>> 24) * FACTOR) & 0xff000000;
		}
		grey.setPixels(pix);
		Image shadow = grey.blur();
		int shad_w = shadow.getWidth();
		int shad_h = shadow.getHeight();
		Image montage = src.createImage(shad_w + offset, shad_h + offset);
		montage.blit(shadow, offset, offset, 0, 0, shad_w, shad_h);
		montage.blit(src, 0, 0, 0, 0, src_w, src_h);
		grey.dispose();
		shadow.dispose();
		return montage;
	}
	
	/*
	private static GaussianBlur smBlurrer = null;
	
	public static Image makeShadow(Image src, int offset)
	{
		int src_w = src.getWidth();
		int src_h = src.getHeight();
		if (null == smBlurrer)
			smBlurrer = new GaussianBlur(OPACITY);
		Image shadow = smBlurrer.blurAlpha(src); 
		int shad_w = shadow.getWidth();
		int shad_h = shadow.getHeight();
		int padding = offset - GaussianBlur.SUBSPAN;
		if (padding < 0)
		{
			offset = -padding;
			padding = 0;
		}
		else
		{
			offset = 0;
		}
		Image montage = src.createImage(shad_w + padding, shad_h + padding);
		montage.blit(shadow, padding, padding, 0, 0, shad_w, shad_h);
		montage.blit(src, offset, offset, 0, 0, src_w, src_h);
		shadow.dispose();
		return montage;
	}
	*/
}
