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
	private Renderer mNewRenderer;
	private boolean mRendering;
	protected volatile int mRenderReason;
	protected volatile boolean mRenderBlocking;
	
	private static int smIdIndex = 0;
	public final int mId;
	
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
	 * (and when REASON_REPLACE triggers a render) this doesn't actually do
	 * rendering but returns true. This prevents potentially lengthy rendering
	 * from blocking the requester.
	 * @return
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
					mRenderer.resizeRendering(this, w, h);
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
				if (mRendering)
				{
					rendering = true;
				}
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

}
