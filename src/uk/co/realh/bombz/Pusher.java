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

import uk.co.realh.hgame.RenderContext;
import uk.co.realh.hgame.Sprite;

/**
 * @author Tony Houghton
 *
 */
public class Pusher {
	
	private static final String TAG = "Pusher";
	
	private static final int LEFT = K.FACING_LEFT;
	private static final int RIGHT = K.FACING_RIGHT;
	private static final int UP = K.FACING_UP;
	private static final int DOWN = K.FACING_DOWN;

    private int mTileX, mTileY;
    private int mInterX, mInterY;
    private int mDirection;
    private boolean mMoving;
    
    boolean mHaveMatch;
    byte mPushingBomb;

    BombzGameManager mMgr;
    BombzLevel mLevel;

    Pusher(BombzGameManager mgr, BombzLevel level) {
    	mMgr = mgr;
    	mLevel = level;
    }

    final void reset() {
	    mTileX = mLevel.mStartX;
	    mTileY = mLevel.mStartY;
	    mInterX = 0;
	    mInterY = 0;
	    mDirection = UP;
	    mMoving = false;
	    mHaveMatch = false;
	    mPushingBomb = 0;
    	mMgr.mTextures.mPusherSprite.setTexture(
    			mMgr.mTextures.mPusherRegions[mDirection]);
    }

    void render(RenderContext rctx) {
    	int x = mTileX * K.FRUSTUM_TILE_SIZE +
	            (mInterX * K.FRUSTUM_TILE_SIZE) / K.DELTA_PIX;
    	int y = mTileY * K.FRUSTUM_TILE_SIZE +
	            (mInterY * K.FRUSTUM_TILE_SIZE) / K.DELTA_PIX;
    	mMgr.mTextures.mPusherSprite.setPosition(x, y);
    	mMgr.mTextures.mPusherSprite.render(rctx);
	    if (0 != mPushingBomb)
	    {
	        switch (mDirection)
	        {
	            case LEFT:
	                x -= K.FRUSTUM_TILE_SIZE;
	                break;
	            case RIGHT:
	                x += K.FRUSTUM_TILE_SIZE;
	                break;
	            case UP:
	                y -= K.FRUSTUM_TILE_SIZE;
	                break;
	            case DOWN:
	                y += K.FRUSTUM_TILE_SIZE;
	                break;
	        }
	    	mMgr.mTextures.mBombSprite.setPosition(x, y);
	    	mMgr.mTextures.mBombSprite.render(rctx);
	    }
    }

    // Returns true if movement etc requires screen refresh
    boolean step()
    {
    	
    }
private:
    // Clear approprate directions depending on whether we can move.
    // Returns true if we can move.
    boolean checkHoriz(boolean *left, boolean *right, boolean *up, boolean *down);
    boolean checkVert(boolean *left, boolean *right, boolean *up, boolean *down);

    // Called to update level when halfway to a new tile
    void gotHalfway();

    // Got to new tile in directions represented by x and y (-1 to 1)
    void gotToNewTile(int dx, int dy);
}
