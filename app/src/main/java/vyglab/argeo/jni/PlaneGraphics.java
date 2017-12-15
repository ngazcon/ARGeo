package vyglab.argeo.jni;

public class PlaneGraphics extends IGraphics
{
    private PlaneGraphics()
    { }

    // Flag is used for the plane color: red or blue
    public PlaneGraphics(
            EllipsoidTangentPlane ellipsoidTangentPlane,
            boolean flag) {
        initialize(ellipsoidTangentPlane, flag);
    }

    private native void initialize(EllipsoidTangentPlane ellipsoidTangentPlane, boolean flag);


    public native void setWidth(double width);
    public native double getWidth();

    public native void setHeight(double width);
    public native double getHeight();
}
