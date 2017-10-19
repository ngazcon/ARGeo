package vyglab.argeo.jni;

final public class Geodetic3D 
	extends NativeObject
{
	private Geodetic3D()
	{ }

	public Geodetic3D(
		double latitude,
		double longitude,
		double altitude) {
		initialize(
			latitude,
			longitude,
			altitude);
	}

	public Geodetic3D(
		double latitude,
		double longitude) {
		initialize(
			latitude,
			longitude,
			0);
	}

	@Override
	public void finalize() {
		dispose();
	}

	private native void initialize(
		double latitude,
		double longitude,
		double altitude);

	public native double getAltitude();
	public native double getLatitude();
	public native double getLongitude();
	public native void dispose();
}

