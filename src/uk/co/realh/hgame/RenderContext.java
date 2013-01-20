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
 * Manages rendering.
 * 
 * @author Tony Houghton
 * @see Renderer
 */
public abstract class RenderContext {
	
	protected static final String TAG = "RenderContext";
	
	public static final int REASON_INIT = 0;
	public static final int REASON_DISPOSE = 1;
	public static final int REASON_RESIZE = 2;
	public static final int REASON_RENDER = 3;
	public static final int REASON_REPLACE = 4;
	
	protected int mWidth;
	protected int mHeight;
	
	protected Renderer mRenderer;
	private volatile Renderer mNewRenderer;
	private boolean mRendering;
	protected volatile int mRenderReason;
	protected volatile boolean mRenderBlocking;
	
	private static int smIdIndex = 0;
	public int mId;
	
	protected boolean mNativeSize;

	private TextureAtlas mBoundTexture;
	
	protected RenderContext()
	{
		mId = ++smIdIndex;
	}

	/**
	 * Old renderer, if any, will have its replacingRenderer method called.
	 * initRendering will then be called on new renderer, followed by render
	 * if appropriate (last request reason was REASON_RENDER).
	 * 
	 * @param renderer	New Renderer, may be null.
	 */
	synchronized
	public void setRenderer(Renderer renderer)
	{
		mNewRenderer = renderer;
		requestRender(REASON_REPLACE, true);
	}

	/**
	 * To be called by game or event thread to request rendering thread
	 * to perform one of the defined actions.
	 * 
	 * @param reason	One of the reason constants above
	 * @param block		Whether to block until the request has been serviced.
	 */
	synchronized
	public void requestRender(int reason, boolean block) {
		mRenderBlocking = block;
		mRenderReason = reason;
		implementRenderRequest();
		if (block)
		{
			try {
				wait();
			} catch (InterruptedException e) {
				Log.w(TAG, "Interrupted while waiting for " +
						mRenderer.getDescription() + " to service " +
						" render request " + reason, e);
			}
		}
	}
	
	/**
	 * Convenience function to request a screen refresh.
	 */
	synchronized
	public void requestRender() {
		mRenderBlocking = false;
		mRenderReason = REASON_RENDER;
		implementRenderRequest();
	}

	/**
	 * Communication between threads is implementation-dependent so subclass
	 * must make actual request.
	 */
	protected abstract void implementRenderRequest();
	
	/**
	 * Called by subclass to service a rendering request. If REASON_RENDER
	 * (and when REASON_REPLACE/RESIZE triggers a render) this doesn't actually
	 * do rendering but returns true. This prevents potentially lengthy
	 * rendering from blocking the requester.
	 * 
	 * @return	Whether calling subclass should perform a render.
	 */
	synchronized
	protected boolean serviceRenderRequest()
	{
		boolean rendering = false;
		switch (mRenderReason)
		{
		case REASON_INIT:
			if (null != mRenderer)
				mRenderer.initRendering(this);
			mWidth = getCurrentScreenWidth();
			mHeight = getCurrentScreenHeight();
			break;
		case REASON_DISPOSE:
			if (null != mRenderer)
				mRenderer.disposeRendering(this);
			break;
		case REASON_RESIZE:
			int w = getCurrentScreenWidth();
			int h = getCurrentScreenHeight();
			if (w != mWidth || h != mHeight)
			{
				mWidth = w;
				mHeight = w;
				if (null != mRenderer)
				{
					mRenderer.resizeRendering(this, w, h);
					rendering = mRendering;
				}
			}
			break;
		case REASON_RENDER:
			rendering = true;
			break;
		case REASON_REPLACE:
			if (null != mRenderer)
				mRenderer.replacingRenderer(this);
			mRenderer = mNewRenderer;
			mNewRenderer = null;
			if (null != mNewRenderer)
			{
				mRenderer.initRendering(this);
				rendering = mRendering;
			}
			break;
		}
		mRendering = rendering;
		if (mRenderBlocking)
			notifyAll();
		return rendering;
	}

	/**
	 * @return Current width of screen/window.
	 */
	protected abstract int getCurrentScreenWidth();

	/**
	 * @return Current height of screen/window.
	 */
	protected abstract int getCurrentScreenHeight();

	/**
	 * @return Cached width of screen/window.
	 */
	public int getScreenWidth()
	{
		return mWidth;
	}

	/**
	 * @return Cached height of screen/window.
	 */
	public int getScreenHeight()
	{
		return mHeight;
	}
	
	/**
	 * If NativeSize is true we can use NEAREST texture scaling method,
	 * otherwise LINEAR.
	 * 
	 * @param native_size	Whether textures can be rendered without scaling.
	 */
	public void setNativeSize(boolean native_size)
	{
		mNativeSize = native_size;
	}

	/**
	 * @return		Whether textures can be rendered without scaling.
	 * @see 		setNativeSize(boolean)
	 */
	public boolean getNativeSize()
	{
		return mNativeSize;
	}

	/**
	 * Create a TextureAtlas from an Image.
	 * 
	 * @param img
	 * @param alpha		Whether texture should have an alpha channel.
	 * @return
	 */
	public abstract TextureAtlas uploadTexture(Image img, boolean alpha);
	
	/**
	 * Create a sprite for easy rendering of a TextureRegion.
	 * 
	 * @param texture
	 * @param w		Width
	 * @param h		Height
	 * @return
	 */
    public abstract Sprite createSprite(TextureRegion texture, int w, int h);

    /**
     * Create a TileBatcher.
     * 
     * @param w			Number of visible columns
     * @param h			Number of visible rows
     * @param tile_w	Width of each tile in frustum space
     * @param tile_h	Height of each tile in frustum space
     * @return
     */
    public abstract TileBatcher createTileBatcher(int w, int h,
    		int tile_w, int tile_h);
    
    /**
     * Wrapper to bind texture, checking it isn't already bound.
     * 
     * @param tex	Texture to bind, may be null
     */
    public void bindTexture(TextureAtlas tex)
    {
    	if (mBoundTexture != tex)
    	{
    		doBindTexture(tex);
    		mBoundTexture = tex;
    	}
    }
    
    /**
     * Makes sure given texture isn't bound. Does nothing if some other
     * texture is bound.
     * 
     * @param tex
     */
    public void unbindTexture(TextureAtlas tex)
    {
    	if (mBoundTexture == tex)
    	{
    		doBindTexture(null);
    		mBoundTexture = null;
    	}
    }

	/**
	 * Implementation of texture binding.
	 * 
     * @param tex	Texture to bind, may be null
	 */
	protected abstract void doBindTexture(TextureAtlas tex);

	/**
	 * Sets the viewport.
	 * 
	 * @param x		Left
	 * @param y		Top
	 * @param w		Width
	 * @param h		Height
	 */
    public abstract void setViewport(int x, int y, int w, int h);

    /**
	 * Sets frustum for 2D rendering, allowing game to use different
	 * coordinate space from viewport. This follows hgame's convention
	 * of the top being the origin and downwards being positive, but you
	 * can flip top and bottom if you want OpenGL convention.
     * 
     * @param l	Left
     * @param r	Right
     * @param b	Bottom
     * @param t	Top
     */
    public abstract void set2DFrustum(int l, int r, int b, int t);

    /**
     * Enable/disable alpha blending.
     * 
     * @param enable	Whether blending should be enabled
     */
    public abstract void enableBlend(boolean enable);

}
