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
	
	private volatile boolean mRunning;
	private volatile boolean mTicking;
	private GameThread mGameThread;
	private TickThread mTickThread;
	
	/**
	 * @param sys	The System object for this platform.
	 */
	public GameManager(Sys sys)
	{
		mSys = sys;
	}

	/**
	 * Sets the current screen for running the game.
	 * 
	 * @param scrn
	 */
	public void setScreen(Screen scrn)
	{
		disableTicks();
		mScreen = scrn;
		if (mRCtx != null)
			mRCtx.setRenderer(scrn);
		if (mRunning && scrn != null)
			Event.pushEvent(Event.newEvent(Event.RESUME));
	}
	
	/**
	 * Sets the RenderContext.
	 * 
	 * @param rctx
	 */
	public void setRenderContext(RenderContext rctx)
	{
		mRCtx = rctx;
		if (mScreen != null)
			mRCtx.setRenderer(mScreen);
	}
	
	/**
	 * Lets the Screen know that it's to continue after a pause
	 * or that it should start.
	 */
	public void resume()
	{
		mGameThread = new GameThread();
		mRunning = true;
		mGameThread.start();
		Event ev = Event.newEvent(Event.RESUME);
		synchronized(ev) {
			Event.pushEvent(ev);
			try {
				ev.wait();
			} catch (InterruptedException e) {
				Log.w(TAG, "Interrupted waiting for " +
						"resume event to be handled", e);
			}
		}
	}
	
	/**
	 * Asks the Screen thread to stop.
	 */
	public void suspend()
	{
		mRunning = false;
		Event.pushEvent(Event.newEvent(Event.PAUSE));
		try {
			mGameThread.join();
		} catch (InterruptedException e) {
			Log.w(TAG, "Interrupted waiting for " +
					"game thread to stop", e);
		}
	}
	
	/**
	 * Causes tick events to be sent at regular intervals.
	 * After a PAUSE ticks will be disabled, and Screen must enable them 
	 * again on RESUME if desired.
	 * 
	 * @param interval	Interval between ticks in ms.
	 */
	public synchronized void enableTicks(int interval)
	{
		if (!mTicking)
		{
			mTicking = true;
			mTickThread = new TickThread(interval);
			mTickThread.start();
		}
	}
	
	public synchronized void disableTicks()
	{
		mTicking = false;
		if (mTickThread != null)
		{
			mTickThread.stopTicking();
			mTickThread = null;
		}
	}
	
	private class GameThread extends Thread {
		
		@Override
		public void run()
		{
			boolean running = true;
			while (running)
			{
				Event ev = Event.popEvent();
				if (ev.mCode == Event.TICK)
				{
					if (mTicking)
						mScreen.step();
				}
				else
				{
					mScreen.handleEvent(ev);
				}
				if (ev.mCode == Event.RESUME)
				{
					synchronized(ev) {
						ev.notifyAll();
					}
				}
				else if (ev.mCode == Event.PAUSE)
					running = false;
			}
		}
	}
	
	private class TickThread extends Thread {
		
		private volatile boolean mEnabled;
		private int mInterval;
		
		TickThread(int interval)
		{
			mInterval = interval;
		}
		
		@Override
		public void run()
		{
			mEnabled = true;
			while (true)
			{
				synchronized(this) {
					if (!mEnabled)
						break;
					Event.pushEvent(Event.newEvent(Event.TICK));
				}
				try {
					sleep(mInterval);
				} catch (InterruptedException e) {
					Log.w(TAG, "Tick thread interrupted", e);
					mEnabled = false;
				}
			}
		}
		
		public synchronized void stopTicking()
		{
			mEnabled = false;
		}
	}
	
}
