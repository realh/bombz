/*
 * Copyright (c) 2012, Tony Houghton <h@realh.co.uk>
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

package uk.co.realh.hgame.android;

import java.io.FileNotFoundException;
import java.io.IOException;

import android.app.Activity;
import android.content.res.AssetFileDescriptor;
import android.media.AudioManager;
import android.media.SoundPool;
import uk.co.realh.hgame.AudioContext;
import uk.co.realh.hgame.Log;

/**
 * 
 */
public class AndroidAudioContext implements AudioContext
{
	private final static String TAG = "Audio";
	
	public final static int CONCURRENT_SAMPLES = 20;
	
	private SoundPool mSoundPool;
	private AndroidSys mSys;
	
	public AndroidAudioContext(Activity activity, AndroidSys sys)
	{
		mSys = sys;
		activity.setVolumeControlStream(AudioManager.STREAM_MUSIC);
		mSoundPool = new SoundPool(CONCURRENT_SAMPLES,
				AudioManager.STREAM_MUSIC, 0);
	}
	
	@Override
	public int loadEffect(String leafname)
	{
		leafname = "sounds/" + leafname;
		try {
			try {
				AssetFileDescriptor fd;
				fd = mSys.getAssets().openFd("sounds/" + leafname);
				return mSoundPool.load(fd, 1);
			} catch (FileNotFoundException e) {
				return mSoundPool.load(mSys.getSDCardFilename(leafname,
						false), 1);
			}
		} catch (IOException e) {
			Log.w(TAG, "Unable to load " + leafname, e);
			return -1;
		}
	}

	@Override
	public void playEffect(int sampleId, float balance)
	{
		if (sampleId == -1)
			return;
		float leftVolume = 1.0f - balance;
		float rightVolume = balance;
		if (leftVolume > rightVolume) {
			rightVolume /= leftVolume;
			leftVolume = 1.0f;
		} else {
			leftVolume /= rightVolume;
			rightVolume = 1.0f;
		}
		mSoundPool.play(sampleId, leftVolume, rightVolume, 0, 0, 1);
	}

	@Override
	public void unloadEffect(int sampleId)
	{
		if (sampleId != -1)
			mSoundPool.unload(sampleId);
	}

	@Override
	public void dispose()
	{
		mSoundPool.release();
		mSoundPool = null;
	}
}
