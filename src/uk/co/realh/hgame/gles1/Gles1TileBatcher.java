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
import uk.co.realh.hgame.TextureRegion;
import uk.co.realh.hgame.TileBatcher;

/**
 * @author Tony Houghton
 *
 */
public class Gles1TileBatcher implements TileBatcher {
	
	private int mNColumns, mNRows;
	@SuppressWarnings("unused")
	private int mTileW;
	private int mTileH;
	
	private Gles1TextureRegion[] mRegions;
	private FloatBuffer mVertBuffer, mTexBuffer;
	private ByteBuffer mIndexBuffer;
	
	private float[] smDummyCoords = {0, 0, 0, 0, 0, 0, 0, 0};

	/**
	 * This uses byte indices, so it's limited to 256 verts per row
	 * which means 64 columns.
	 * 
	 * @param rctx
	 * @param nColumns	Number of columns in grid
	 * @param nRows		Number of rows in grid
	 * @param tile_w	Width of each tile in frustum space
	 * @param tile_h	Height of each tile in frustum space
	 */
	public Gles1TileBatcher(int nColumns, int nRows,
			int tile_w, int tile_h) {
		mNColumns = nColumns;
		mNRows = nRows;
		mTileW = tile_w;
		mTileH = tile_h;
		
		mRegions = new Gles1TextureRegion[nColumns * nRows];
		
		/* Tiles which are adjacent on screen don't necessarily have
		 * adjacent textures so corner vertices can't be shared.
		 * 4 verts * 2 (x, y) * 4 bytes per float = 32.
		 * We render one row at a time using indices and translation,
		 * and fill in texture buffer on the fly so we only need one row's
		 * worth of vertices.
		 */
		ByteBuffer bb = ByteBuffer.allocateDirect(nColumns * 32);
		bb.order(ByteOrder.nativeOrder());
		mVertBuffer = bb.asFloatBuffer();
		bb = ByteBuffer.allocateDirect(nColumns * 32);
		bb.order(ByteOrder.nativeOrder());
		mTexBuffer = bb.asFloatBuffer();
		mIndexBuffer = ByteBuffer.allocateDirect(nColumns * 6);
		
		/* Fill in fixed buffers */
		int x;
		mVertBuffer.position(0);
		for (x = 0; x < nColumns; ++x)
		{
			// Bottom-left, top-left, bottom-right, top-right
			float x0 = (float) (x * tile_w);
			float y0 = (float) tile_h;
			float x1 = x0 + (float) tile_w;
			float y1 = 0f;
			mVertBuffer.put(x0);
			mVertBuffer.put(y1);
			mVertBuffer.put(x0);
			mVertBuffer.put(y0);
			mVertBuffer.put(x1);
			mVertBuffer.put(y1);
			mVertBuffer.put(x1);
			mVertBuffer.put(y0);
		}
		mVertBuffer.flip();
		
		mIndexBuffer.position(0);
		for (x = 0; x < nColumns * 4; x += 4)
		{
			/* Two triangles:
			 * BL, TL, BR
			 * TL, BR, TR
			 * OpenGL will see these bytes as unsigned.
			 */
			byte b = (byte) ((x < 128) ? x: x - 256);
			mIndexBuffer.put(b);
			b += 1;
			mIndexBuffer.put(b);
			b += 1;
			mIndexBuffer.put(b);
			b -= 1;
			mIndexBuffer.put(b);
			b += 1;
			mIndexBuffer.put(b);
			b += 1;
			mIndexBuffer.put(b);
		}
		mIndexBuffer.flip();
	}

	/**
	 * @see uk.co.realh.hgame.TileBatcher#setTextureAt(
	 * 		uk.co.realh.hgame.TextureRegion, int, int)
	 */
	@Override
	public void setTextureAt(TextureRegion tex, int x, int y) {
		mRegions[x + y * mNColumns] = (Gles1TextureRegion) tex;
	}

	@Override
	public void render(RenderContext rctx) {
		// Render one row at a time
		GL10 gl = ((Gles1RenderContext) rctx).mGL;
		mVertBuffer.position(0);
		gl.glVertexPointer(2, GL10.GL_FLOAT, 0, mVertBuffer);
		gl.glMatrixMode(GL10.GL_MODELVIEW);
		gl.glLoadIdentity();
		mIndexBuffer.position(0);
		for (int y = 0; y < mNRows; ++y)
		{
			mTexBuffer.position(0);
			for (int x = 0; x < mNColumns; ++x)
			{
				Gles1TextureRegion reg = mRegions[x + y * mNColumns];
				mTexBuffer.put(reg == null ? smDummyCoords : reg.mCoords);
			}
			mTexBuffer.flip();
			gl.glTexCoordPointer(2, GL10.GL_FLOAT, 0, mTexBuffer);
			gl.glDrawElements(GL10.GL_TRIANGLES, mNColumns * 6,
					GL10.GL_UNSIGNED_BYTE, mIndexBuffer);
			gl.glTranslatef(0, mTileH, 0);
		}
		gl.glLoadIdentity();
	}

}
