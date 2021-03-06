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
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.Window;
import android.view.WindowManager;

import uk.co.realh.hgame.Event;
import uk.co.realh.hgame.GameManager;
import uk.co.realh.hgame.Log;
import uk.co.realh.hgame.OnScreenButton;
import uk.co.realh.hgame.RenderContext;
import uk.co.realh.hgame.ScreenButtonSource;
import uk.co.realh.hgame.gles1.android.AndroidGles1RenderContext;

/**
 * @author Tony Houghton
 *
 */
public abstract class HGameActivity extends Activity
		implements OnTouchListener, ScreenButtonSource {
	
	protected static final String TAG = "Activity";
	
	private GLSurfaceView mGlView;
	protected AndroidSys mSys;
	protected GameManager mMgr;
	private HGameRenderer mRenderer;
    private AndroidKbdDInput mKbdDInput;
	
	private OnScreenButton[] mButtons =
			new OnScreenButton[ScreenButtonSource.MAX_BUTTONS];
	int mNButtons = 0;
	
	public static final void initLog()
	{
		uk.co.realh.hgame.Log.setLogger(new AndroidLog());
	}
	
	/**
	 * Superclass should call this and set game manager
	 * _before_ calling onCreate below.
	 * 
	 * @param owner		Name associated with developer
	 * @param domain	Domain of URL associated with app/developer
	 * @param appName	App name
	 * @param pkg		Package name (containing R class)
	 */
	protected final void createSys(String owner,
			String domain, String appName, String pkg)
	{
        if (null == mKbdDInput)
        {
            mKbdDInput = new AndroidKbdDInput();
        }
		if (null == mSys)
		{
			try {
				mSys = new AndroidSys(this, owner, domain,
						appName, pkg);
			} catch (Throwable e) {
				Log.f(TAG, "Can't create AndroidSys object", e);
			}
		}
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		try {
			super.onCreate(savedInstanceState);
			
			requestWindowFeature(Window.FEATURE_NO_TITLE);
			getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
					WindowManager.LayoutParams.FLAG_FULLSCREEN);
			
			mGlView = new GLSurfaceView(this);
			mRenderer = new HGameRenderer();
			mGlView.setRenderer(mRenderer);
			mGlView.setOnTouchListener(this);
			setContentView(mGlView);
			mMgr.setAudio(new AndroidAudioContext(this, mSys));
		} catch (Throwable e) {
			Log.f(TAG, "Error creating app", e);
		}
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
		Log.d(TAG, "onResume");
		try {
			super.onResume();
			mGlView.onResume();
		} catch (Throwable e) {
			Log.f(TAG, "Exception in onResume", e);
		}
	}
	
	@Override
	public void onPause() {
		Log.d(TAG, "onPause");
		try {
			super.onPause();
			mMgr.suspend();
			mGlView.onPause();
		} catch (Throwable e) {
			Log.f(TAG, "Exception in onPause", e);
		}
	}
	
	@Override
	public void onDestroy() {
		Log.d(TAG, "onDestroy");
		try {
			if (null != mMgr && null != mMgr.mAudio) {
				mMgr.mAudio.dispose();
				mMgr.mAudio = null;
			}
		} catch (Throwable e) {
			Log.e(TAG, "Exception in onDestroy", e);
		}
		super.onDestroy();
	}
	
	@Override
	public void onBackPressed() {
		Log.d(TAG, "onBackPressed");
		if (!mMgr.onBackPressed())
			super.onBackPressed();
	}
	
	@Override
	public boolean onTouch(View v, MotionEvent e) {
		try {
			mMgr.resetBlankTimeout();
			int a = e.getAction();
			int i = (a & MotionEvent.ACTION_POINTER_INDEX_MASK) >>
					MotionEvent.ACTION_POINTER_INDEX_SHIFT;
			a &= MotionEvent.ACTION_MASK;
			/*
			Log.v(TAG, String.format(
					"Touch event action %d, index %d, at (%d, %d)",
					a, i, (int) e.getX(i), (int) e.getY(i)));
			*/
			switch (a) {
			case MotionEvent.ACTION_DOWN:
			case MotionEvent.ACTION_POINTER_DOWN:
				int x = (int) e.getX(i);
				int y = (int) e.getY(i);
				Event.pushEvent(Event.newEvent(Event.TAP, x, y));
				for (OnScreenButton btn: mButtons) {
					if (null == btn)
						break;
					btn.handleEvent(OnScreenButton.BUTTON_PRESS,
							(int) e.getX(i), (int) e.getY(i),
							e.getPointerId(i));
				}
				break;
			case MotionEvent.ACTION_UP:
			case MotionEvent.ACTION_POINTER_UP:
			case MotionEvent.ACTION_CANCEL:
				for (OnScreenButton btn: mButtons) {
					if (null == btn)
						break;
					btn.handleEvent(OnScreenButton.BUTTON_RELEASE,
							(int) e.getX(i), (int) e.getY(i), e.getPointerId(i));
				}
				break;
			case MotionEvent.ACTION_MOVE:
				for (OnScreenButton btn: mButtons) {
					if (null == btn)
						break;
					int l = e.getPointerCount();
					for (int j = 0; j < l; ++j) {
						btn.handleEvent(OnScreenButton.BUTTON_MOTION,
								(int) e.getX(j), (int) e.getY(j), e.getPointerId(j));
					}
				}
				break;
				
			}
		} catch (Throwable x) {
			Log.f(TAG, "Error in touch handler", x);
		}
		return true;
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
				mRCtx.onDrawFrame();
			} catch (Throwable e) {
				Log.f(TAG, "Rendering exception", e);
			}
		}

		/**
		 * @see android.opengl.GLSurfaceView.Renderer#onSurfaceChanged(
		 * 		javax.microedition.khronos.opengles.GL10, int, int)
		 */
		@Override
		public void onSurfaceChanged(GL10 gl, int w, int h) {
			Log.d(TAG, "onSurfaceChanged: " + w + "x" + h);
			try {
				/*
				 * Change to this to ignore spurious orientation changes
				if ((w != mW || h != mH) && (w != mH || h != mW))
				 */
				if (w != mW || h != mH)
				{
					Log.d(TAG, "Surface resized to " + w + "x" + h);
					mRCtx.updateSize(w, h);
					mRCtx.preRequestRender(RenderContext.REASON_RESIZE);
				}
			} catch (Throwable e) {
				Log.f(TAG, "Exception in onSurfaceChanged", e);
			}
		}
		
		/**
		 * @see android.opengl.GLSurfaceView.Renderer#onSurfaceCreated(
		 * 		javax.microedition.khronos.opengles.GL10,
		 * 		javax.microedition.khronos.egl.EGLConfig)
		 */
		@Override
		public void onSurfaceCreated(GL10 gl, EGLConfig config) {
			try {
				mW = mGlView.getWidth();
				mH = mGlView.getHeight();
				Log.d(TAG, "onSurfaceCreated: " + mW + "x" + mH);
				mRCtx = new AndroidGles1RenderContext(mGlView, gl, mMgr.mScreen);
				mRCtx.preRequestRender(RenderContext.REASON_INIT);
				mMgr.setRenderContext(mRCtx);
				mMgr.resume();
			} catch (Throwable e) {
				Log.f(TAG, "Exception in onSurfaceCreated", e);
			}
		}
	}
	
	/**
	 * @see uk.co.realh.hgame.ScreenButtonSource#addOnScreenButton(uk.co.realh.hgame.OnScreenButton)
	 */
	@Override
	public void addOnScreenButton(OnScreenButton btn) {
		mButtons[mNButtons++] = btn;
	}
	
	/**
	 * @see uk.co.realh.hgame.ScreenButtonSource#removeButtons()
	 */
	@Override
	public void removeButtons()
	{
		for (int n = 0; n < mNButtons; ++n)
			mButtons[n] = null;
		mNButtons = 0;
	}

    public AndroidKbdDInput getKbdDInput()
    {
        return mKbdDInput;
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event)
    {
        return mKbdDInput.onKeyDown(keyCode, event);
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event)
    {
        return mKbdDInput.onKeyUp(keyCode, event);
    }
}
