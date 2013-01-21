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

package uk.co.realh.hgame.android.gles1;

import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;

import uk.co.realh.hgame.Image;
import uk.co.realh.hgame.TextureAtlas;
import uk.co.realh.hgame.android.AndroidImage;
import uk.co.realh.hgame.gles1.Gles1RenderContext;
import uk.co.realh.hgame.gles1.Gles1TextureAtlas;

/**
 * @author Tony Houghton
 *
 */
public class AndroidGles1RenderContext extends Gles1RenderContext {
	
	private GLSurfaceView mView;

	/**
	 * @param view
	 * @param gl
	 */
	public AndroidGles1RenderContext(GLSurfaceView view, GL10 gl) {
		super(gl, view.getWidth(), view.getHeight());
		mView = view;
		view.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
	}

	/**
	 * @see uk.co.realh.hgame.RenderContext#implementRenderRequest()
	 */
	@Override
	protected void implementRenderRequest() {
		mView.requestRender();
	}

	/**
	 * @see uk.co.realh.hgame.RenderContext#uploadTexture(
	 * 		uk.co.realh.hgame.Image, boolean)
	 */
	@Override
	public TextureAtlas uploadTexture(Image img, boolean alpha) {
		float scaling = mNativeSize ? GL10.GL_NEAREST : GL10.GL_LINEAR;
		Gles1TextureAtlas atlas = new Gles1TextureAtlas(this,
				img.getWidth(), img.getHeight());
		mGL.glBindTexture(GL10.GL_TEXTURE_2D, atlas.mTextureId);
		GLUtils.texImage2D(GL10.GL_TEXTURE_2D, 0,
				((AndroidImage) img).getAndroidBitmap(), 0);
		mGL.glTexParameterf(GL10.GL_TEXTURE_2D,
				GL10.GL_TEXTURE_MIN_FILTER, scaling);
		mGL.glTexParameterf(GL10.GL_TEXTURE_2D,
				GL10.GL_TEXTURE_MAG_FILTER, scaling);
		return atlas;
	}
	
	/**
	 * Called from Activity owning view.
	 */
	public void onDrawFrame()
	{
		if (serviceRenderRequest())
		{
			mGL.glClear(GL10.GL_COLOR_BUFFER_BIT);
			mRenderer.render(this);
		}
	}
	
	/**
	 * Must be called to update size prior to requestRender(REASON_RESIZE).
	 * 
	 * @param w
	 * @param h
	 */
	public final void updateSize(int w, int h)
	{
		mWidth = w;
		mHeight = h;
	}

}
