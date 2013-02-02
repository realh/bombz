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

import java.util.concurrent.ArrayBlockingQueue;

/**
 * @author Tony Houghton
 *
 */
public class Event {
	
	private static final String TAG = "Event";
	
	/**
	 * Codes of commonly used events.
	 */
	public static final int TICK = getQuark("TICK");
	public static final int PAUSE = getQuark("PAUS");
	public static final int RESUME = getQuark("RESM");
	public static final int TAP = getQuark("TAP_");
	public static final int INTERRUPT = getQuark("INTR");
	
	/**
	 * Generates an integer from a 4-character string. As long as the strings
	 * are ASCII and unique the codes will be unique.
	 * 
	 * @param s		4 character string
	 * @return		Integer code for the string
	 */
	public static int getQuark(String s)
	{
		return (s.codePointAt(0) << 24) | (s.codePointAt(1) << 16) |
				(s.codePointAt(2) << 8) | s.codePointAt(3);
	}
	
	/**
	 * The quark code for this event. Bad Java but fast.
	 */
	public int mCode;
	public int mDatum1, mDatum2;
	
	/**
	 * Creates a new or recycled Event.
	 * 
	 * @param code
	 * @return
	 */
	public static Event newEvent(int code)
	{
		Event ev = smPool.getObject();
		ev.mCode = code;
		return ev;
	}
	
	/**
	 * Creates a new or recycled Event.
	 * 
	 * @param code
	 * @param d1	Extra data for event
	 * @param d2	Extra data for event
	 * @return
	 */
	public static Event newEvent(int code, int d1, int d2)
	{
		Event ev = smPool.getObject();
		ev.mCode = code;
		ev.mDatum1 = d1;
		ev.mDatum2 = d2;
		return ev;
	}
	
	/**
	 * Returns this event to the pool for recycling.
	 * 
	 */
	public void free()
	{
		smPool.free(this);
	}
	
	/**
	 * Enforces use of createEvent.
	 */
	private Event()
	{
		mCode = 0;
	}
	
	/**
	 * Pushes an event onto the queue.
	 * 
	 * @param ev
	 */
	public static void pushEvent(Event ev)
	{
		try {
			smQueue.put(ev);
		} catch (InterruptedException e) {
			Log.e(TAG, "Event push interrupted", e);
		}
	}
	
	/**
	 * Gets event at head of queue, blocking if one isn't available yet.
	 * 
	 * @return
	 */
	public static Event popEvent()
	{
		try {
			return smQueue.take();
		} catch (InterruptedException e) {
			Log.e(TAG, "Event pop interrupted", e);
			return newEvent(INTERRUPT);
		}
	}
	
	private static final int MAX_EVENTS = 32;
	
	private static class Factory implements Pool.PoolFactory<Event> {
		@Override
		public Event createForPool()
		{
			return new Event();
		}
	}
	private static final Factory smFactory = new Factory();
	private static final Pool<Event> smPool =
			new Pool<Event>(smFactory, MAX_EVENTS);
	
	private static ArrayBlockingQueue<Event> smQueue =
			new ArrayBlockingQueue<Event>(MAX_EVENTS);
}
