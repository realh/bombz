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

package uk.co.realh.hgame.gles1;

import uk.co.realh.hgame.TextureAtlas;
import uk.co.realh.hgame.TextureRegion;

/**
 * Provides mCoords in order:
 * Bottom-left, top-left, bottom-right, top-right
 * Suitable for a GL_TRIANGLE_STRIP,
 * @link http://en.wikipedia.org/wiki/Triangle_strip
 * 
 * @author Tony Houghton
 *
 */
public class Gles1TextureRegion extends TextureRegion {

	/**
	 * More bad but faster Java.
	 */
	public final float[] mCoords;

	/**
	 * @param atlas
	 * @param x
	 * @param y
	 * @param w
	 * @param h
	 */
	protected Gles1TextureRegion(Gles1TextureAtlas atlas,
			int x, int y, int w, int h) {
		super(atlas, calcUV(atlas, x, y, w, h));
	    mCoords = new float[8];
		// Bottom-left, top-left, bottom-right, top-right
	    mCoords[0] = mU0;
	    mCoords[1] = mV1;
	    mCoords[2] = mU0;
	    mCoords[3] = mV0;
	    mCoords[4] = mU1;
	    mCoords[5] = mV1;
	    mCoords[6] = mU1;
	    mCoords[7] = mV0;
	}
	
	private static float[] smUV = new float[4];
	
	/**
     * Edges are brought inwards by half a pixel to prevent artifacts.
	 * 
	 * @param atlas
	 * @param x
	 * @param y
	 * @param w
	 * @param h
	 * @return
	 */
	private static float[] calcUV(TextureAtlas atlas,
			int x, int y, int w, int h) {
	    float aw = (float) atlas.getWidth();
	    float ah = (float) atlas.getHeight();
	    smUV[0] = ((float) x + 0.5f) / aw;
	    smUV[1] = ((float) y + 0.5f) / ah;
	    smUV[2] = smUV[0] + (float) (w - 1) / aw;
	    smUV[3] = smUV[1] + (float) (h - 1) / ah;
	    return smUV;
	}
}
