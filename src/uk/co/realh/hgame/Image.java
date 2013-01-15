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
 * Abstraction of an image that can be loaded from PNG and used as a texture.
 * NB pixel coords have origin at top-left. 
 * 
 * @author Tony Houghton
 */
public interface Image {
	
	/**
	 * Creates a new Image with same properties as this but with different
	 * width and height.
	 * 
	 * @param w	width
	 * @param h	height
	 * @return	New Image.
	 */
	public Image createImage(int w, int h);
	
	/**
	 * @return	Width of Image.
	 */
	public int getWidth();
	
	/**
	 * @return	Height of Image.
	 */
	public int getHeight();
	
	/*
	 * @return	Array of one ARGB int per pixel, stride == width.
	 */
	//public int[] getPixels();
	
	/*
	 * Replaces Image's contents.
	 * 
	 * @param pixels	Array of one ARGB int per pixel, stride == width,
	 * 					total size must match Image's size.
	 */
	//public void setPixels(int[] pixels);
	
	/**
	 * Copies part of another Image into this Image using alpha as appropriate
	 * to merge/composite. Generally both Images must be same implementation
	 * class.
	 * 
	 * @param src		Source Image.
	 * @param dst_x		Destination X offset.
	 * @param dst_y		Destination Y offset.
	 * @param src_x		Source X offset.
	 * @param src_y		Source Y offset.
	 * @param w			Width of area to copy.
	 * @param h			Height of area to copy.
	 */
    public void blit(Image src, int dst_x, int dst_y,
            int src_x, int src_y, int w, int h);
    
    /**
     * @return	A blurred version of this image.
     */
    public Image blur();
    
    /**
     * Force resources to be recycled. Do not use this image afterwards.
     */
    public void dispose();
	
}
