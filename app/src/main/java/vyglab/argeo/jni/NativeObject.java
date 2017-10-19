package vyglab.argeo.jni;

import vyglab.argeo.jni.annotations.UsedByNativeCode;

public abstract class NativeObject
{
	/**
	 * Field used to store a pointer to the native instance allocated on the heap.
	 * Don't modify this value directly, or else you risk causing segfaults or
	 * leaking memory.
	 */
	@SuppressWarnings("UnusedDeclaration")
	@UsedByNativeCode
	protected long mPtr;

    /**
     * This method is used to invoke C++ destructors and free native resources,
     * and normally you want to declare it as <code>native</code> in your subclass.
     * The native implementation should, in turn, simply invoke the base class'
     * <code>dispose()</code> method. For more information, see the documentation
     * in the NativeObject.h header file.
     */
	protected abstract void dispose();
}
