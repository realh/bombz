package uk.co.realh.bombz;

import android.os.Bundle;
import android.util.Log;

import uk.co.realh.hgame.android.HGameActivity;

/**
 * @author Tony Houghton
 */
public class BombzAndroidActivity extends HGameActivity {
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		Log.d(TAG, "Entering onCreate");
		try {
			initLog();
			createSys("realh", "bombz.sf.net", "Bombz", "uk.co.realh.bombz");
			Log.d(TAG, "Created Sys");
			mMgr = new BombzGameManager(mSys);
			Log.d(TAG, "Created GameManager");
			super.onCreate(savedInstanceState);
			Log.d(TAG, "Called HGameActivity.onCreate");
		} catch (Throwable e) {
			Log.e(TAG, "Exception in onCreate", e);
		}
	}
	
}
