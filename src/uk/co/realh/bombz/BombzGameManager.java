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

import uk.co.realh.hgame.ButtonFeedback;
import uk.co.realh.hgame.GameManager;
import uk.co.realh.hgame.Log;
import uk.co.realh.hgame.SavedSettings;
import uk.co.realh.hgame.ScreenButtonSource;
import uk.co.realh.hgame.Sys;

/**
 * @author Tony Houghton
 *
 */
public class BombzGameManager extends GameManager {
	
	BombzTextures mTextures;
	SavedSettings mSavedGame;
	SavedSettings mConfiguration;
	Stats mStats;
	
	int mCurrentLevel;
	
	private BombzPauseScreen mBombzPauseScreen;
	private BombzMasterMenuScreen mMainMenuScreen;
	private BombzGameScreen mGameScreen;
	private ChooseLevelScreen mChooseLevelScreen;
	private final ButtonFeedback mHapticFeedback;

	/**
	 * @param sys
	 * @param sbs	Object for managing on-screen buttons
	 * @throws IOException
	 * @see uk.co.realh.hgame.ScreenButtonSource
	 */
	public BombzGameManager(Sys sys, ScreenButtonSource sbs) throws IOException
	{
		super(sys, sbs);
		mSavedGame = sys.getSavedSettings("saves");
		mConfiguration = sys.getSavedSettings("config");
		mStats = new Stats(sys.getSavedSettings("stats"));
		mHapticFeedback = sys.getHapticFeedback();
		mTextures = new BombzTextures(sys,
				mConfiguration.get("touchpad", K.CONTROL_VPAD_LEFT));
		mCurrentLevel = mSavedGame.get("level", 1);
		mMainMenuScreen = new BombzMasterMenuScreen(this);
		setScreen(mMainMenuScreen);
	}
	
	public BombzMasterMenuScreen getMasterMenuScreen()
	{
		// Created in c'tor so always exists
		return mMainMenuScreen;
	}
	
	public BombzGameScreen getGameScreen() throws IOException
	{
		if (null == mGameScreen)
			mGameScreen = new BombzGameScreen(this, mButtons, mHapticFeedback);
		return mGameScreen;
	}
	
	public ChooseLevelScreen getChooseLevelScreen()
	{
		if (null == mGameScreen)
			mChooseLevelScreen = new ChooseLevelScreen(this);
		return mChooseLevelScreen;
	}
	
	public BombzPauseScreen getPauseScreen()
	{
		if (null == mBombzPauseScreen)
			mBombzPauseScreen = new BombzPauseScreen(this);
		return mBombzPauseScreen;
	}
	
	void completedLevel()
	{
		int secs = mGameScreen.mTimeLimit.getTimeLeft();
		mStats.succeeded(secs, mGameScreen.mLevel.countDetonators());
		int score;
		if (secs > 10)
			score = 3;
		else if (secs > 0)
			score = 2;
		else
			score = 1;
		int old_score = mSavedGame.get("score_" + mCurrentLevel, 0);
		if (score > old_score)
			mSavedGame.set("score_" + mCurrentLevel, score);
		mCurrentLevel += 1;
		mSavedGame.set("level", mCurrentLevel);
		try {
			mSavedGame.save();
		} catch (IOException e) {
			Log.e(TAG, "Unable to save progress", e);
		}
    	setScreen(getMasterMenuScreen());
	}
	
}
