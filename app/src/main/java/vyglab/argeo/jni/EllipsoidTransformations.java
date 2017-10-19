package vyglab.argeo.jni;

public class EllipsoidTransformations
{
	public static native Geocentric3D geocentric3DFromGeodetic3D(Geodetic3D geodetic);
	public static native Geodetic3D   geodetic3DFromGeocentric3D(Geocentric3D geocentric);
}
