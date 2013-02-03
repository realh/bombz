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

import javax.microedition.khronos.opengles.GL10;


import uk.co.realh.hgame.RenderContext;
import uk.co.realh.hgame.Renderer;
import uk.co.realh.hgame.SimpleRect;
import uk.co.realh.hgame.Sprite;
import uk.co.realh.hgame.TextureAtlas;
import uk.co.realh.hgame.TextureRegion;
import uk.co.realh.hgame.TileBatcher;

/**
 * @author Tony Houghton
 *
 */
public abstract class Gles1RenderContext extends RenderContext {
	
	public final GL10 mGL;
	protected int mWidth, mHeight;
	protected int mBoundTexture;
	private SimpleRect mViewport, m2DFrustum;
	
	/**
	 * @param gl
	 * @param w		Initial width of surface
	 * @param h		Initial height of surface
	 */
	protected Gles1RenderContext(GL10 gl, int w, int h, Renderer renderer)
	{
		super(renderer);
		mGL = gl;
		mWidth = w;
		mHeight = h;
		gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
		gl.glClearColor(0, 0, 0, 1);
	}

	/**
	 * @see uk.co.realh.hgame.RenderContext#getCurrentScreenWidth()
	 */
	@Override
	protected int getCurrentScreenWidth() {
		return mWidth;
	}

	/**
	 * @see uk.co.realh.hgame.RenderContext#getCurrentScreenHeight()
	 */
	@Override
	protected int getCurrentScreenHeight() {
		return mHeight;
	}

	/**
	 * @see uk.co.realh.hgame.RenderContext#createSprite(
	 * 		uk.co.realh.hgame.TextureRegion, int, int)
	 */
	@Override
	public Sprite createSprite(TextureRegion texture, int w, int h) {
		return new Gles1Sprite(texture, 0, 0, w, h);
	}

	/**
	 * @see uk.co.realh.hgame.RenderContext#createSprite(
	 * 		uk.co.realh.hgame.TextureRegion, int, int, int, int)
	 */
	@Override
	public Sprite createSprite(TextureRegion texture, int x, int y,
			int w, int h) {
		return new Gles1Sprite(texture, x, y,  w, h);
	}

	/**
	 * @see uk.co.realh.hgame.RenderContext#createTileBatcher(int, int, int)
	 */
	@Override
	public TileBatcher createTileBatcher(int w, int h,
			int tile_w, int tile_h) {
		return new Gles1TileBatcher(w, h, tile_w, tile_h);
	}

	/**
	 * @see uk.co.realh.hgame.RenderContext#doBindTexture(
	 * 		uk.co.realh.hgame.TextureAtlas)
	 */
	@Override
	public void bindTexture(TextureAtlas tex) {
		int id = ((Gles1TextureAtlas) tex).mTextureId;
    	if (mBoundTexture != id)
    	{
			mGL.glBindTexture(GL10.GL_TEXTURE_2D, id);
    		mBoundTexture = id;
    	}
	}

	@Override
    public void unbindTexture(TextureAtlas tex)
    {
    	if (mBoundTexture == ((Gles1TextureAtlas) tex).mTextureId)
    	{
			mGL.glBindTexture(GL10.GL_TEXTURE_2D, 0);
    		mBoundTexture = 0;
    	}
    }
	
	/**
	 * @see uk.co.realh.hgame.RenderContext#setViewport(int, int, int, int)
	 */
	@Override
	public void setViewport(int x, int y, int w, int h) {
		mGL.glViewport(x, y, w, h);
		mViewport = new SimpleRect(x, y, w, h);
	}
	
	/**
	 * @see uk.co.realh.hgame.RenderContext#set2DFrustum(int, int, int, int)
	 * This flips top and bottom so that origin is at top.
	 */
	@Override
    public void set2DFrustum(int l, int r, int b, int t)
	{
	    mGL.glMatrixMode(GL10.GL_PROJECTION);
	    mGL.glLoadIdentity();
	    mGL.glOrthof(l, r, t, b, 1, -1);
		m2DFrustum = new SimpleRect(l, t, r - l, b - t);
	}
	/**
	 * @see uk.co.realh.hgame.RenderContext#getViewport()
	 */
	@Override
	public SimpleRect getViewport() {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * @see uk.co.realh.hgame.RenderContext#get2DFrustum()
	 */
	@Override
	public SimpleRect get2DFrustum() {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * @see uk.co.realh.hgame.RenderContext#enableBlend(boolean)
	 */
	@Override
	public void enableBlend(boolean enable) {
		if (enable)
		{
			mGL.glEnable(GL10.GL_BLEND);
			mGL.glBlendFunc(GL10.GL_SRC_ALPHA, GL10.GL_ONE_MINUS_SRC_ALPHA);
		}
		else
		{
			mGL.glDisable(GL10.GL_BLEND);
		}
	}

	@Override
    public void enable2DTextures(boolean enable)
    {
		if (enable)
		{
			mGL.glEnableClientState(GL10.GL_TEXTURE_COORD_ARRAY);
			mGL.glEnable(GL10.GL_TEXTURE_2D);
		}
		else
		{
			mGL.glDisable(GL10.GL_TEXTURE_2D);
			mGL.glDisableClientState(GL10.GL_TEXTURE_COORD_ARRAY);
		}
    }
	
	@Override
    public void cls()
	{
		mGL.glClear(GL10.GL_COLOR_BUFFER_BIT);
	}
}
