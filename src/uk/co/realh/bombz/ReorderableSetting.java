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

import uk.co.realh.bombz.Reorder.Datum;
import uk.co.realh.hgame.SavedSettings;

/**
 * I can't get Java's crappy generics to work here, so use String to save
 * writing the whole thing twice for int and float.
 * 
 * @author Tony Houghton
 *
 */
public class ReorderableSetting implements Reorder.Dataset {

	protected static class SettingDatum implements Reorder.Datum {
		public String mValue;
		public SettingDatum(String v) {
			mValue = v;
		}
	}
	
	protected SavedSettings mSettings;
	private String mKey;
	private String mDefault;
	
	/**
	 * 
	 * @param settings		The settings to manipulate
	 * @param key			key prefix for the setting of interest
	 * @param defaultVal	Default value for this setting
	 */
	public ReorderableSetting(SavedSettings settings,
			String key, String defaultVal) {
		mSettings = settings;
		mKey = key;
		mDefault = defaultVal;
	}
	
	/**
	 * @see uk.co.realh.bombz.Reorder.Dataset#get(int)
	 */
	@Override
	public Datum get(int n) {
		return new SettingDatum(mSettings.get(mKey + (n + 1), mDefault));
	}

	/**
	 * @see uk.co.realh.bombz.Reorder.Dataset#set(int, uk.co.realh.bombz.Reorder.Datum)
	 */
	@Override
	public void set(int n, Datum d) {
		mSettings.set(mKey + (n + 1), ((SettingDatum) d).mValue);
	}
	
}
