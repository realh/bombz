package uk.co.realh.hgame.android;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Field;
import java.util.TreeSet;

import uk.co.realh.hgame.ButtonFeedback;
import uk.co.realh.hgame.DInput;
import uk.co.realh.hgame.Font;
import uk.co.realh.hgame.Image;
import uk.co.realh.hgame.SavedSettings;
import uk.co.realh.hgame.Sys;
import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.Vibrator;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.WindowManager;

/**
 * @author Tony Houghton
 */
public class AndroidSys implements Sys {
	
	private static String TAG = "Sys";
	
	private HGameActivity mActivity;
	private AssetManager mAssets;
	//private String mOwner;
	//private String mDomain;
	//private String mAppName;
	private Class<?> mRString;
	private Class<?> mRDrawable;

	/**
	 * @param act		Android Activity
	 * @param owner		Name associated with developer
	 * @param domain	Domain of URL associated with app/developer
	 * @param appName	App name
	 * @param pkg		Package name (containing R class)
	 * @throws ClassNotFoundException 
	 */
	public AndroidSys(HGameActivity act,
			String owner, String domain, String appName, String pkg)
			throws ClassNotFoundException
	{
		mActivity = act;
		mAssets = act.getAssets();
		//mOwner = owner;
		//mDomain = domain;
		//mAppName = appName;
		mRString = Class.forName(pkg + ".R$string");
		mRDrawable = Class.forName(pkg + ".R$drawable");
	}
	
	/**
	 * Returns assets pathname relative to SD card.
	 * 
	 * @param leafname
	 * @return
	 */
	String getSDCardFilename(String leafname, boolean ensure)
	{
		File f = mActivity.getExternalFilesDir(null);
		if (!f.isDirectory())
			f.mkdirs();
		Log.d(TAG, "sdcard path: " + f.getPath());
		return f.getPath() + "/" + leafname;
	}

	@Override
	public String getProfileFilename(String leafname) {
		return getSDCardFilename(leafname, false);
	}

	@Override
	public String getProfileFilename(String leafname, boolean ensure) {
		return getSDCardFilename(leafname, ensure);
	}

	@Override
	public InputStream openAsset(String leafname) throws IOException {
		try {
			return mAssets.open(leafname);
		} catch (FileNotFoundException e) {
			return new FileInputStream(getSDCardFilename(leafname, false));
		}
	}

	AssetManager getAssets() {
		return mAssets;
	}

	@Override
	public Image loadPNG(InputStream fd, String desc) {
		// TODO: We should support 16-bit for old phones
		Bitmap.Config config = Bitmap.Config.ARGB_8888;
		BitmapFactory.Options opts = new BitmapFactory.Options();
		opts.inPreferredConfig = config;
		Bitmap bitmap = BitmapFactory.decodeStream(fd);
		if (bitmap == null)
		{
			throw new RuntimeException("Unable to create bitmap from " + desc);
		}
		return new AndroidImage(bitmap);
	}

	@Override
	public Image loadResPNG(String name) {
		int id;
		try {
			id = mRDrawable.getDeclaredField(name).getInt(null);
		} catch (Throwable e) {
			Log.e(TAG, "Unable to load res PNG " + name);
			return null;
		}
		Resources res = mActivity.getResources();
		Drawable drwbl = res.getDrawable(id);
		Bitmap bmp = ((BitmapDrawable) drwbl).getBitmap();
		return new AndroidImage(bmp);
	}
	
	@Override
	public Font openFont(int px) {
		return new AndroidFont(px);
	}

	@Override
	public String[] listFolder(String folder) throws IOException {
		TreeSet<String> set = new TreeSet<String>();
		String[] files = null;
		try {
			files = mAssets.list(folder);
		} catch (FileNotFoundException e) {
			// Allow this in case folder is on SD card only
		}
		if (files != null && files.length != 0)
		{
			for (String item: files)
			{
				set.add(item);
			}
		}
		File f = new File(getSDCardFilename(folder, false));
		if (f.isDirectory())
		{
			files = f.list();
			if (files != null && files.length != 0)
			{
				for (String item: files)
				{
					set.add(item);
				}
			}
			else
			{
				Log.d(TAG, folder + " on sdcard is empty");
			}
		}
		return set.toArray(files);
	}

	@Override
	public String translate(String tag) {
		Field f;
		try {
			f = mRString.getDeclaredField(tag);
			tag = mActivity.getString(f.getInt(null));
		} catch (Throwable e) {
			Log.w(TAG, "Can't translate tag '" + tag + "'", e);
			tag = tag.replace('_', ' ');
		}
		return tag;
	}

	@Override
	public SavedSettings getSavedSettings(String leafname) throws IOException {
		return new AndroidSavedSettings(getProfileFilename(leafname, true));
	}

	@Override
	public boolean usesTouchScreen() {
		return true;
	}

	/**
	 * @see uk.co.realh.hgame.Sys#getHapticFeedback()
	 */
	@Override
	public ButtonFeedback getHapticFeedback() {
		Vibrator vib = (Vibrator)
				mActivity.getSystemService(Context.VIBRATOR_SERVICE);
		return (null != vib) ? new AndroidHapticFeedback(vib) : null;
	}

	/**
	 * @see uk.co.realh.hgame.Sys#disableScreenBlanker()
	 */
	@Override
	public void disableScreenBlanker() {
		mActivity.runOnUiThread(new ScreenBlankDisabler());
	}

	/**
	 * @see uk.co.realh.hgame.Sys#enableScreenBlanker()
	 */
	@Override
	public void enableScreenBlanker() {
		mActivity.runOnUiThread(new ScreenBlankEnabler());
	}
	
	private class ScreenBlankDisabler implements Runnable {
		@Override
		public void run() {
			mActivity.getWindow().addFlags(
					WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		}
	}

	private class ScreenBlankEnabler implements Runnable {
		@Override
		public void run() {
			mActivity.getWindow().clearFlags(
					WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		}
	}

	/**
	 * @see uk.co.realh.hgame.Sys#getDisplayDPI()
	 */
	@Override
	public int getDisplayDPI() {
		DisplayMetrics metrics = new DisplayMetrics();
		mActivity.getWindowManager().getDefaultDisplay().getMetrics(metrics);	
		Log.d(TAG, "Display DPI " + metrics.densityDpi);
		return metrics.densityDpi;
	}

    @Override
    public DInput getKbdDInput()
    {
        return mActivity.getKbdDInput();
    }
}
