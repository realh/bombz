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

package uk.co.realh.hgame.android;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.view.Window;
import android.view.WindowManager;

import uk.co.realh.hgame.GameManager;
import uk.co.realh.hgame.RenderContext;
import uk.co.realh.hgame.Sys;
import uk.co.realh.hgame.gles1.android.AndroidGles1RenderContext;

/**
 * @author Tony Houghton
 *
 */
public abstract class HGameActivity extends Activity {
	
	protected static final String TAG = "Activity";
	private GLSurfaceView mGlView;
	protected Sys mSys;
	protected GameManager mMgr;
	
	/**
	 * Superclass should call this and set game manager in constructor
	 * or at least before calling onCreate() below.
	 * 
	 * @param owner		Name associated with developer
	 * @param domain	Domain of URL associated with app/developer
	 * @param appName	App name
	 * @param pkg		Package name (containing R class)
	 */
	protected final void createSys(String owner,
			String domain, String appName, String pkg)
	{
		if (null == mSys)
		{
			try {
				mSys = new AndroidSys(this, owner, domain,
						appName, pkg);
			} catch (Throwable e) {
				Log.e(TAG, "Can't create AndroidSys object", e);
				throw new RuntimeException("Can't create AndroidSys object");
			}
		}
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		uk.co.realh.hgame.Log.setLogger(new AndroidLog());
		
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);
		
		mGlView = new GLSurfaceView(this);
		mGlView.setRenderer(new HGameRenderer());
		setContentView(mGlView);
	}

	/*
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_bombz_android, menu);
		return true;
	}
	*/
	
	@Override
	public void onResume() {
		super.onResume();
		mMgr.resume();
		mGlView.onResume();
	}
	
	@Override
	public void onPause() {
		super.onPause();
		mMgr.suspend();
		mGlView.onPause();
	}
	
	private class HGameRenderer implements GLSurfaceView.Renderer
	{

		private AndroidGles1RenderContext mRCtx;
		private int mW, mH;
		
		/**
		 * @see android.opengl.GLSurfaceView.Renderer#onDrawFrame(
		 * 		javax.microedition.khronos.opengles.GL10)
		 */
		@Override
		public void onDrawFrame(GL10 gl) {
			try {
				Log.d(TAG, "onDrawFrame");
				mRCtx.onDrawFrame();
			} catch (Throwable e) {
				Log.e(TAG, "Rendering exception", e);
			}
		}

		/**
		 * @see android.opengl.GLSurfaceView.Renderer#onSurfaceChanged(
		 * 		javax.microedition.khronos.opengles.GL10, int, int)
		 */
		@Override
		public void onSurfaceChanged(GL10 gl, int w, int h) {
			if (w != mW || h != mH)
			{
				Log.d(TAG, "Surface resized to " + w + "x" + h);
				mRCtx.updateSize(w, h);
				mRCtx.preRequestRender(RenderContext.REASON_RESIZE);
			}
		}
		
		/**
		 * @see android.opengl.GLSurfaceView.Renderer#onSurfaceCreated(
		 * 		javax.microedition.khronos.opengles.GL10,
		 * 		javax.microedition.khronos.egl.EGLConfig)
		 */
		@Override
		public void onSurfaceCreated(GL10 gl, EGLConfig config) {
			mW = mGlView.getWidth();
			mH = mGlView.getHeight();
			Log.d(TAG, "Surface created " + mW + "x" + mH);
			mRCtx = new AndroidGles1RenderContext(mGlView, gl, mMgr.mScreen);
			mMgr.setRenderContext(mRCtx);
			mRCtx.preRequestRender(RenderContext.REASON_INIT);
		}
	}
}
