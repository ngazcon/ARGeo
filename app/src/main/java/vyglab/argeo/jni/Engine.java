package vyglab.argeo.jni;

import android.content.Context;
import android.util.Log;

public class Engine
{
	static {
		System.loadLibrary("argeo-lib");

		 try {
			 System.loadLibrary("MGD");
		 }
		 catch (UnsatisfiedLinkError e)
		 {
			 Log.e("mgd", "lala");
		 }
	}

	public static native void init(Context context);
}
