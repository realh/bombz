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

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;

/**
 * Absracts common OS services.
 *
 * @author Tony Houghton
 */
public interface Sys {
	
	/**
	 * Builds an absolute pathname for storing settings etc.
	 * 
	 * @param leafname	File's leafname
	 * @return			File's pathname
	 */
	public String getProfileFilename(String leafname);
	
	/**
	 * Opens a file in app's assets.
	 * 
	 * @param leafname
	 * @return Open file (read-only)
	 * @throws FileNotFoundException 
	 * @throws IOException 
	 */
	public InputStream openAsset(String leafname) throws IOException;
	
	/**
	 * Loads a PNG as an Image.
	 * 
	 * @param fd	Open file stream
	 * @param desc	Description in case of exception; you could use leafname.
	 * @return 		An image
	 */
	public Image loadPNG(InputStream fd, String desc);
	
	/**
	 * Loads some font appropriate for the game & platform.
	 * @param px	Font size in pixels
	 * @return		A Font
	 */
	public Font openFont(int px);
	
	/**
	 * Lists the contents of a folder in app's assets.
	 * 
	 * @return Array of filenames.
	 * @throws IOException 
	 */
	public String[] listFolder(String folder) throws IOException;
	
	/**
	 * Gets a translated string for a given tag.
	 * 
	 * @param tag
	 * @return
	 */
	public String translate(String tag);
	
	/**
	 * Create a settings object.
	 * 
	 * @return
	 */
	public Settings createSettings();

}
