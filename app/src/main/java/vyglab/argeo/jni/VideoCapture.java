package vyglab.argeo.jni;

import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.graphics.*;
import java.io.*;
import android.hardware.Camera.Size;

import java.util.List;

@SuppressWarnings( "deprecation" )
public class VideoCapture
        extends NativeObject
        implements SurfaceTexture.OnFrameAvailableListener
{
    private Camera mCamera;
    private Parameters mCameraParams;
    private SurfaceTexture mSurfaceTexture;

    public VideoCapture() {
    }

    @SuppressWarnings( "unused" )
    void start() {
        this.mCamera = Camera.open();

        setParameters(this.mCamera.getParameters());

        try {
            this.mCamera.setPreviewTexture(this.mSurfaceTexture);
        }
        catch (IOException e) {
            e.printStackTrace();
        }

        this.mCamera.startPreview();
    }

    @SuppressWarnings( "unused" )
    void stop() {
        if (isRunning()) {
            mCamera.stopPreview();
            mCamera.release();
        }
    }

    public void onFrameAvailable(SurfaceTexture surfaceTexture) {
        frameAvailable();
    }

    public native void frameAvailable();
    public native void dispose();

    public SurfaceTexture getSurfaceTexture() {
        return this.mSurfaceTexture;
    }

    public void setSurfaceTexture(SurfaceTexture surfaceTexture) {
        if (this.mSurfaceTexture != surfaceTexture) {
            this.mSurfaceTexture = surfaceTexture;
            this.mSurfaceTexture.setOnFrameAvailableListener(this);
        }
    }

    public void setParameters(Parameters value) {
        this.mCameraParams = value;

        List<String> sceneModes = this.mCameraParams.getSupportedSceneModes();
        if(sceneModes.contains(Parameters.SCENE_MODE_HDR)) {
            this.mCameraParams.setSceneMode(Parameters.SCENE_MODE_HDR);
        }

        // http://stackoverflow.com/questions/16727836/camera-display-preview-in-full-screen-does-not-maintain-aspect-ratio-image-i
        List<Size> sizes = this.mCameraParams.getSupportedPreviewSizes();
        Size cs = sizes.get(0);//getOptimalSize(sizes, 1184, 768);


        this.mCameraParams.setPreviewSize(cs.width, cs.height);
        setCurrentPreviewSize(cs.width, cs.height);

        List<int[]> fpsRanges = this.mCameraParams.getSupportedPreviewFpsRange();
        int[] fpsRange        = fpsRanges.get(fpsRanges.size() - 1);

        this.mCameraParams.setPreviewFpsRange(fpsRange[0], fpsRange[1]);

        List<String> whiteBalances = this.mCameraParams.getSupportedWhiteBalance();
        if(whiteBalances.contains(Parameters.WHITE_BALANCE_AUTO)) {
            this.mCameraParams.setWhiteBalance(Parameters.WHITE_BALANCE_AUTO);
        }

        List<String> focusModes = this.mCameraParams.getSupportedFocusModes();
        if(focusModes.contains(Parameters.FOCUS_MODE_CONTINUOUS_PICTURE)) {
            this.mCameraParams.setFocusMode(Parameters.FOCUS_MODE_CONTINUOUS_PICTURE);
        }

        this.mCameraParams.setFlashMode(Parameters.FLASH_MODE_OFF);

        if(this.mCameraParams.isAutoWhiteBalanceLockSupported()) {
            this.mCameraParams.setAutoWhiteBalanceLock(false);
        }

        if(this.mCameraParams.isAutoExposureLockSupported()) {
            this.mCameraParams.setAutoExposureLock(false);
        }
        //if(mCameraParams.isZoomSupported())
        //{
        //    int maxZoom = mCameraParams.getMaxZoom();
        //    mCameraParams.setZoom(maxZoom);
        //}

        this.mCamera.setParameters(this.mCameraParams);

        int zoom = value.getZoomRatios().get(value.getZoom()).intValue();

        float fl = this.mCameraParams.getFocalLength();

        // http://stackoverflow.com/questions/3261776/determine-angle-of-view-of-smartphone-camera
        double aspect = (double) cs.width / (double) cs.height;
        setCurrentAspectRatio(aspect);
        setCurrentPreviewSize(cs.width, cs.height);

        double thetaH = Math.toRadians(value.getHorizontalViewAngle());
        double thetaV = Math.toRadians(value.getVerticalViewAngle() - 13.5d);

        //double thetaV = 2d * Math.atan(Math.tan(thetaH / 2d) / aspect);
        //double thetaHa = 2d * Math.atan(aspect * Math.tan(thetaV / 2d));

        // https://developers.google.com/tango/overview/intrinsics-extrinsics
        //double thetaV = 2d * Math.atan((double) cs.height / (double) cs.width);
        thetaV        = 2d * Math.atan(100d * Math.tan(thetaV / 2d) / zoom);
        thetaH = 2d * Math.atan(100d * Math.tan(thetaH / 2d) / zoom);

        setCurrentVerticalViewAngle(thetaV);
        //setCurrentHorizontalViewAngle(thetaH);
    }



    /**
     * Calculate the optimal size of camera preview
     * @param sizes
     * @param w
     * @param h
     * @return
     */
    private Size getOptimalSize(
            List<Size> sizes,
            int w,
            int h) {

        final double ASPECT_TOLERANCE = 0.1;
        double targetRatio = (double)w / h;

        if (sizes == null)
            return null;

        Size optimalSize = null;
        double minDiff = Double.MAX_VALUE;

        int targetHeight = h;

        for (Size size : sizes) {
            double ratio = (double) size.width / size.height;

            if (Math.abs(ratio - targetRatio) > ASPECT_TOLERANCE)
                continue;

            if (Math.abs(size.height - targetHeight) < minDiff) {
                optimalSize = size;
                minDiff = Math.abs(size.height - targetHeight);
            }
        }

        if (optimalSize == null) {
            minDiff = Double.MAX_VALUE;
            for (Size size : sizes) {
                if (Math.abs(size.height - targetHeight) < minDiff) {
                    optimalSize = size;
                    minDiff = Math.abs(size.height - targetHeight);
                }
            }
        }
        return optimalSize;

    }

    public native boolean isRunning();

    protected native void setCurrentPreviewSize(double width, double height);

    public native double  getCurrentVerticalViewAngle();
    protected native void setCurrentVerticalViewAngle(double value);

    public native double  getCurrentAspectRatio();
    protected native void setCurrentAspectRatio(double value);

}
