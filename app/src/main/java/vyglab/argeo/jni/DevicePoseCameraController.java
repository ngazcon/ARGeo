package vyglab.argeo.jni;

public class DevicePoseCameraController extends NativeObject
{
    public static int STANDING = 0;
    public static int MOVING   = 1;

    private DevicePoseCameraController()
    { }

    public native double getYaw();
    public native double getPitch();
    public native double getRoll();

    public native void enableLocationUpdates();
    public native void disableLocationUpdates();

    public native void enableOrientationUpdates();
    public native void disableOrientationUpdates();

    public native Geodetic3D getPosition();

    public native double getSensorLowPassFilterAlpha();
    public native void   setSensorLowPassFilterAlpha(double alpha);

    public native void setGPSStandingFrec(int value);
    public native int  getGPSStandingFrec();

    public native void setGPSMovingFrec(int value);
    public native int  getGPSMovingFrec();

    public native void setNetworkStandingFrec(int value);
    public native int  getNetworkStandingFrec();

    public native void setNetworkMovingFrec(int value);
    public native int  getNetworkMovingFrec();

    public native void setInitialSamples(int value);
    public native int  getInitialSamples();

    public native void setAccuracyDelta(int value);
    public native int  getAccuracyDelta();

    public native int  getGPSState();
    public native void forceLocationUpdates();

    public native double getSpeed();
    public native double getAccuracy();
    public native String getProvider();

    public native void dispose();
}
