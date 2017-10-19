package vyglab.argeo.app.controller.ListenersTTARView;

import android.app.Activity;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;

/**
 * Created by ngazcon on 12/07/2017.
 */

public class ListenerTTARViewTouchNormal implements View.OnTouchListener {

    private Activity m_activity;

    public ListenerTTARViewTouchNormal(Activity activity) {
        m_activity = activity;
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        Log.d("-- TOUCH EVENT: ", "listener NORMAL");

        return true;
    }

}
