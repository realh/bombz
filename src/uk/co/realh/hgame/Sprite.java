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
 * A single textured square/rectangle.
 *  
 * @author Tony Houghton
 *
 */
public abstract class Sprite {
	
    protected TextureRegion mTexture;
    protected int mX, mY;             // position
    protected int mW, mH;             // size
    
    /**
     * Size and position are in frustum's space.
     * 
     * @param texture
     * @param w		Width
     * @param h		Height
     */
    protected Sprite(TextureRegion texture, int x, int y, int w, int h)
    {
        mTexture = texture;
        mX = x;
        mY = y;
        mW = w;
        mH = h;
    }

    /**
     * Sets where in view sprite will be rendered.
     * x and y are in frustum's space.
     * 
     * @param x
     * @param y
     */
    public void setPosition(int x, int y)
    {
    	setPositionAndSize(x, y, mW, mH);
    }

    /**
     * Sets a new size for the sprite.
     * w and h are in frustum's space.
     * 
     * @param x
     * @param y
     */
    public void setSize(int w, int h)
    {
    	setPositionAndSize(mX, mY, w, h);
    }

    /**
     * Calling function must set everything up first, including
     * binding the texture if necessary.
     * 
     * @param rc
     */
    public abstract void render(RenderContext rc);

    /**
     * Change the sprite's texture eg for animation.
     * 
     * @param tex		New texture
     */
    public void setTexture(TextureRegion tex)
    {
        mTexture = tex;
    }
    
    /**
     * Change the sprite's position and size at once
     * 
     * @param tex		New texture
     * @param x			New left
     * @param y			New top
     * @param w			New width
     * @param h			New height
     */
    public void setPositionAndSize(int x, int y, int w, int h)
    {
        mX = x;
        mY = y;
        mW = w;
        mH = h;
    }

}
