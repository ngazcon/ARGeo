package vyglab.argeo.jni;

public class ModelGraphics extends IGraphics
{
	private ModelGraphics()
	{ }

	public ModelGraphics(String path) {
		initialize(path);
	}

	public ModelGraphics(
		String path,
		int heightClamping,
		int orientationClamping) {
		initialize(
			path,
			heightClamping,
			orientationClamping);
	}

	private native void initialize(String path);
	private native void initialize(
		String path,
		int heightClamping,
		int orientationClamping);

	private native void setHeightClamping(int clamping);
	private native void setOrientationClamping(int clamping);

}
