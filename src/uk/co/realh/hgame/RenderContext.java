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
	
	/**
	 * When an initRendering has been completed this flag is set to true
	 * and notifyAll called.
	 */
	boolean mReady;
	
	/**
	 * Subclasses are responsible for making sure the Renderer gets called
	 * with REASON_INIT.
	 * 
	 * @param renderer	Initial renderer
	 */
	protected RenderContext(Renderer renderer)
	{
		mId = ++smIdIndex;
		mRenderer = renderer;
		mRenderReason = REASON_INIT;
	}
	
	/**
	 * Old renderer, if any, will have its replacingRenderer method called.
	 * replacedRenderer will then be called on new renderer, followed by render
	 * if appropriate (last request reason was REASON_RENDER).
	 * 
	 * @param renderer	New Renderer, may be null.
	 */
	synchronized
	public void setRenderer(Renderer renderer)
	{
		if (renderer != mRenderer)
		{
			mNewRenderer = renderer;
			requestRender(REASON_REPLACE, true);
		}
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
		if (mRenderBlocking)
			Log.f(TAG, "requestRender called while already blocking");
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
	 * Like requestRender but doesn't actually request the render.
	 * This is so that it can be used in situations such as in conjunction
	 * with Android's GLSurfaceView.onPause(). It is anticipated that
	 * the system will handle blocking in that situation.
	 * 
	 * @param reason
	 * @see requestRender(int, boolean)
	 */
	synchronized
	public void preRequestRender(int reason) {
		if (mRenderBlocking)
			Log.f(TAG, "requestRender called while already blocking");
		mRenderBlocking = false;
		mRenderReason = reason;
	}
	
	/**
	 * Convenience function to request a screen refresh.
	 */
	synchronized
	public void requestRender() {
		if (mRenderBlocking)
			Log.f(TAG, "requestRender called while already blocking");
		mRenderBlocking = false;
		mRenderReason = REASON_RENDER;
		implementRenderRequest();
	}
	
	synchronized
	public void waitUntilReady() {
		while (!mReady) {
			try {
				wait();
			} catch (InterruptedException e) {
				Log.w(TAG, "Interrupt waiting for RenderContext ready", e);
			}
		}
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
	 * @throws IOException 
	 */
	synchronized
	protected boolean serviceRenderRequest()
	{
		boolean rendering = false;
		try {
			switch (mRenderReason)
			{
			case REASON_INIT:
				mWidth = getCurrentScreenWidth();
				mHeight = getCurrentScreenHeight();
				if (null != mRenderer)
				{
					mRenderer.initRendering(this, mWidth, mHeight);
					mReady = true;
					notifyAll();
					rendering = true;
				}
				break;
			case REASON_DISPOSE:
				mReady = false;
				if (null != mRenderer)
					mRenderer.deleteRendering(this);
				break;
			case REASON_RESIZE:
				int w = getCurrentScreenWidth();
				int h = getCurrentScreenHeight();
				if (w != mWidth || h != mHeight)
				{
					mWidth = w;
					mHeight = h;
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
				if (null != mRenderer)
				{
					mRenderer.replacedRenderer(this);
					rendering = mRendering;
				}
				break;
			}
			mRendering = rendering;
			if (mRenderBlocking)
			{
				notifyAll();
				mRenderBlocking = false;
			}
		} catch (Throwable e) {
			rendering = false;
			Log.e(TAG, "Exception in RenderContext", e);
		}
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
     * Size is in frustum's space.
	 * 
	 * @param texture
	 * @param w		Width
	 * @param h		Height
	 * @return
	 */
    public abstract Sprite createSprite(TextureRegion texture, int w, int h);

	/**
	 * Create a sprite for easy rendering of a TextureRegion.
     * Size and position are in frustum's space.
	 * 
	 * @param texture
	 * @param x		Initial position
	 * @param y		Initial position
	 * @param w		Width
	 * @param h		Height
	 * @return
	 */
	public abstract Sprite createSprite(TextureRegion texture, int x, int y,
			int w, int h);
	
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
     * Binds texture, checking it isn't already bound.
     * 
     * @param tex	Texture to bind, may be null
     */
    public abstract void bindTexture(TextureAtlas tex);
    
    /**
     * Makes sure given texture isn't bound. Does nothing if some other
     * texture is bound.
     * 
     * @param tex
     */
    public abstract void unbindTexture(TextureAtlas tex);

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
	 * Sets the viewport.
     * 
     * @param r
     */
    public void setViewport(SimpleRect r) {
    	setViewport(r.x, r.y, r.w, r.h);
    }
    
    /**
     * @return	Viewport set by setViewport.
     */
    public abstract SimpleRect getViewport();

    /**
	 * Sets frustum for 2D rendering, allowing game to use different
	 * coordinate space from viewport.
     * 
     * @param l	Left
     * @param r	Right
     * @param b	Bottom
     * @param t	Top
     */
    public abstract void set2DFrustum(int l, int r, int b, int t);
    
    /**
	 * Sets frustum for 2D rendering, allowing game to use different
	 * coordinate space from viewport.
	 * 
     * @param r
     */
    public void set2DFrustum(SimpleRect r) {
    	set2DFrustum(r.x, r.x + r.w, r.y + r.h, r.y);
    }
    
    /**
     * @return	SimpleRect representation of frustum set by set2DFrustum
     */
    public abstract SimpleRect get2DFrustum();

    /**
     * Enables/disables alpha blending.
     * 
     * @param enable	Whether blending should be enabled
     */
    public abstract void enableBlend(boolean enable);
    
    /**
     * Enables/disables 2D textures.
     * 
     * @param enable
     */
    public abstract void enable2DTextures(boolean enable);
    
    /**
     * Clears the screen to black.
     */
    public abstract void cls();
    
    /**
     * @return	Ratio of viewport / frustum width
     */
    public float getScaleFactorX()
    {
    	return (float) getViewport().w / (float) get2DFrustum().w;
    }
    
    /**
     * @return	Ratio of viewport / frustum height
     */
    public float getScaleFactorY()
    {
    	return (float) getViewport().h / (float) get2DFrustum().h;
    }
}
