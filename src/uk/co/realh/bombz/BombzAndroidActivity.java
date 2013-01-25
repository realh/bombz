package uk.co.realh.bombz;

import java.io.IOException;
import java.io.InputStream;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;
import android.app.Activity;
//import android.view.Menu;

import uk.co.realh.hgame.Image;
import uk.co.realh.hgame.Log;
import uk.co.realh.hgame.Sprite;
import uk.co.realh.hgame.Sys;
import uk.co.realh.hgame.TextureAtlas;
import uk.co.realh.hgame.TextureRegion;
import uk.co.realh.hgame.TileBatcher;
import uk.co.realh.hgame.android.AndroidLog;
import uk.co.realh.hgame.android.AndroidSys;
import uk.co.realh.hgame.gles1.android.AndroidGles1RenderContext;

/**
 * @author Tony Houghton
 */
public class BombzAndroidActivity extends Activity {
	
	private static final String TAG = "Activity";
	private GLSurfaceView mGlView;
	private Sys mSys;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Log.setLogger(new AndroidLog());
		
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);

		
		String[] subdirs;
		try {
			mSys = new AndroidSys(this,
					"realh", "bombz.sf.net", "Bombz", "uk.co.realh.bombz");
			subdirs = mSys.listFolder("pngs");
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
		
		mGlView = new GLSurfaceView(this);
		mGlView.setRenderer(new TestRenderer());
		setContentView(mGlView);
	}

	/*
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_bombz_android, menu);
		return true;
	}
	*/
	
	@Override
	public void onResume() {
		super.onResume();
		mGlView.onResume();
	}
	
	@Override
	public void onPause() {
		super.onPause();
		mGlView.onPause();
	}
	
	private class TestRenderer implements GLSurfaceView.Renderer
	{

		private AndroidGles1RenderContext mRCtx;
		private TextureAtlas mLogoAtlas, mTileAtlas;
		private Sprite mLogoSprite;
		private TileBatcher mBatcher;
		private int mW, mH;
		
		/* (non-Javadoc)
		 * @see android.opengl.GLSurfaceView.Renderer#onDrawFrame(
		 * 		javax.microedition.khronos.opengles.GL10)
		 */
		@Override
		public void onDrawFrame(GL10 gl) {
			try {
				Log.d(TAG, "onDrawFrame");
				mRCtx.cls();
				mRCtx.enableBlend(false);
				mRCtx.bindTexture(mTileAtlas);
				mBatcher.render(mRCtx);
				Log.d(TAG, "Rendered tiles");
				mRCtx.enableBlend(true);
				mRCtx.bindTexture(mLogoAtlas);
				mLogoSprite.render(mRCtx);
				Log.d(TAG, "Rendered logo");
			} catch (Throwable e) {
				Log.e(TAG, "Rendering exception", e);
			}
		}

		/* (non-Javadoc)
		 * @see android.opengl.GLSurfaceView.Renderer#onSurfaceChanged(
		 * 		javax.microedition.khronos.opengles.GL10, int, int)
		 */
		@Override
		public void onSurfaceChanged(GL10 gl, int w, int h) {
			if (w != mW || h != mH)
			{
				Log.d(TAG, "Surface resized to " + w + "x" + h);
				initSize(w, h);
			}
		}

		/* (non-Javadoc)
		 * @see android.opengl.GLSurfaceView.Renderer#onSurfaceCreated(
		 * 		javax.microedition.khronos.opengles.GL10,
		 * 		javax.microedition.khronos.egl.EGLConfig)
		 */
		@Override
		public void onSurfaceCreated(GL10 gl, EGLConfig config) {
			Log.d(TAG, "Surface created " +
					mGlView.getWidth() + "x" + mGlView.getHeight());
			mRCtx = new AndroidGles1RenderContext(mGlView, gl, null);
			mRCtx.enable2DTextures(true);
			try {
				InputStream fd = mSys.openAsset("pngs/32/title_logo.png");
				Image img = mSys.loadPNG(fd, "title logo");
				fd.close();
				Log.d(TAG, "Loaded title logo PNG " +
						img.getWidth() + "x" + img.getHeight());
				mLogoAtlas = mRCtx.uploadTexture(img, true);
				TextureRegion region = mLogoAtlas.createRegion(0, 0, 
						img.getWidth(), img.getHeight());
				float aspect = (float) img.getWidth() / (float) img.getHeight();
				mLogoSprite = mRCtx.createSprite(region, 20, 20,
						600, (int) ((float) 600 / aspect));
				img.dispose();
				
				fd = mSys.openAsset("pngs/32/tile_atlas.png");
				img = mSys.loadPNG(fd, "title logo");
				fd.close();
				Log.d(TAG, "Loaded tile atlas PNG " +
						img.getWidth() + "x" + img.getHeight());
				mTileAtlas = mRCtx.uploadTexture(img, false);
				img.dispose();
				region = mTileAtlas.createRegion(0, 0, 32, 32);
				mBatcher = mRCtx.createTileBatcher(20, 15, 32, 32);
				for (int y = 0; y < 15; ++y)
				{
					for (int x = 0; x < 20; ++x)
					{
						mBatcher.setTextureAt(region, x, y);
					}
				}
			} catch (IOException e) {
				Log.e(TAG, "Failed to load title logo", e);
			}
			initSize(mGlView.getWidth(), mGlView.getHeight());
		}
		
		private void initSize(int w, int h)
		{
			mW = w;
			mH = h;
			int aspect_w = h * 4 / 3;
			Log.d(TAG, "Setting viewport " +
					(w - aspect_w) / 2 + ", " + h + ", " + aspect_w + ", " + 0);
			mRCtx.setViewport((w - aspect_w) / 2, 0, aspect_w, h);
			mRCtx.set2DFrustum(0, 640, 0, 480);
			mGlView.requestRender();
		}
		
	}
}
