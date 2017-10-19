package vyglab.argeo.jni;

public class EllipsoidTangentPlane extends NativeObject
{
    private Geocentric3D mOrigin;
    private Geocentric3D mNormal;
    private Geocentric3D mXAxis;
    private Geocentric3D mYAxis;

    protected EllipsoidTangentPlane()
    { }

    public EllipsoidTangentPlane(Geodetic3D origin) {
        initialize(origin);
    }

    public Geocentric3D getOrigin() {
        if (this.mOrigin == null) {
            this.mOrigin = nativeGetOrigin();
        }
        return this.mOrigin;
    }

    public Geocentric3D getNormal() {
        if (this.mNormal == null) {
            this.mNormal = nativeGetNormal();
        }
        return this.mNormal;
    }

    public Geocentric3D getXAxis() {
        if (this.mXAxis == null) {
            this.mXAxis = nativeGetXAxis();
        }
        return this.mXAxis;
    }

    public Geocentric3D getYAxis() {
        if (this.mYAxis == null) {
            this.mYAxis = nativeGetYAxis();
        }
        return this.mYAxis;
    }

    private native void initialize(Geodetic3D origin);

    private native  Geocentric3D nativeGetOrigin();
    private native  Geocentric3D nativeGetNormal();
    private native  Geocentric3D nativeGetXAxis();
    private native  Geocentric3D nativeGetYAxis();

    public native void dispose();
}
