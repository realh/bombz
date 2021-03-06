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

/**
 * Various constants for the game.
 */
public class K
{
	static final int FUSE_TICKS = 60;
	static final int FRUSTUM_TILE_SIZE = 32;
	static final int N_COLUMNS = 20;
	static final int N_ROWS = 15;
	static final int DELTA_PIX = 4;		// How many frustum units
										// hero moves each step
	static final int TICK_INTERVAL = 40;
	static final int TILE_ATLAS_COLUMNS = 6;

	static final int N_LEVELS = 42;

	static final int FACING_LEFT = 0;
	static final int FACING_UP = 1;
	static final int FACING_RIGHT = 2;
	static final int FACING_DOWN = 3;

	static final int MENU_TEXT_COLOUR = 0x000088;

	static final int CONTROL_NONE = 0;			// No touchscreen controls
	static final int CONTROL_VPAD_LEFT = 1;
	static final int CONTROL_VPAD_RIGHT = 2;
	static final int CONTROL_VBUTTONS_LEFT = 3;
	static final int CONTROL_VBUTTONS_RIGHT = 4;

	// Divisor under overall width and height
	static final int CONTROL_XPADDING = 40;
	static final int CONTROL_YPADDING = 24;

	// Size of main digits compared to tile size
	static final int DIGIT_MUL = 3;
	static final int DIGIT_DIV = 4;

	// How long to keep screen on for in ns
	static final long EXTENDED_IDLE = 180000000000l;

	// In units of tiles
	static final int CTRL_MENU_WIDTH = 5;
	static final int CTRL_MENU_HEIGHT = 3;
	
	static final float DPI_TO_VPAD = 320f / 256f;
}
