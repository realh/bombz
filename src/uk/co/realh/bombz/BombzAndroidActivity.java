package uk.co.realh.bombz;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.util.Random;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.view.Window;
import android.view.WindowManager;
import android.app.Activity;
//import android.view.Menu;


/**
 * @author Tony Houghton
 */
public class BombzAndroidActivity extends Activity {
	
	private static final String TAG = "Activity";
	private GLSurfaceView mGlView;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);
		
		mGlView = new GLSurfaceView(this);
		mGlView.setRenderer(new TestRenderer());
		//mGlView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
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

		private int mW, mH;
		private int mAspectW;
		private FloatBuffer mVertBuffer;
		private Random mRand = new Random();
		
		TestRenderer() {
			ByteBuffer bb = ByteBuffer.allocateDirect(24);
			bb.order(ByteOrder.nativeOrder());
			mVertBuffer = bb.asFloatBuffer();
			float coords[] = {20, 20, 620, 20, 320, 460}; 
			mVertBuffer.put(coords);
			mVertBuffer.flip();
		}
		
		/* (non-Javadoc)
		 * @see android.opengl.GLSurfaceView.Renderer#onDrawFrame(
		 * 		javax.microedition.khronos.opengles.GL10)
		 */
		@Override
		public void onDrawFrame(GL10 gl) {
			//Log.d(TAG, "onDrawFrame");
			Log.d(TAG, "Setting viewport " +
					(mW - mAspectW) / 2 + ", " + 0 +
					", " + mAspectW + ", " + mH);
			//gl.glViewport((mW - mAspectW) / 2, 0, mAspectW, mH);
			gl.glClearColor(mRand.nextFloat(), mRand.nextFloat(),
					mRand.nextFloat(), 1);
			gl.glClear(GL10.GL_COLOR_BUFFER_BIT);
		    gl.glMatrixMode(GL10.GL_PROJECTION);
		    gl.glLoadIdentity();
			gl.glOrthof(0, 640, 0, 480, 1, -1);
			gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
			gl.glColor4f(1, 0, 0, 1);
			gl.glVertexPointer(2, GL10.GL_FLOAT, 0, mVertBuffer);
			gl.glDrawArrays(GL10.GL_TRIANGLES, 0, 3);
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				Log.w(TAG, "Sleep interrupted", e);
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
				initSize(gl, w, h);
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
			//mRCtx = new AndroidGles1RenderContext(mGlView, gl);
			//Log.d(TAG, "Created RenderContext");
			gl.glClearColor(0, 0, 0, 1);
			initSize(gl, mGlView.getWidth(), mGlView.getHeight());
		}
		
		private void initSize(GL10 gl, int w, int h)
		{
			mW = w;
			mH = h;
			mAspectW = h * 4 / 3;
			mGlView.requestRender();
		}
		
	}
}
