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

/**
 * Like VPad but with 4 separate buttons.
 * @author Tony Houghton
 */
public class VButtons implements DInput, OnScreenButton {
	
	// LRUD in screen coordinates
	private SimpleRect[] mBounds = {
			new SimpleRect(), new SimpleRect(),
			new SimpleRect(), new SimpleRect()
	};
	ButtonFeedback mFeedback;

	// Pointer id pressing each button (l, r, u, d)
	private int[] mPressing = { -1, -1, -1, -1};
	
	// 0 or more of DInput constants ORred.
	private int mPressingMask;
	
	/**
	 */
	public VButtons(ButtonFeedback feedback) {
		mFeedback = feedback;
	}
	
	/**
	 * Set bounds of left button in screen coords
	 * @param x
	 * @param y
	 * @param w
	 * @param h
	 */
	public void setLeftBounds(int x, int y, int w, int h) {
		mBounds[0].setRect(x, y, w, h);
	}

	/**
	 * Set bounds of right button in screen coords
	 * @param x
	 * @param y
	 * @param w
	 * @param h
	 */
	public void setRightBounds(int x, int y, int w, int h) {
		mBounds[1].setRect(x, y, w, h);
	}

	/**
	 * Set bounds of up button in screen coords
	 * @param x
	 * @param y
	 * @param w
	 * @param h
	 */
	public void setUpBounds(int x, int y, int w, int h) {
		mBounds[2].setRect(x, y, w, h);
	}

	/**
	 * Set bounds of down button in screen coords
	 * @param x
	 * @param y
	 * @param w
	 * @param h
	 */
	public void setDownBounds(int x, int y, int w, int h) {
		mBounds[3].setRect(x, y, w, h);
	}

	synchronized
	public void reset() {
		mPressing[0] = mPressing[1] = mPressing[2] = mPressing[3] = -1;
		mPressingMask = 0;
	}
	
	/**
	 * @see uk.co.realh.hgame.OnScreenButton#handleEvent(int, int, int, int)
	 */
	@Override
	synchronized
	public void handleEvent(int type, int x, int y, int pointerId) {
		// TODO Auto-generated method stub
		int newMask = mPressingMask;
		if (mPressing[0] == pointerId)
		{
			mPressing[0] = -1;
			newMask &= ~LEFT;
		}
		if (mPressing[1] == pointerId)
		{
			mPressing[1] = -1;
			newMask &= ~RIGHT;
		}
		if (mPressing[2] == pointerId)
		{
			mPressing[2] = -1;
			newMask &= ~UP;
		}
		if (mPressing[3] == pointerId)
		{
			mPressing[3] = -1;
			newMask &= ~DOWN;
		}
		if (BUTTON_RELEASE != type)
		{
			if (mBounds[0].isPointInRect(x, y)) {
				mPressing[0] = pointerId;
				newMask |= LEFT;
			}
			else if (mBounds[1].isPointInRect(x, y)) {
				mPressing[1] = pointerId;
				newMask |= RIGHT;
			}
			else if (mBounds[2].isPointInRect(x, y)) {
				mPressing[2] = pointerId;
				newMask |= UP;
			}
			else if (mBounds[3].isPointInRect(x, y)) {
				mPressing[3] = pointerId;
				newMask |= DOWN;
			}
		}
		if (newMask != mPressingMask)
		{
			mPressingMask = newMask;
			if (mFeedback != null)
				mFeedback.feedback();
		}
	}

	/**
	 * @see uk.co.realh.hgame.DInput#pollDInput()
	 */
	@Override
	synchronized
	public int pollDInput() {
		return mPressingMask;
	}

}
