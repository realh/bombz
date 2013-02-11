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
 * @author Tony Houghton
 *
 */
public class VPad implements DInput, OnScreenButton {
	
	private float mCentreX;
	private float mCentreY;
	// Squared versions of radii
	private float mOuterRad2;
	private float mInnerRad2;
	
	private final ButtonFeedback mFeedback;
	
	// If x/y is between these two ratios we're on diagonal
	private static final float RATIO = 0.7f;
	private static final float INV_RATIO = 1 / RATIO;
	
	// Pointer id pressing each button (l, r, u, d)
	private int[] mPressing = { -1, -1, -1, -1};
	
	// 0 or more of DInput constants ORred.
	private int mPressingMask;
	
	/**
	 * @param x				Position of top-left corner in viewport
	 * @param y				Position of top-left corner in viewport (0 == top)
	 * @param outerRadius	Radius of whole vpad
	 * @param innerRadius	Radius of "dead" area in middle
	 * @param feedback		Provider of haptic feedback, may be null
	 */
	public VPad(int x, int y, int outerRadius, int innerRadius,
			ButtonFeedback feedback) {
		mFeedback = feedback;
		setDimensions(x, y, outerRadius, innerRadius);
	}
	
	public VPad(ButtonFeedback feedback) {
		mFeedback = feedback;
	}
	
	public final void setDimensions(int x, int y, int outerRadius, int innerRadius) {
		mCentreX = x + outerRadius / 2;
		mCentreY = y + outerRadius / 2;
		mOuterRad2 = outerRadius * outerRadius;
		mInnerRad2 = innerRadius * innerRadius;
	}

	/* (non-Javadoc)
	 * @see uk.co.realh.hgame.OnScreenButton#handleEvent(int, int, int, int)
	 */
	@Override
	public void handleEvent(int type, int x, int y, int pointerId) {
		// TODO Auto-generated method stub
		x -= mCentreX;
		y -= mCentreY;
		float rsq = x * x + y * y;
		float ratio;
		if (y != 0)
			ratio = x / y;
		else if (x < 0)
			ratio = -999999999;
		else
			ratio = 999999999;
		int newMask = mPressingMask;
		if (BUTTON_RELEASE != type)
		{
			if (rsq < mOuterRad2 && rsq >= mInnerRad2)
			{
				if (x >= 0 && y >= 0)
				{
					if (ratio < INV_RATIO)
					{
						mPressing[3] = pointerId;
						newMask |= DOWN;
					}
					if (ratio > RATIO)
					{
						mPressing[1] = pointerId;
						newMask |= RIGHT;
					}
				}
				else if (x < 0 && y >= 0)
				{
					if (-ratio < INV_RATIO)
					{
						mPressing[3] = pointerId;
						newMask |= DOWN;
					}
					if (-ratio > RATIO)
					{
						mPressing[0] = pointerId;
						newMask |= LEFT;
					}
				}
				else if (x >= 0 && y < 0)
				{
					if (-ratio < INV_RATIO)
					{
						mPressing[2] = pointerId;
						newMask |= UP;
					}
					if (-ratio > RATIO)
					{
						mPressing[1] = pointerId;
						newMask |= RIGHT;
					}
				}
				else // if (x < 0 && y < 0)
				{
					if (ratio < INV_RATIO)
					{
						mPressing[2] = pointerId;
						newMask |= UP;
					}
					if (ratio > RATIO)
					{
						mPressing[0] = pointerId;
						newMask |= LEFT;
					}
				}
			}
			else {
				type = BUTTON_RELEASE;
			}
		}
		if (BUTTON_RELEASE == type)
		{
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
	public int pollDInput() {
		return mPressingMask;
	}

}
