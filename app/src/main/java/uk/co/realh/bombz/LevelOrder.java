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
 * Holds relationship between old and new level ordering.
 * @author Tony Houghton
 *
 */
public class LevelOrder {
	
	public final static int[] ORDER_1_3 = {
		12,  6,  7,  5,  3, 20, 11, 42,  4,  9,  1, 26, 21, 24, 13, 31, 28,
		 2, 41, 23, 25, 22, 40, 39, 33, 17, 15, 36, 18, 14, 34, 19, 35, 29,
		 8, 37, 32, 27, 16, 30, 38, 10
	};

	private static final String TAG = "LevelOrder";

    ReorderableSetting mScore;
    ReorderableSetting mDeto;
    ReorderableSetting mFailed;
    ReorderableSetting mMoves;
    ReorderableSetting mStarted;
    ReorderableSetting mSucceeded;
    ReorderableSetting mTimeLeft;
    ReorderableSetting mTimeLimit;

	LevelOrder(SavedSettings savedGame, SavedSettings stats, int oldVsn) {
        mScore = new ReorderableSetting(savedGame, "score_", "0");
        mDeto = new ReorderableSetting(stats, "detonators_left_", "0");
        mFailed = new ReorderableSetting(stats, "failed_", "0");
        mMoves = new ReorderableSetting(stats, "moves_", "0");
        mStarted = new ReorderableSetting(stats, "started_", "0");
        mSucceeded = new ReorderableSetting(stats, "succeeded_", "0");
        mTimeLeft = new ReorderableSetting(stats, "time_left_", "0");
        mTimeLimit = new ReorderableSetting(stats, "time_limit_", "0");
        
        reorder(ORDER_1_3);
        
		for (int n = 0; n < K.N_LEVELS; ++n) {
			if (n > 0 && savedGame.get("score_" + n, 0) == 0) {
				savedGame.set("highest_completed", n -1);
				savedGame.set("level", n -1);
				break;
			}
		}
		
		try {
			savedGame.save();
			stats.save();
		} catch (IOException e) {
			Log.e(TAG, "Unable to save data after update for level reorder", e);
		}
	}
	
	private void reorder(int[] order) {
        new Reorder(mScore).reorder(order);
        new Reorder(mDeto).reorder(order);
        new Reorder(mFailed).reorder(order);
        new Reorder(mMoves).reorder(order);
        new Reorder(mStarted).reorder(order);
        new Reorder(mSucceeded).reorder(order);
        new Reorder(mTimeLeft).reorder(order);
        new Reorder(mTimeLimit).reorder(order);
	}

}
