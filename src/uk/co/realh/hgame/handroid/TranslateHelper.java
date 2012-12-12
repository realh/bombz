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

// HGame - a simple cross-platform game framework

// TranslateHelper - loads strings from app's R class into
//					 native Translate object


package uk.co.realh.hgame.handroid;

import java.lang.reflect.Field;

import android.app.Activity;

public class TranslateHelper {
	/**
	 * @param act		Activity
	 * @param nobj_addr Address of native Translate object. int would do
	 * 					for now, but Android may go 64-bit one day
	 * @param pkg_name  Name of package containing R
	 * @throws ClassNotFoundException 
	 * @throws IllegalAccessException 
	 * @throws IllegalArgumentException 
	 */
	public TranslateHelper(Activity act, long nobj_addr, String pkg_name)
			throws ClassNotFoundException,
			IllegalArgumentException, IllegalAccessException
	{
		Class<?> r = Class.forName(pkg_name + ".R.strings");
		Field[] fields = r.getDeclaredFields();
		for (Field f : fields)
		{
			addKeyValue(nobj_addr, f.getName().replace('_', ' '),
					act.getString(f.getInt(null)));
		}
	}
	
	public native void addKeyValue(long nobj_addr,
			String key, String value);
	
}
