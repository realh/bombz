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

import uk.co.realh.hgame.DInput;
import uk.co.realh.hgame.Log;
import uk.co.realh.hgame.RenderContext;

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
    
    private DInput mDInput;
    
    boolean mHaveMatch;
    byte mPushingBomb;

    BombzGameManager mMgr;
    BombzLevel mLevel;

    private static class Lrud {
    	public boolean left, right, up, down;
    	/*
    	Lrud(boolean l, boolean r, boolean u, boolean d) {
    		left = l;
    		right = r;
    		up = u;
    		down = d;
    	}
    	*/
    	
    	Lrud() {}
    }
    
    private Lrud mLrud = new Lrud();
    
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
    }

    void render(RenderContext rctx) {
    	int x = mTileX * K.FRUSTUM_TILE_SIZE +
	            (mInterX * K.FRUSTUM_TILE_SIZE) / K.DELTA_PIX;
    	int y = mTileY * K.FRUSTUM_TILE_SIZE +
	            (mInterY * K.FRUSTUM_TILE_SIZE) / K.DELTA_PIX;
    	/*
    	Log.d(TAG, "Pusher sprite " + mMgr.mTextures.mPusherSprite);
    	Log.d(TAG, "Direction " + mDirection);
    	Log.d(TAG, "Regions " + mMgr.mTextures.mPusherRegions);
    	Log.d(TAG, "Region[] " + mMgr.mTextures.mPusherRegions[mDirection]);
    	*/
    	mMgr.mTextures.mPusherSprite.setTexture(
    			mMgr.mTextures.mPusherRegions[mDirection]);
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
	    boolean refresh = false;
	    if (!mMoving)
	    {
	    	int keys = mDInput.pollDInput();
	        mLrud.left = (keys & DInput.LEFT) != 0;
	        mLrud.right = (keys & DInput.RIGHT) != 0;
	        mLrud.up = (keys & DInput.UP) != 0;
	        mLrud.down = (keys & DInput.DOWN) != 0;
	        // 2 opposite directions at once cancel each other out
	        if (mLrud.left && mLrud.right)
	        {
	            mLrud.left = mLrud.right = false;
	        }
	        if (mLrud.up && mLrud.down)
	        {
	            mLrud.up = mLrud.down = false;
	        }
	        // In case user is pressing horiz & vert simultaneously,
	        // alternate based on previous direction
	        if (mDirection == UP || mDirection == DOWN)
	        {
	            if (!checkHoriz())
	                checkVert();
	        }
	        else
	        {
	            if (!checkVert())
	                checkHoriz();
	        }
	        int dx = 0;
	        int dy = 0;
	        if (mLrud.left)
	        {
	            mMoving = true;
	            mDirection = LEFT;
	            dx = -1;
	        }
	        else if (mLrud.right)
	        {
	            mMoving = true;
	            mDirection = RIGHT;
	            dx = 1;
	        }
	        else if (mLrud.up)
	        {
	            mMoving = true;
	            mDirection = UP;
	            dy = -1;
	        }
	        else if (mLrud.down)
	        {
	            mMoving = true;
	            mDirection = DOWN;
	            dy = 1;
	        }
	        byte c = mLevel.getTileAt(mTileX + dx, mTileY +dy);
	        if (c == Cell.BOMB1 || c == Cell.BOMB2)
	        {
	            if (mHaveMatch)
	            {
	                Log.v(TAG, "Lighting bomb");
	                if (c == Cell.BOMB1)
	                {
	                    mLevel.setTileAt(mTileX + dx, mTileY +dy,
	                            Cell.BOMB1_FUSED_FIRST, true);
	                }
	                else
	                {
	                    mLevel.setTileAt(mTileX + dx, mTileY +dy,
	                            Cell.BOMB2_FUSED_FIRST, true);
	                }
	                mMoving = false;
	                mHaveMatch = false;
	            }
	            else
	            {
	                mPushingBomb = c;
	                mLevel.setBlankAt(mTileX + dx, mTileY +dy);
	            }
	        }
	    }
	    if (mMoving)
	    {
	        refresh = true;
	        switch (mDirection)
	        {
	            case LEFT:
	                mInterX -= 1;
	                if (mInterX == -K.FRUSTUM_TILE_SIZE)
	                    gotToNewTile(-1, 0);
	                else if (mInterX == -K.FRUSTUM_TILE_SIZE / 2)
	                    gotHalfway();
	                break;
	            case RIGHT:
	                mInterX += 1;
	                if (mInterX == K.FRUSTUM_TILE_SIZE)
	                    gotToNewTile(1, 0);
	                else if (mInterX == K.FRUSTUM_TILE_SIZE / 2)
	                    gotHalfway();
	                break;
	            case UP:
	                mInterY -= 1;
	                if (mInterY == -K.FRUSTUM_TILE_SIZE)
	                    gotToNewTile(0, -1);
	                else if (mInterY == -K.FRUSTUM_TILE_SIZE / 2)
	                    gotHalfway();
	                break;
	            case DOWN:
	                mInterY += 1;
	                if (mInterY == K.FRUSTUM_TILE_SIZE)
	                    gotToNewTile(0, 1);
	                else if (mInterY == K.FRUSTUM_TILE_SIZE / 2)
	                    gotHalfway();
	                break;
	        }
	    }
	    return refresh;
    }
    
	boolean checkHoriz()
	{
	    if ((mLrud.left &&
	    		mLevel.canMoveTo(mTileX - 1, mTileY, -1, 0, mHaveMatch)) ||
	            (mLrud.right && mLevel.canMoveTo(mTileX + 1, mTileY, 1, 0, mHaveMatch)))
	    {
	        mLrud.up = mLrud.down = false;
	        return true;
	    }
	    else
	    {
	        mLrud.left = mLrud.right = false;
	    }
	    return false;
	}
	
	boolean checkVert()
	{
	    if ((mLrud.up && mLevel.canMoveTo(mTileX, mTileY - 1, 0, -1, mHaveMatch)) ||
	            (mLrud.down && mLevel.canMoveTo(mTileX, mTileY + 1, 0, 1, mHaveMatch)))
	    {
	        mLrud.left = mLrud.right = false;
	        return true;
	    }
	    else
	    {
	        mLrud.up = mLrud.down = false;
	    }
	    return false;
	}

    /**
     * Called to update level when halfway to a new tile
     */
    private void gotHalfway()
	{
	    int x = mTileX;
	    int y = mTileY;
	    switch (mDirection)
	    {
	        case LEFT:
	            x -= 1;
	            break;
	        case RIGHT:
	            x += 1;
	            break;
	        case UP:
	            y -= 1;
	            break;
	        case DOWN:
	            y += 1;
	            break;
	    }
	    byte c = mLevel.getTileAt(x, y);
	    switch (c)
	    {
	        case Cell.MATCH:
	            mHaveMatch = true;
	        case Cell.EARTH:
	            mLevel.setBlankAt(x, y);
	            break;
	    }
	}

    /**
     * Got to new tile in directions represented by dx and dy (-1 to 1).
     * 
     * @param dx
     * @param dy
     */
    private void gotToNewTile(int dx, int dy)
	{
	    mInterX = mInterY = 0;
	    mTileX += dx;
	    mTileY += dy;
	    mMoving = false;
	    if (mPushingBomb != 0)
	    {
	        mLevel.setTileAt(mTileX + dx, mTileY + dy, mPushingBomb);
	        mPushingBomb = 0;
	    }
	}
}
