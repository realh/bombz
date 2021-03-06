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

import java.io.IOException;

/**
 * Interface for objects which can maintain rendering resources and
 * perform rendering.
 * 
 * @author Tony Houghton
 * @see RenderContext
 */
public interface Renderer {
	
	/**
	 * Called when render context is initialised to load textures etc.
	 * Must be able to cope if called when already initialised because
	 * deleteRendering is not guaranteed.
	 * 
	 * Most implementations need to load textures here, hence
	 * possibility of IOException.
	 * 
	 * @param w		Width of screen in pixels
	 * @param h		Height of screen in pixels
	 * @param rc
	 * @throws IOException 
	 */
	public void initRendering(RenderContext rctx, int w, int h)
			throws IOException;
	
	/**
	 * Called when render context is being torn down to dispose of
	 * textures etc. This might never be called, you'll just get an
	 * initRendering call with a new RenderContext instead after surface
	 * has been recreated.
	 */
	public void deleteRendering(RenderContext rctx);
	
	/**
	 * Called when render context has been resized.
	 * Most implementations need to reload textures here, hence
	 * possibility of IOException.
	 * 
	 * @param rctx
	 * @param w		New window/screen width
	 * @param h		New window/screen height
	 * @throws IOException 
	 */
	public void resizeRendering(RenderContext rctx, int w, int h)
			throws IOException;
	
	/**
	 * Called when screen/window should be updated.
	 * 
	 * @param rctx
	 */
	public void render(RenderContext rctx);

	/**
	 * Called when this renderer is about to be replaced by another. It
	 * should dispose of any resources which are not going to be used again
	 * by this renderer whereas deleteRendering should dispose of all of
	 * app's rendering resources.
	 * 
	 * @param rctx
	 */
	public void replacingRenderer(RenderContext rctx);
	
	/**
	 * Called when this renderer has replaced one in an existing RenderContext.
	 * Implementations should load any specific textures etc but not reload
	 * any that were shared with other renderers.
	 * Most implementations need to reload textures here, hence
	 * possibility of IOException.
	 * 
	 * @param rctx
	 * @throws IOException 
	 */
	public void replacedRenderer(RenderContext rctx) throws IOException;
	
	/**
	 * Gets a description of the renderer. Used for debugging and errors
	 * so translation is unnecessary.
	 * 
	 * @return
	 */
	public String getDescription();
}
