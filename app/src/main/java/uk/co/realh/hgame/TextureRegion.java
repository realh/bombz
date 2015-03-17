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
 * A rectangular region in a TextureAtlas containing an individual texture.
 * 
 * @author Tony Houghton
 * @see TextureAtlas
 */
public class TextureRegion {

	// public fields not very good Java, but we sacrifice style for speed.
    public final TextureAtlas mAtlas;
    // Coordinates are normalised to 0.0-1.0 with origin at top-left.
    public final float mU0, mV0, mU1, mV1;
    
    /**
     * Coordinates are normalised to 0.0-1.0 with origin at top-left.
     * 
     * @param atlas
     * @param u0
     * @param v0
     * @param u1
     * @param v1
     */
    protected TextureRegion(TextureAtlas atlas,
    		float u0, float v0, float u1, float v1)
    {
        mAtlas = atlas;
        mU0 = u0;
		mV0 = v0;
		mU1 = u1;
		mV1 = v1;
    }

    /**
     * Coordinates are in pixel units in atlas source's space
     * with origin at top-left.
     * 
     * @param atlas
     * @param x
     * @param y
     * @param w
     * @param h
     */
    public TextureRegion(TextureAtlas atlas,
    		int x, int y, int w, int h)
	{
	    mAtlas = atlas;
	    float aw = (float) atlas.getWidth();
	    float ah = (float) atlas.getHeight();
	    mU0 = (float) x / aw;
	    mV0 = (float) y / ah;
	    mU1 = mU0 + (float) w / aw;
	    mV1 = mV0 + (float) h / ah;
	}
    
    /**
     * Makes it easier for subclasses to calculate coords with helper funcs.
     * 
     * @param atlas
     * @param coords
     */
    protected TextureRegion(TextureAtlas atlas, float[] coords)
    {
        mAtlas = atlas;
        mU0 = coords[0];
		mV0 = coords[1];
		mU1 = coords[2];
		mV1 = coords[3];
    }
}
