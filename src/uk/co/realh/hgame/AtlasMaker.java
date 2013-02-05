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

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

/**
 * Pastes a number of images together to make an atlas.
 * Optimised for "landscape" images.
 *
 * The resultant atlas is available as field mAtlas, and each element in
 * mRegions corresponds to the image with the same index in the input.
 *
 * @author Tony Houghton
 */
public class AtlasMaker {
	
	private final static String TAG = "AtlasMaker";

	public final TextureAtlas mAtlas;
	public final List<TextureRegion> mRegions;

	/**
	 * @param rctx		Needed to generate atlas
	 * @param images	List of images to paste together
	 */
	public AtlasMaker(RenderContext rctx, List<Image> images) {
		// First get a list sorted in order of image width. This holds
		// indexed into original array whose order is preserved.
		ArrayList<Integer> sorted = new ArrayList<Integer>(images.size());
		for (int n = 0; n < images.size(); ++n)
		{
			sorted.add(n);
		}
		Collections.sort(sorted, new Cmp(images));

		// Work out position of each region
		SimpleRect[] dims = new SimpleRect[images.size()];
		int widest_row = 0;
		int y = 0;	// Offset from top for next region
		/* Each row is a pair of widest+narrowest in attempt to
		 * minimise wasted space. If there's an odd number it's
		 * likely to be best to put widest on its own, which is
		 * what 'first' flag is for.
		 */
		boolean first = true;
		while (sorted.size() > 0)
		{
			int index = sorted.remove(sorted.size() - 1);
			int w = images.get(index).getWidth();
			int h = images.get(index).getHeight();
			dims[index] = new SimpleRect(0, y, w, h);
			Log.d(TAG, "Left region[" + index + "] = " +
					dims[index].toString());
			if (sorted.size() > 0 && (!first || images.size() % 2 == 0))
			{
				int index2 = sorted.remove(0);
				int w2 = images.get(index2).getWidth();
				int h2 = images.get(index2).getHeight();
				dims[index2] = new SimpleRect(w, y, w2, h2);
				Log.d(TAG, "Right region[" + index2 + "] = " +
						dims[index2].toString());
				w += w2;
				if (h2 > h)
					h = h2;
			}
			if (w > widest_row)
				widest_row = w;
			y += h;
			first = false;
		}

		// Paste all images together and create atlas
		Image montage = images.get(0).createImage(roundPow2(widest_row),
				roundPow2(y));
		Log.d(TAG, "Making atlas " + montage.getWidth() + "x" +
				montage.getHeight());
		for (int n = 0; n < dims.length; ++n)
		{
			montage.blit(images.get(n), dims[n].x, dims[n].y,
					0, 0, dims[n].w, dims[n].h);
		}
		mAtlas = rctx.uploadTexture(montage, true);
		montage.dispose();

		// Create regions
		mRegions = new ArrayList<TextureRegion>(images.size());
		for (int n = 0; n < dims.length; ++n)
		{
			mRegions.add(mAtlas.createRegion(dims[n].x, dims[n].y,
							dims[n].w, dims[n].h));
		}
	}

	/**
	 * Rounds a number to next highest power of 2.
	 *
	 * @param a
	 * @return
	 */
	public static int roundPow2(int a)
	{
	    int b = 2;
	    while (b < a)
	        b <<= 1;
	    return b;
	}

	private static class Cmp implements Comparator<Integer> {

		private final List<Image> mImages;

		public Cmp(List<Image> images) {
			mImages = images;
		}

		@Override
		public int compare(Integer arg0, Integer arg1) {
			return mImages.get(arg0).getWidth() - mImages.get(arg1).getWidth();
		}
	}
}
