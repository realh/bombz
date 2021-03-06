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

import uk.co.realh.hgame.AudioContext;
import uk.co.realh.hgame.AudioContext.SampleHandle;
import uk.co.realh.hgame.ButtonFeedback;
import uk.co.realh.hgame.GameManager;
import uk.co.realh.hgame.Log;
import uk.co.realh.hgame.SavedSettings;
import uk.co.realh.hgame.Screen;
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
	
	SampleHandle mBangSample, mFuseSample, mMatchSample,
			mPip1Sample, mPip2Sample;
	
	private BombzPauseScreen mBombzPauseScreen;
	private BombzMasterMenuScreen mMainMenuScreen;
	private BombzGameScreen mGameScreen;
	private ChooseLevelScreen mChooseLevelScreen;
	private ControlMenuScreen mControlMenuScreen;
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
		SavedSettings stats = sys.getSavedSettings("stats");
		int ov = stats.get("version", Stats.VERSION);
		if (ov != Stats.VERSION)
			new LevelOrder(mSavedGame, stats, ov);
		mStats = new Stats(stats);
		mHapticFeedback = sys.getHapticFeedback();
		mTextures = new BombzTextures(sys);
		mCurrentLevel = mSavedGame.get("level", 1);
		mMainMenuScreen = new BombzMasterMenuScreen(this);
		setScreen(mMainMenuScreen);
	}
	
	@Override
	public void setAudio(AudioContext audio)
	{
		super.setAudio(audio);
		loadSounds();
	}
	
	public BombzMasterMenuScreen getMasterMenuScreen()
	{
		// Created in c'tor so always exists
		return mMainMenuScreen;
	}
	
	public BombzGameScreen getGameScreen() throws IOException
	{
		if (null == mGameScreen)
			mGameScreen = new BombzGameScreen(this, mButtons, mHapticFeedback,
                    mSys.getKbdDInput());
		return mGameScreen;
	}
	
	public ChooseLevelScreen getChooseLevelScreen()
	{
		if (null == mChooseLevelScreen)
			mChooseLevelScreen = new ChooseLevelScreen(this);
		return mChooseLevelScreen;
	}
	
	public ControlMenuScreen getControlMenuScreen()
	{
		if (null == mControlMenuScreen)
			mControlMenuScreen = new ControlMenuScreen(this);
		return mControlMenuScreen;
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
		int highest = mSavedGame.get("highest_completed", 0);
		int next = mCurrentLevel;
		do {
			++next;
		} while (mSavedGame.get("score_" + next, 0) > 0);
		if (next >= highest)
			mSavedGame.set("highest_completed", next - 1);
		setCurrentLevel(next);
    	setScreen(getMasterMenuScreen());	// Causes save().
	}
	
	void setCurrentLevel(int level) {
		mCurrentLevel = level;
		mSavedGame.set("level", level);
		try {
			mSavedGame.save();
		} catch (IOException e) {
			Log.w(TAG, "Unable to save game state");
		}
	}

	/**
	 * @param newScreen		New screen
	 * @param backScreen	Screen to show when Back is pressed on newScreen
	 */
	public void setScreen(BombzMenuScreen newScreen, Screen backScreen) {
		super.setScreen(newScreen);
		newScreen.mBackScreen = backScreen;
	}
	
	public void setScreen(BombzMenuScreen newScreen) {
		super.setScreen(newScreen);
		newScreen.mBackScreen = null;
	}
	
	public void loadSounds() {
		mBangSample = mAudio.loadEffect("bomb.ogg");
		mFuseSample = mAudio.loadEffect("fuse.ogg");
		mMatchSample = mAudio.loadEffect("match.ogg");
		mPip1Sample = mAudio.loadEffect("pip1.ogg");
		mPip2Sample = mAudio.loadEffect("pip2.ogg");
	}
	
}
