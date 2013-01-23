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

import java.util.ArrayList;
import java.util.List;

/**
 * A pool of reusable objects to avoid gc.
 * 
 * @author Tony Houghton
 *
 */
public class Pool<T> {
	
	/**
	 * Creates the objects to populate the pool.
	 * 
	 * @author Tony Houghton
	 *
	 * @param <T>	Type of a reusable object
	 */
	public interface PoolFactory<T> {
		public T createForPool();
	}

	private final PoolFactory<T> mFactory;
	private final int mCapacity;
	private final List<T> mFreePool;
	
	/**
	 * Makes a Pool capable of holding capacity objects.
	 * 
	 * @param factory
	 * @param capacity
	 */
	public Pool(PoolFactory<T> factory, int capacity)
	{
		mFactory = factory;
		mCapacity = capacity;
		mFreePool = new ArrayList<T>(capacity);
	}
	
	/**
	 * @return	A reusable T, recycled from pool if possible.
	 */
	public T getObject()
	{
		int s = mFreePool.size();
		if (s == 0)
			return mFactory.createForPool();
		else
			return mFreePool.remove(s - 1);
	}
	
	/**
	 * Returns an object to the free pool for reuse.
	 * 
	 * @param Object
	 */
	public void free(T obj)
	{
		if (mFreePool.size() < mCapacity)
			mFreePool.add(obj);
		else
			Log.w("Pool", "Pool capacity exceeded");
	}
}
