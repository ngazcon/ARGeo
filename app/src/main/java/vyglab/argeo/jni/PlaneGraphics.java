package vyglab.argeo.jni;

public class PlaneGraphics extends IGraphics
{
    private PlaneGraphics()
    { }

    public PlaneGraphics(
            EllipsoidTangentPlane ellipsoidTangentPlane,
            boolean flag) {
        initialize(ellipsoidTangentPlane, flag);
    }

    private native void initialize(EllipsoidTangentPlane ellipsoidTangentPlane, boolean flag);


    public native void setWidth(double width);
    public native double getWidth();
}
