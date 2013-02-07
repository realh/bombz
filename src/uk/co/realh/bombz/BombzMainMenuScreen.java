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

import uk.co.realh.hgame.Event;
import uk.co.realh.hgame.Log;
import uk.co.realh.hgame.TapEventHandler;

/**
 * @author Tony Houghton
 *
 */
public class BombzMainMenuScreen extends BombzMenuScreen {

	private static final String TAG = "MainMenu";
	
	/**
	 * @param mgr
	 */
	public BombzMainMenuScreen(BombzGameManager mgr) {
		super(mgr);
		addTextWidget(mMgr.mSys.translate("Play"),
				new PlayTappedListener());
		addTextWidget(mMgr.mSys.translate("Choose_Level"),
				new ChooseLevelTappedListener());
		addTextWidget(mMgr.mSys.translate("Configure_Controls"),
				new ConfigureControlsTappedListener());
		addTextWidget(mMgr.mSys.translate("Other_Settings"),
				new OtherSettingsTappedListener());
	}

	/**
	 * @see uk.co.realh.hgame.Renderer#getDescription()
	 */
	@Override
	public String getDescription() {
		return "MainMenuScreen";
	}

	private class PlayTappedListener implements TapEventHandler {
		@Override
		public boolean handleTapEvent(Event e) {
			Log.d(TAG, "Play tapped");
			try {
				mMgr.setScreen(mMgr.getGameScreen());
			} catch (IOException x) {
				Log.e(TAG, "Unable to start game", x);
			}
			return true;
		}
	}

	private class ChooseLevelTappedListener implements TapEventHandler {
		@Override
		public boolean handleTapEvent(Event e) {
			Log.d(TAG, "Choose Level tapped");
			return true;
		}
	}
	
	private class ConfigureControlsTappedListener implements TapEventHandler
	{
		@Override
		public boolean handleTapEvent(Event e) {
			Log.d(TAG, "Configure Controls tapped");
			return true;
		}
	}
	
	private class OtherSettingsTappedListener implements TapEventHandler
	{
		@Override
		public boolean handleTapEvent(Event e) {
			Log.d(TAG, "Other Settings tapped");
			return true;
		}
	}
	
}
