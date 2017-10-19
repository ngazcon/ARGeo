package vyglab.argeo.jni;

public class ITerrain extends NativeObject
{
    protected ITerrain()
    { }

    public native void    setWireframe(boolean value);
    public native boolean getWireframe();

    public native void    setShow(boolean value);
    public native boolean getShow();

    public native void    setLighting(boolean value);
    public native boolean getLighting();

    public native void   setHeightExaggeration(double value);
    public native double getHeightExaggeration();

    protected native void dispose();
}
