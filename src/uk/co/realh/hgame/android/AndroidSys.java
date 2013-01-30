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
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

import uk.co.realh.hgame.Font;
import uk.co.realh.hgame.Image;
import uk.co.realh.hgame.Sys;
import uk.co.realh.hgame.Settings;

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
	}
	
	/**
	 * Returns assets pathname relative to SD card.
	 * 
	 * @param leafname
	 * @return
	 */
	private String getSDCardFilename(String leafname)
	{
		File f = mContext.getExternalFilesDir(null);
		Log.d(TAG, "sdcard path: " + f.getPath());
		return f.getPath() + "/" + leafname;
	}

	@Override
	public String getProfileFilename(String leafname) {
		return getSDCardFilename(leafname);
	}

	@Override
	public InputStream openAsset(String leafname) throws IOException {
		try {
			return mAssets.open(leafname);
		} catch (FileNotFoundException e) {
			return new FileInputStream(getSDCardFilename(leafname));
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
	public Font openFont(int px) {
		// TODO Auto-generated method stub
		return null;
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
		File f = new File(getSDCardFilename(folder));
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
			return mContext.getString(f.getInt(null));
		} catch (Throwable e) {
			Log.w(TAG, "Can't translate tag '" + tag + "'", e);
		}
		return tag;
	}

	@Override
	public Settings createSettings() {
		// TODO Auto-generated method stub
		return null;
	}

}
