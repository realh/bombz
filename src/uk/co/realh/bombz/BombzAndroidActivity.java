package uk.co.realh.bombz;

import uk.co.realh.hgame.android.HGameActivity;

/**
 * @author Tony Houghton
 */
public class BombzAndroidActivity extends HGameActivity {
	
	BombzAndroidActivity() {
		createSys("realh", "bombz.sf.net", "Bombz", "uk.co.realh.bombz");
		mMgr = new BombzGameManager(mSys);
	}
	
}
