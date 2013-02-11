package uk.co.realh.hgame.android;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Field;
import java.util.TreeSet;

import android.content.Context;
import android.content.res.AssetManager;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.os.Vibrator;
import android.util.Log;

import uk.co.realh.hgame.ButtonFeedback;
import uk.co.realh.hgame.Font;
import uk.co.realh.hgame.Image;
import uk.co.realh.hgame.SavedSettings;
import uk.co.realh.hgame.Sys;

/**
 * @author Tony Houghton
 */
public class AndroidSys implements Sys {
	
	private static String TAG = "Sys";
	
	private Context mContext;
	private AssetManager mAssets;
	//private String mOwner;
	//private String mDomain;
	//private String mAppName;
	private Class<?> mRString;
	private Class<?> mRDrawable;
	
	/**
	 * @param aContext	Android Context eg the Activity
	 * @param owner		Name associated with developer
	 * @param domain	Domain of URL associated with app/developer
	 * @param appName	App name
	 * @param pkg		Package name (containing R class)
	 * @throws ClassNotFoundException 
	 */
	public AndroidSys(Context aContext,
			String owner, String domain, String appName, String pkg)
			throws ClassNotFoundException
	{
		mContext = aContext;
		mAssets = aContext.getAssets();
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
	private String getSDCardFilename(String leafname, boolean ensure)
	{
		File f = mContext.getExternalFilesDir(null);
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
		Resources res = mContext.getResources();
		Bitmap bmp = ((BitmapDrawable) res.getDrawable(id)).getBitmap();
		Config cfg = bmp.getConfig();
		Log.d(TAG, "Res '" + name + "' bitmap format " + cfg.toString());
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
			tag = mContext.getString(f.getInt(null));
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
				mContext.getSystemService(Context.VIBRATOR_SERVICE);
		return (null != vib) ? new AndroidHapticFeedback(vib) : null;
	}

}
