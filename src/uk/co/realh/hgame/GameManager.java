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
	
	private static String TAG = "GameManager";
	
	// Bad Java: public fields for speed
	public RenderContext mRCtx;
	public Screen mScreen;
	public final Sys mSys;
	
	private volatile boolean mRunning;
	private volatile boolean mTicking;
	private volatile int mTickPeriod;
	private Thread mGameThread;
	private Thread mTickThread;
	
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
		mScreen = scrn;
		mRCtx.setRenderer(scrn);
		if (mRunning && scrn != null)
			Event.pushEvent(Event.newEvent(Event.RESUME));
	}
	
	/**
	 * Lets the Screen know that it's to continue after a pause
	 * or that it should start.
	 */
	public void resume()
	{
		mRunning = true;
		Event.pushEvent(Event.newEvent(Event.RESUME));
	}
	
	/**
	 * Lets the Screen know that it should suspend its operation.
	 */
	public void suspend()
	{
		mRunning = false;
		Event ev = Event.newEvent(Event.PAUSE);
		synchronized(ev) {
			Event.pushEvent(ev);
			try {
				ev.wait();
			} catch (InterruptedException e) {
				Log.w(TAG, "Interrupted waiting for " +
						"suspend event to be handled", e);
			}
		}
	}
	
	/**
	 * Starts execution of Screen, starting with a RESUME event.
	 */
	public void start()
	{
		resume();
		mGameThread = new GameThread();
		mGameThread.start();
	}
	
	/**
	 * Asks the Screen thread to stop.
	 */
	public void stop()
	{
		Event.pushEvent(Event.newEvent(Event.STOP));
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
		mTickPeriod = interval;
		if (!mTicking)
		{
			mTicking = true;
			mTickThread = new TickThread();
			mTickThread.start();
		}
	}
	
	public synchronized void disableTicks()
	{
		mTicking = false;
		if (mTickThread != null)
		{
			try {
				mTickThread.join();
			} catch (InterruptedException e) {
				Log.w(TAG, "interrupted waiting for tick thread to stop", e);
			}
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
				mScreen.handleEvent(ev);
				if (ev.mCode == Event.PAUSE)
				{
					synchronized(ev) {
						ev.notifyAll();
					}
				}
				else if (ev.mCode == Event.STOP)
					running = false;
			}
		}
	}
	
	private class TickThread extends Thread {
		
		@Override
		public void run()
		{
			while (mTicking)
			{
				Event.pushEvent(Event.newEvent(Event.TICK));
				try {
					sleep(mTickPeriod);
				} catch (InterruptedException e) {
					Log.w(TAG, "Tick thread interrupted", e);
					mTicking = false;
				}
			}
		}
	}
	
}
