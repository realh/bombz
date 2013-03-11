package uk.co.realh.hgame;
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

/**
 * A sort of bridge between game-specific logic and platform-specific code.
 * @author Tony Houghton
 *
 */
public class GameManager {
	
	protected static String TAG = "GameManager";
	
	// Bad Java: public fields for speed
	public RenderContext mRCtx;
	public Screen mScreen;
	public final Sys mSys;
	public final ScreenButtonSource mButtons;
	
	private volatile boolean mRunning;
	private volatile boolean mTicking;
	private GameThread mGameThread;
	private long mTickInterval;
	private volatile long mLastTapTime;
	private volatile long mBlankTimeout;
	private volatile boolean mBlankDisabled;
	
	/**
	 * @param sys	The System object for this platform.
	 * @param sbs 	Buttons source
	 * @see ScreenButtonSource
	 */
	public GameManager(Sys sys, ScreenButtonSource sbs)
	{
		mSys = sys;
		mButtons = sbs;
	}
	
	/**
	 * Indicate there has been user activity, keep screen blanker disabled.
	 */
	public void resetBlankTimeout()
	{
		mLastTapTime = System.nanoTime();
		if (!mBlankDisabled && mBlankTimeout > 0)
		{
			Log.d(TAG, "Disabling screen blanker");
			mBlankDisabled = true;
			mSys.disableScreenBlanker();
		}
	}
	
	/**
	 * How long to wait idle before enabling screenblanker,
	 * 0 for system default.
	 * @param timeout	In ns
	 */
	public void setScreenBlankTimeout(long timeout)
	{
		mBlankTimeout = timeout;
		if (timeout > 0)
		{
			resetBlankTimeout();
		}
		else
		{
			mBlankDisabled = false;
			mSys.enableScreenBlanker();
		}
	}

	/**
	 * Sets the current screen for running the game.
	 * 
	 * @param scrn
	 */
	public void setScreen(Screen scrn)
	{
		disableTicks();
		mButtons.removeButtons();
		mScreen = scrn;
		if (mRCtx != null)
		{
			mRCtx.setRenderer(scrn);
		}
		if (mRunning && scrn != null)
		{
			Event.pushEvent(Event.newEvent(Event.RESUME));
		}
		resetBlankTimeout();
	}
	
	/**
	 * Sets the RenderContext.
	 * 
	 * @param rctx
	 */
	synchronized
	public void setRenderContext(RenderContext rctx)
	{
		mRCtx = rctx;
		mRCtx.setRenderer(mScreen);
	}
	
	/**
	 * Lets the Screen know that it's to continue after a pause
	 * or that it should start.
	 */
	public void resume()
	{
		resetBlankTimeout();
		mGameThread = new GameThread();
		mRunning = true;
		mGameThread.start();
		Event.pushEvent(Event.newEvent(Event.RESUME));
	}
	
	/**
	 * Asks the Screen thread to stop if it's running, and waits for it.
	 */
	public void suspend()
	{
		mRunning = false;
		disableTicks();
		if (null != mGameThread) {
			Event.pushEvent(Event.newEvent(Event.PAUSE));
			try {
				mGameThread.join();
			} catch (InterruptedException e) {
				Log.w(TAG, "Interrupted waiting for " +
						"game thread to stop", e);
			}
			mGameThread = null;
		}
	}
	
	/**
	 * Causes tick events to be sent at regular intervals.
	 * On a PAUSE ticks will be disabled, and Screen must enable them 
	 * again on RESUME if desired.
	 * 
	 * @param interval	Interval between ticks in ms.
	 */
	public synchronized void enableTicks(int interval)
	{
		mTickInterval = (long) interval * 1000000;
		mTicking = true;
		Event.pushEvent(Event.newEvent(Event.TICK));
	}
	
	public synchronized void disableTicks()
	{
		mTicking = false;
	}
	
	private class GameThread extends Thread {
		
		@Override
		public void run()
		{
			long lastTick = System.nanoTime();
			try {
				boolean running = true;
				while (running)
				{
					Event ev = null;
					long interval = -1;
					
					if (mTicking) {
						long now = System.nanoTime();
						interval = mTickInterval - (now - lastTick);
						if (interval < -mTickInterval) {
							/* Drop ticks if it looks like we've hit a slow
							 * patch.
							 */
							lastTick = now;
						}
						if (interval < 0)
							interval = 0;
						ev = Event.popEvent(interval);
					} else {
						ev = Event.popEvent();
					}
					if (null == ev || ev.mCode == Event.TICK) {
						if (null == ev) {
							lastTick += mTickInterval;
						} else {
							lastTick = System.nanoTime();
						}
						if (mTicking)
							mScreen.step();
					} else {
						if (ev.mCode == Event.RESUME)
							mRCtx.waitUntilReady();
						mScreen.handleEvent(ev);
					}
					if (null != ev && ev.mCode == Event.PAUSE)
						running = false;
					if (mBlankDisabled && 0 < mBlankTimeout &&
							System.nanoTime() - mLastTapTime > mBlankTimeout)
					{
						Log.d(TAG, "Idle for " + mBlankTimeout / 1000000000l +
								"s, enabling screen blanker");
						mSys.enableScreenBlanker();
						mBlankDisabled = false;
					}
				}
			} catch (Throwable e) {
				Log.f(TAG, "Error in game thread", e);
			}
		}
	}
	
	/**
	 * Forwards back button events to screen.
	 * 
	 * @return	Whether screen handled event.
	 */
	public boolean onBackPressed() {
		if (null != mScreen)
			return mScreen.onBackPressed();
		else
			return false;
	}
}
