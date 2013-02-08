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

import uk.co.realh.hgame.GameManager;
import uk.co.realh.hgame.Log;
import uk.co.realh.hgame.SavedSettings;
import uk.co.realh.hgame.Screen;
import uk.co.realh.hgame.Sys;

/**
 * @author Tony Houghton
 *
 */
public class BombzGameManager extends GameManager {
	
	BombzTextures mTextures;
	SavedSettings mSavedGame;
	SavedSettings mConfiguration;
	
	private BombzMainMenuScreen mMainMenuScreen;
	private BombzGameScreen mGameScreen;

	/**
	 * @param sys
	 * @throws IOException 
	 */
	public BombzGameManager(Sys sys) throws IOException {
		super(sys);
		mSavedGame = sys.getSavedSettings("saves");
		mConfiguration = sys.getSavedSettings("config");
		mTextures = new BombzTextures(sys,
				mConfiguration.get("touchpad", K.CONTROL_VPAD_LEFT));
		mMainMenuScreen = new BombzMainMenuScreen(this);
		setScreen(mMainMenuScreen);
	}
	
	public Screen getMainMenuScreen()
	{
		// Created in c'tor so always exists
		return mMainMenuScreen;
	}
	
	public Screen getGameScreen() throws IOException
	{
		if (null == mGameScreen)
			mGameScreen = new BombzGameScreen(this);
		return mGameScreen;
	}
}
