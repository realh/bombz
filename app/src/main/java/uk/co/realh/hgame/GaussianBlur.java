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
 * Fast Gaussian blur using lookup tables.
 * 
 * @author Tony Houghton
 */
public class GaussianBlur {
	
	private static final String TAG = "Blur";
	
	private static double RHO = 0.4f;	// Gauss function parameter
	public static int SUBSPAN = 3;		// Number of pixels to blur
										// either side of focus
	
	private static int SPAN = SUBSPAN * 2 + 1;
	private static double RHO2 = 2.0 * RHO * RHO;
	private static double QUOTIENT = Math.sqrt(RHO2 * Math.PI);

	// Each member is fixed point +-8.23
	private int[][][] mTable;
	
	/* This is only needed temporarily, but it doesn't take up much
	 * memory and it's probably better to leave it to avoid GC.
	 */
	private double[][] mFactors;
	
	/**
	 * This dims as well as blurs, hence opacity parameter.
	 * 
	 * @param opacity
	 */
	public GaussianBlur(double opacity) {
		Log.d(TAG, "Creating table");
		mFactors = new double[SPAN][];
		double multiplier = 0;
		for (int j = 0; j < SPAN; ++j)
		{
			int y = j - SUBSPAN;
			mFactors[j] = new double[SPAN];
			for (int i = 0; i < SPAN; ++i)
			{
				int x = i - SUBSPAN;
				double a = Math.pow(Math.E, (double) (-x * x - y * y) / RHO2)/ QUOTIENT;
				multiplier += a;
				mFactors[j][i] = a;
			}
		}
		multiplier = opacity * (1 << 23) / multiplier;
		
		mTable = new int[SPAN][][];
		for (int y = 0; y < SPAN; ++y)
		{
			mTable[y] = new int[SPAN][];
			for (int x = 0; x < SPAN; ++x)
			{
				mTable[y][x] = new int[256];
				for (int n = 0; n < 256; ++n)
				{
					mTable[y][x][n] = (int) (multiplier * (double) n);
				}
			}
		}
		Log.d(TAG, "Created table");
	}

	public Image blurAlpha(Image src) {
		Log.d(TAG, "Blurring");
		int w = src.getWidth();
		int h = src.getHeight();
		int w2 = w + SUBSPAN * 2;
		int h2 = h + SUBSPAN * 2;
		int[] src_pix = src.getPixels();
		int[] dest_pix = new int[w2 * h2];
		for (int y = 0; y < h2; ++y) {
			for (int x = 0; x < w2; ++x) {
				int total = 0;
				for (int j = 0; j < SPAN; ++j) {
					int y2 = j + y - SUBSPAN * 2;
					if (y2 < 0)
						continue;
					else if (y2 >= h)
						break;
					for (int i = 0; i < SPAN; ++i) {
						int x2 = i + x - SUBSPAN * 2;
						if (x2 < 0)
							continue;
						else if (x2 >= w)
							break;
						total += mTable[j][i][src_pix[y2 * w + x2] >>> 24];
					}
				}
				dest_pix[y * w2 + x] = (total << 1) & 0xff000000;
			}
		}
		Image dest = src.createImage(w2, h2);
		dest.setPixels(dest_pix);
		Log.d(TAG, "Blurred");
		return dest;
	}
}
