package uk.co.realh.bombz;

import android.os.Bundle;
import android.app.Activity;
//import android.view.Menu;

import uk.co.realh.hgame.Log;
import uk.co.realh.hgame.Sys;
import uk.co.realh.hgame.android.AndroidLog;
import uk.co.realh.hgame.android.AndroidSys;

/**
 * @author Tony Houghton
 */
public class BombzAndroidActivity extends Activity {
	
	private static final String TAG = "Activity";

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Log.setLogger(new AndroidLog());
		setContentView(R.layout.activity_bombz_android);
		Sys sys;
		String[] subdirs;
		try {
			sys = new AndroidSys(this,
					"realh", "bombz.sf.net", "Bombz", "uk.co.realh.bombz");
			subdirs = sys.listFolder("pngs");
		} catch (Throwable e) {
			Log.e(TAG, "Can't list pngs folder(s)", e);
			return;
		}
		String subs = null;
		for (String sub: subdirs)
		{
			if (subs == null)
				subs = sub;
			else
				subs += ", " + sub;
		}
		Log.i(TAG, "subs contains: " + subs);
	}

	/*
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_bombz_android, menu);
		return true;
	}
	*/
}
