package uk.co.realh.bombz;

import android.os.Bundle;
import android.app.Activity;
//import android.view.Menu;

import uk.co.realh.hgame.Log;
import uk.co.realh.hgame.android.AndroidLog;

public class BombzAndroidActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Log.setLogger(new AndroidLog());
		setContentView(R.layout.activity_bombz_android);
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
