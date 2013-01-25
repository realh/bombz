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
 * Constants for the tiles/cells that make up a level.
 * There are few enough of these to fit in a byte but
 * sufficient enough to wrap between positive and negative.
 */
public class BombzCell
{
	static final byte OUTSIDE = -128;
	static final byte BLANK = OUTSIDE + 1;
	static final byte EARTH = BLANK + 1;
	static final byte MATCH = EARTH + 1;
	static final byte PICKET = MATCH + 1;
	static final byte BOMB1 = PICKET + 1;
	static final byte BOMB2 = BOMB1 + 1;
	static final byte EXPLO00 = BOMB2 + 1;
	static final byte EXPLO11 = EXPLO00 + 11;
	static final byte CHROME00 = EXPLO11 + 1;
	static final byte CHROME15 = CHROME00 + 15;
	static final byte BOMB1_FUSED_FIRST = CHROME15 + 1;
	static final byte BOMB1_FUSED_LAST =
			(BOMB1_FUSED_FIRST + K.FUSE_TICKS - 1);
	static final byte BOMB2_FUSED_FIRST = BOMB1_FUSED_LAST + 1;
	static final byte BOMB2_FUSED_LAST =
			(BOMB2_FUSED_FIRST + K.FUSE_TICKS - 1);
	
	static final int N_CELLS = BOMB2_FUSED_LAST + 1 - OUTSIDE;
}
