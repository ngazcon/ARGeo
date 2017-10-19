package vyglab.argeo.jni;

public class Entity extends NativeObject
{
	private IGraphics mGraphics;

	private Entity()
	{ }

	public Entity(
			String id,
			String name,
			IGraphics graphics) {
		initialize(
				id,
				name,
				graphics);
	}

	public Entity(
			String name,
			IGraphics graphics) {
		initialize(
				name,
				graphics);
	}

	protected native void initialize(
			String name,
			IGraphics graphics);

	protected native void initialize(
			String id,
			String name,
			IGraphics graphics);

	protected native void dispose();

	public native String getId();
	public native String getName();

	public IGraphics getGraphics() {
		if (this.mGraphics == null) {
			this.mGraphics = nativeGetGraphics();
		}
		return this.mGraphics;
	}

	public native Geocentric3D getPosition();
	public native Quaternion   getOrientation();

	public native void setPosition(Geodetic3D geodetic);
	public native void setPosition(Geocentric3D geocentric);
	public native void setOrientation(Quaternion quaternion);

	private native IGraphics    nativeGetGraphics();
}
