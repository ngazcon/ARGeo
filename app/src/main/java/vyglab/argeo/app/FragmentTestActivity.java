package vyglab.argeo.app;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

import vyglab.argeo.R;
import vyglab.argeo.jni.Engine;

public class FragmentTestActivity extends AppCompatActivity
{
	 @Override
	 protected void onCreate(Bundle savedInstanceState) {
		 Engine.init(this);
		 super.onCreate(savedInstanceState);
		 setContentView(R.layout.fragment_test_activity);
	 }
}
