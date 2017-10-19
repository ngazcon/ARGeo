package vyglab.argeo.jni;

public class DevicePoseCameraController extends NativeObject
{
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

    public native void dispose();
}
