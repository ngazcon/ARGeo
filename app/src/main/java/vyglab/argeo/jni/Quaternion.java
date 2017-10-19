package vyglab.argeo.jni;

final public class Quaternion
        extends NativeObject
{
    private Quaternion()
    { }

    public Quaternion(
            double w,
            double x,
            double y,
            double z) {
        initialize(
                w,
                x,
                y,
                z);
    }

    @Override
    public void finalize() {
        dispose();
    }

    public static native Quaternion fromTwoVectors(
            Geocentric3D u,
            Geocentric3D v);

    public static native Quaternion fromYawPitchRoll(
            double yaw,
            double pitch,
            double roll);

    public static native Quaternion fromAxisAngle(
            Geocentric3D axis,
            double angle);

    private native void initialize(
            double w,
            double x,
            double y,
            double z);

    public native double getW();
    public native double getX();
    public native double getY();
    public native double getZ();
    public native void dispose();
}

