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

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

/**
 * Very simple key-value file management.
 * 
 * @author Tony Houghton
 */
public abstract class SavedSettings {
	
	protected Map<String, String> mMap = new HashMap<String, String>();
	
	/**
	 * Called from constructor to parse an exisiting file.
	 * 
	 * @param reader
	 * @throws IOException
	 */
	protected void load(BufferedReader reader) throws IOException {
		while (true) {
			String s = reader.readLine();
			if (null == s)
				break;
			int eq = s.indexOf('=');
			mMap.put(s.substring(0, eq), s.substring(eq + 1));
		}
	}
	
	/**
	 * Called by save() implementation to save settings to a file.
	 * 
	 * @param writer
	 * @throws IOException
	 */
	protected void save(BufferedWriter writer) throws IOException {
		for (Map.Entry<String, String> e: mMap.entrySet()) {
			writer.write(e.getKey());
			writer.write('=');
			writer.write(e.getValue());
			writer.newLine();
		}
	}
	
	/**
	 * Save settings to an implementation-defined file.
	 * 
	 * @throws IOException 
	 */
	public abstract void save() throws IOException;
	
	/**
	 * Set a string value.
	 * 
	 * @param k		Key
	 * @param v		Value
	 */
	public void set(String k, String v) {
		mMap.put(k, v);
	}

	/**
	 * Set a value for a non-string type.
	 * 
	 * @param k		Key
	 * @param v		Value
	 */
	public <T> void set(String k, T v) {
		mMap.put(k, String.valueOf(v));
	}

	/**
	 * Get a string value.
	 * 
	 * @param k		Key
	 * @param v		Default value if not already set
	 * @return		Value
	 */
	public String get(String k, String v) {
		if (mMap.containsKey(k))
			return mMap.get(k);
		else
			return v;
	}
	
	/**
	 * Get an int value.
	 * 
	 * @param k		Key
	 * @param v		Default value if not already set
	 * @return		Value
	 */
	public int get(String k, int v) {
		if (mMap.containsKey(k))
			return Integer.parseInt(mMap.get(k));
		else
			return v;
	}
	
	/**
	 * Get a float value.
	 * 
	 * @param k		Key
	 * @param v		Default value if not already set
	 * @return		Value
	 */
	public float get(String k, float v) {
		if (mMap.containsKey(k))
			return Float.parseFloat(mMap.get(k));
		else
			return v;
	}
	
}
