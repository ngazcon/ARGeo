package vyglab.argeo.jni;

public class ScreenSpaceCameraController extends NativeObject
{
    private ScreenSpaceCameraController()
    { }

    public native double getYawOffset();
    public native double getPitchOffset();
    public native double getRollOffset();

    public native void  setTranslationEnabled(boolean enabled);
    public native void  setRotationEnabled(boolean enabled);

    public native void    setTranslationSpeed(double value);
    public native double  getTranslationSpeed();

    public native void   setRotationSpeed(double value);
    public native double getRotationSpeed();

    public native Geodetic3D getPositionOffset();

    public native void dispose();
}
