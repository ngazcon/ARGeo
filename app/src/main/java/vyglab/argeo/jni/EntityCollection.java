package vyglab.argeo.jni;

public class EntityCollection extends NativeObject
{
	public EntityCollection()
	{ }

	protected native void initialize();
	protected native void dispose();

	public    native Entity  getById(String id);
	public    native boolean add(Entity entity);
	public    native boolean remove(Entity entity);
}
