package vyglab.argeo.jni;

final public class Geocentric3D
	extends NativeObject
{
	private Geocentric3D()
	{ }

	public Geocentric3D(
		double x,
		double y,
		double z) {
		initialize(x, y, z);
	}

	@Override
	public void finalize() {
		dispose();
	}


	private native void initialize(
		double x, 
		double y,
		double z);

	public native double getX();
	public native double getY();
	public native double getZ();

	public native void dispose();
}
