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

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.opengles.GL10;

import uk.co.realh.hgame.RenderContext;
import uk.co.realh.hgame.Sprite;
import uk.co.realh.hgame.TextureRegion;

/**
 * Note all Sprites must be created on same thread.
 * 
 * @author Tony Houghton
 *
 */
public class Gles1Sprite extends Sprite {

	private FloatBuffer mVertBuffer;
	private FloatBuffer mTexBuffer;
	
    /**
     * Width and height are in frustum's space.
     * 
     * @param texture
     * @param w			Width
     * @param h			Height
     */
	public Gles1Sprite(TextureRegion texture, int x, int y, int w, int h) {
		super(texture, x, y, w, h);
		
		// 4 points * 2 (x, y) * 4 bytes
		ByteBuffer bb = ByteBuffer.allocateDirect(32);
		bb.order(ByteOrder.nativeOrder());
		mVertBuffer = bb.asFloatBuffer();
		
		bb = ByteBuffer.allocateDirect(32);
		bb.order(ByteOrder.nativeOrder());
		mTexBuffer = bb.asFloatBuffer();
		
		setupVertCoords();
		setupTexCoords();
	}

	@Override
    public void setTexture(TextureRegion tex)
    {
		mTexture = tex;
        setupTexCoords();
    }
    
	/**
	 * @see uk.co.realh.hgame.Sprite#setPositionAndSize(int, int, int, int)
	 */
	@Override
    public void setPositionAndSize(int x, int y, int w, int h)
    {
        mX = x;
        mY = y;
        mW = w;
        mH = h;
        setupVertCoords();
    }
    
	/**
	 * @see uk.co.realh.hgame.Sprite#render(uk.co.realh.hgame.RenderContext)
	 */
	@Override
	public void render(RenderContext rc) {
		GL10 gl = ((Gles1RenderContext) rc).mGL;
		gl.glVertexPointer(2, GL10.GL_FLOAT, 0, mVertBuffer);
		gl.glTexCoordPointer(2, GL10.GL_FLOAT, 0, mTexBuffer);
		gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP, 0, 4);
	}
	
	/**
	 * Sets up OpenGL position and size coord buffers.
	 * Bottom-left, top-left, bottom-right, top-right.
	 */
	private final void setupVertCoords() {
		mVertBuffer.position(0);
		mVertBuffer.put((float) mX);
		mVertBuffer.put((float) (mY + mH));
		mVertBuffer.put((float) mX);
		mVertBuffer.put((float) mY);
		mVertBuffer.put((float) (mX + mW));
		mVertBuffer.put((float) (mY + mH));
		mVertBuffer.put((float) (mX + mW));
		mVertBuffer.put((float) mY);
		mVertBuffer.flip();
	}
	
	/**
	 * Sets up OpenGL texture coord buffers.
	 * Bottom-left, top-left, bottom-right, top-right.
	 */
	private final void setupTexCoords() {
		mTexBuffer.position(0);
		mTexBuffer.put(((Gles1TextureRegion) mTexture).mCoords);
		mTexBuffer.flip();
	}

}
