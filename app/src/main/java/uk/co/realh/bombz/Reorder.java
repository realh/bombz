package uk.co.realh.bombz;
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
 * @author Tony Houghton
 *
 */
public class Reorder {
	
	/**
	 * Completely abstract representation of a member of a Dataset.
	 * @author Tony Houghton
	 */
	public static interface Datum {};
	
	/**
	 * Accesses a dataset for reordering.
	 * @author Tony Houghton
	 *
	 * ** Indices start from 0, despite newOrder starting from 1 **
	 */
	public static interface Dataset {
		public Datum get(int n);
		public void set(int n, Datum d);
	}
	
	private Dataset mSet;

	/**
	 * 
	 */
	public Reorder(Dataset set) {
		mSet = set;
	}
	
	/**
	 * Does the reordering. No test is done to ensure that newOrder and Dataset
	 * have the same length, so use with care.
	 * 
	 * @param newOrder	Each element corresponds to an element at the same
	 * 					position in the Dataset, and the value in newOrder
	 * 					is the position it should be moved to.
	 * 					*** Indices start from 1, not 0. ***
	 */
	void reorder(int[] newOrder) {
		Datum[] newData = new Datum[newOrder.length];
		for (int n = 0; n < newOrder.length; ++n) {
			newData[newOrder[n] - 1] = mSet.get(n);
		}
		for (int n = 0; n < newOrder.length; ++n) {
			mSet.set(n, newData[n]);
		}
	}

}
