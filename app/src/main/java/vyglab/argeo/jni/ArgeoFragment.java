package vyglab.argeo.jni;

import android.os.Bundle;

import android.support.v4.app.Fragment;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.LayoutInflater;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import vyglab.argeo.R;
import vyglab.argeo.test.EntityTest;

public class ArgeoFragment
		extends Fragment
		implements View.OnTouchListener, SurfaceHolder.Callback
{
	private long   mPtr;
	private Viewer mViewer;

    private int   mActivePointerId = MotionEvent.INVALID_POINTER_ID;
    private float mLastTouchX;
    private float mLastTouchY;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		initialize();
	}

	@Override
	public View onCreateView(
			LayoutInflater inflater,
			ViewGroup container,
			Bundle savedInstanceState) {

		View view = inflater.inflate(R.layout.argeo_fragment, container, false);

		SurfaceView surfaceView = (SurfaceView)view.findViewById(R.id.argeo_surfaceview);
		surfaceView.getHolder().addCallback(this);

		return view;
	}

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        final int action = event.getActionMasked();


        switch (action) {
            case MotionEvent.ACTION_DOWN: {
                final int pointerIndex = event.getActionIndex();

                final float x = event.getX(pointerIndex);
                final float y = event.getY(pointerIndex);

                // Remember where we started (for dragging)
                mLastTouchX = x;
                mLastTouchY = y;

                // Save the ID of this pointer (for dragging)
                mActivePointerId = event.getPointerId(0);
                onDrag(
                    mLastTouchX,
                    mLastTouchY,
                    action,
                    0,
                    DragState.Start);
                break;
            }

            case MotionEvent.ACTION_MOVE: {
                // Find the index of the active pointer and fetch its position
                final int pointerIndex =
                        event.findPointerIndex(mActivePointerId);

                final float x = event.getX(pointerIndex);
                final float y = event.getY(pointerIndex);

                // Remember this touch position for the next move event
                mLastTouchX = x;
                mLastTouchY = y;

                onDrag(
                    mLastTouchX,
                    mLastTouchY,
                    action,
                    0,
                    DragState.Move);
                break;
            }

            case MotionEvent.ACTION_UP:
                mActivePointerId = MotionEvent.INVALID_POINTER_ID;
                onRotate(
                    mLastTouchX,
                    mLastTouchY,
                    action,
                    0,
                    0.25f);
                break;

            case MotionEvent.ACTION_CANCEL:
                mActivePointerId = MotionEvent.INVALID_POINTER_ID;
                break;

            case MotionEvent.ACTION_POINTER_UP: {
                final int pointerIndex = event.getActionIndex();
                final int pointerId    = event.getPointerId(pointerIndex);

                if (pointerId == mActivePointerId) {
                    // This was our active pointer going up. Choose a new
                    // active pointer and adjust accordingly.
                    final int newPointerIndex = pointerIndex == 0 ? 1 : 0;
                    mLastTouchX = event.getX(newPointerIndex);
                    mLastTouchY = event.getY(newPointerIndex);
                    mActivePointerId = event.getPointerId(newPointerIndex);

                    onDrag(
                        mLastTouchX,
                        mLastTouchY,
                        action,
                        0,
                        DragState.End);
                }
                break;
            }
        }
        return true;
    }

    @Override
	public void finalize() {
		dispose();
	}

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		onSurfaceCreated(holder.getSurface());


		EntityTest.addEntities(getViewer().getEntities());

		/*
		//getViewer().getDevicePoseCameraController().setLocationGpsProviderEnabled(false);
		//getViewer().getDevicePoseCameraController().setLocationNetworkProviderEnabled(false);
		getViewer().getScene().getCamera().setPosition(
		        EllipsoidTransformations.geocentric3DFromGeodetic3D(new Geodetic3D(0, 0, 0)));

		PolylineGraphics polylineGraphics = new PolylineGraphics(
		        new Geocentric3D[] {
                        EllipsoidTransformations.geocentric3DFromGeodetic3D(new Geodetic3D(0,  10, 5)),
                        EllipsoidTransformations.geocentric3DFromGeodetic3D(new Geodetic3D(0, -10, 5))},
                false,
                10);


		getViewer().getEntities().add(
		        new Entity("asd", polylineGraphics));

        // 60cm
        // 0.000005386,

        // 150 cm
        // 0.000013471

        EllipsoidTangentPlane plane = new EllipsoidTangentPlane(new Geodetic3D(
                0.000013471,
                0,
                0));

        Geocentric3D pa = plane.getOrigin();

        double x = pa.getX();
        double y = pa.getY();
        double z = pa.getZ();

        PlaneGraphics pp = new PlaneGraphics(
                plane,
                true);
        pp.setWidth(0.12);

        Entity ventana = new Entity(
                "Ventana",
        		pp);

        ventana.setOrientation(Quaternion.fromAxisAngle(plane.getXAxis(), Math.PI / 2d));

        getViewer().getEntities().add(ventana);
		*/

		getViewer().startRenderLoop();
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
