package vyglab.argeo.jni;

import android.os.Bundle;
import android.app.Activity;

import android.view.View;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import vyglab.argeo.R;
import vyglab.argeo.test.EntityTest;

public class ArgeoActivity
		extends Activity
		implements View.OnTouchListener, SurfaceHolder.Callback
{
	private long   mPtr;
	private Viewer mViewer;

	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		Engine.init(this);
		initialize();

		setContentView(R.layout.argeo_activity);

		SurfaceView surfaceView = (SurfaceView)findViewById(R.id.argeo_surfaceview);
		surfaceView.getHolder().addCallback(this);

		surfaceView.setOnTouchListener(this);

		super.onCreate(savedInstanceState);
	}


	@Override
	public void finalize() {
		dispose();
	}

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		onSurfaceCreated(holder.getSurface());

		getViewer().startRenderLoop();

		getViewer().executeOnRenderLoop(new RenderLoopRunnable() {
			@Override
			public void run()
			{
				EntityTest.addEntities(getViewer().getEntities());
			}
		});
	}

	@Override
	public void surfaceChanged(
			SurfaceHolder holder,
			int format,
			int width,
			int height)
	{ }

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		getViewer().stopRenderLoop();
		onSurfaceDestroyed(holder.getSurface());
	}


	@Override
	public boolean onTouch(View v, MotionEvent event) {
		final int windowX = (int) event.getX();
		final int windowY = (int) event.getY();

		getViewer().executeOnRenderLoop(new RenderLoopRunnable() {
			@Override
			public void run()
			{
				getViewer().
						getScene().
						getTerrain().setWireframe(true);
			}
		});

		return true;
	}


	public Viewer getViewer() {
		if (this.mViewer == null) {
			this.mViewer = nativeGetViewer();
		}
		return this.mViewer;
	}

	private native Viewer nativeGetViewer();

	protected native void initialize();
	protected native void dispose();

	protected native void onSurfaceCreated(Surface surface);
	protected native void onSurfaceDestroyed(Surface surface);

	protected native boolean onDrag(
			float x,
			float y,
			int   action,
			long  eventTime,
			int   dragState);

	protected native boolean onScale(
			float x,
			float y,
			int   action,
			long  eventTime,
			float factor);

	protected native boolean onRotate(
			float x,
			float y,
			int   action,
			long  eventTime,
			float radians);
}
