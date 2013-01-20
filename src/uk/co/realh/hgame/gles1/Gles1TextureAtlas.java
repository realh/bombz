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
 * @author Tony Houghton
 *
 */
public class Gles1TextureAtlas implements TextureAtlas {
	
	protected final int mWidth, mHeight;
	public final int mTextureId;
	private Gles1RenderContext mRCtx;
	
	private static int[] smIdBucket = new int[1];
	
	/**
	 * Texture is uploaded by RenderContext.
	 * 
	 * @param gl
	 * @param w
	 * @param h
	 * @see uk.co.realh.hgame.RenderContext#uploadTexture(Image, boolean)
	 */
	public Gles1TextureAtlas(Gles1RenderContext rctx, int w, int h)
	{
		mRCtx = rctx;
		mWidth = w;
		mHeight = h;
		rctx.mGL.glGenTextures(1, smIdBucket, 0);
		mTextureId = smIdBucket[0];
	}

	@Override
	public int getWidth() {
		return mWidth;
	}

	@Override
	public int getHeight() {
		return mHeight;
	}

	@Override
	public TextureRegion createRegion(int x, int y, int w, int h) {
		return new Gles1TextureRegion(this, x, y, w, h);
	}

	@Override
	public void dispose() {
		mRCtx.unbindTexture(this);
		int[] ids = {mTextureId};
		mRCtx.mGL.glDeleteTextures(1, ids, 0);
	}

}
