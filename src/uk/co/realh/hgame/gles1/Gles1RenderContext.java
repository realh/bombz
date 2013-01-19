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

import uk.co.realh.hgame.Image;
import uk.co.realh.hgame.RenderContext;
import uk.co.realh.hgame.Sprite;
import uk.co.realh.hgame.TextureAtlas;
import uk.co.realh.hgame.TextureRegion;
import uk.co.realh.hgame.TileBatcher;

/**
 * @author Tony Houghton
 *
 */
public class Gles1RenderContext extends RenderContext {
	
	public final GL10 mGL;
	
	protected Gles1RenderContext(GL10 gl)
	{
		mGL = gl;
	}

	/* (non-Javadoc)
	 * @see uk.co.realh.hgame.RenderContext#implementRenderRequest()
	 */
	@Override
	protected void implementRenderRequest() {
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see uk.co.realh.hgame.RenderContext#getCurrentScreenWidth()
	 */
	@Override
	protected int getCurrentScreenWidth() {
		// TODO Auto-generated method stub
		return 0;
	}

	/* (non-Javadoc)
	 * @see uk.co.realh.hgame.RenderContext#getCurrentScreenHeight()
	 */
	@Override
	protected int getCurrentScreenHeight() {
		// TODO Auto-generated method stub
		return 0;
	}

	/* (non-Javadoc)
	 * @see uk.co.realh.hgame.RenderContext#uploadTexture(uk.co.realh.hgame.Image, boolean)
	 */
	@Override
	public TextureAtlas uploadTexture(Image img, boolean alpha) {
		// TODO Auto-generated method stub
		return null;
	}

	/* (non-Javadoc)
	 * @see uk.co.realh.hgame.RenderContext#createSprite(uk.co.realh.hgame.TextureRegion, int, int)
	 */
	@Override
	public Sprite createSprite(TextureRegion texture, int w, int h) {
		// TODO Auto-generated method stub
		return null;
	}

	/* (non-Javadoc)
	 * @see uk.co.realh.hgame.RenderContext#createTileBatcher(int, int, int)
	 */
	@Override
	public TileBatcher createTileBatcher(int w, int h, int tile_size) {
		// TODO Auto-generated method stub
		return null;
	}

	/* (non-Javadoc)
	 * @see uk.co.realh.hgame.RenderContext#doBindTexture(uk.co.realh.hgame.TextureAtlas)
	 */
	@Override
	protected void doBindTexture(TextureAtlas tex) {
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see uk.co.realh.hgame.RenderContext#setViewport2D(int, int, int, int)
	 */
	@Override
	public void setViewport2D(int x, int y, int w, int h) {
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see uk.co.realh.hgame.RenderContext#enableBlend(boolean)
	 */
	@Override
	public void enableBlend(boolean enable) {
		// TODO Auto-generated method stub

	}

}
