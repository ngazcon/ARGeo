package vyglab.argeo.jni;

public class BillboardGraphics extends IGraphics
{
	private BillboardGraphics()
	{ }

	public BillboardGraphics(String imagePath) {
		initialize(imagePath);
	}

	public BillboardGraphics(
			String imagePath,
			String imageId) {
		initialize(
				imagePath,
				imageId);
	}

	public BillboardGraphics(
			String imagePath,
			String imageId,
			int width,
			int height,
			int heightClamping) {
		initialize(
				imagePath,
				imageId,
				width,
				height,
				heightClamping);
	}

	private native void initialize(
			String imagePath);

	private native void initialize(
			String imagePath,
			String imageId);

	private native void initialize(
			String imagePath,
			String imageId,
			int    width,
			int    height,
			int    heightClamping);

	public native String getImageId();

	public native String getImagePath();
	public native void   setImagePath(String path);

	public native void    setShow(boolean show);
	public native boolean getShow();

	public native int  getHeight();
	public native void setHeight(int height);

	public native int  getWidth();
	public native void setWidth(int width);
}
