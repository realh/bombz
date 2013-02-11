package uk.co.realh.bombz;

import android.os.Bundle;

import uk.co.realh.hgame.Log;
import uk.co.realh.hgame.android.HGameActivity;

/**
 * @author Tony Houghton
 */
public class BombzAndroidActivity extends HGameActivity {
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		try {
			initLog();
			createSys("realh", "bombz.sf.net", "Bombz", "uk.co.realh.bombz");
			mMgr = new BombzGameManager(mSys, this);
			super.onCreate(savedInstanceState);
		} catch (Throwable e) {
			Log.e(TAG, "Exception in onCreate", e);
		}
	}

}
