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

package uk.co.realh.bombz;

import java.io.IOException;

import uk.co.realh.hgame.Log;
import uk.co.realh.hgame.SavedSettings;

/**
 * Collects stats to help determine a better ordering of levels
 * @author Tony Houghton
 *
 */
public class Stats {
	
	private static final String TAG = "Stats";

	private SavedSettings mSettings;
	
	private int mCurrentLevel;
	private int mMoves;

	/**
	 * @param settings	For loading/saving
	 */
	public Stats(SavedSettings settings) {
		mSettings = settings;
	}
	
	public void startAttempt(int level, int detonators, int timeLimit) {
		mCurrentLevel = level;
		mSettings.set("time_limit_" + level, timeLimit);
		mMoves = 0;
	}
	
	/**
	 * A move is actually a change of direction.
	 * @param n	Number of moves made
	 */
	public void madeMoves(int n) {
		mMoves += n;
	}

	/**
	 * A move is actually a change of direction
	 */
	public void madeMove() {
		++mMoves;
	}
	
	public void failed() {
		incrementSetting("failed_");
		save();
	}
	
	public void succeeded(int timeLeft, int detonatorsLeft) {
		incrementSetting("succeeded_");
		addSuccessMeanStat("detonators_left_", detonatorsLeft); 
		addSuccessMeanStat("time_left_", timeLeft); 
		addSuccessMeanStat("moves_", mMoves); 
		save();
	}
	
	private void save() {
		mSettings.set("order_version", K.ORDER_VERSION);
		try {
			mSettings.save();
		} catch (IOException e) {
			Log.w(TAG, "Failed to save stats", e);
		}
	}
	
	private void incrementSetting(String s) {
		s += mCurrentLevel;
		mSettings.set(s, mSettings.get(s, 0) + 1);
	}
	
	private void addSuccessMeanStat(String s, int val) {
		s += mCurrentLevel;
		float mean = mSettings.get(s, 0.0f);
		float n = (float) mSettings.get("succeeded_" + mCurrentLevel, 1);
		if (0.0f == mean || 1 == n)
			mean = (float) val / (float) n;
		else
			mean = (mean * (n - 1) + (float) val) / n;
		mSettings.set(s, mean);
	}

}
