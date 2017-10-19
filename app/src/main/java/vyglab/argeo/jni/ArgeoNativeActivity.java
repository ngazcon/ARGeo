package vyglab.argeo.jni;

import android.app.NativeActivity;
import android.os.Bundle;
import android.view.SurfaceHolder;

public class ArgeoNativeActivity
	extends NativeActivity
	implements SurfaceHolder.Callback
{
	@Override
    public void onCreate(Bundle savedInstanceState) 
    {
		//setContentView(R.layout.main);
		super.onCreate(savedInstanceState);
    }

	public void onGLInitialized()
	{ }

	static 
	{
       System.loadLibrary("argeo-lib");
	}
}
