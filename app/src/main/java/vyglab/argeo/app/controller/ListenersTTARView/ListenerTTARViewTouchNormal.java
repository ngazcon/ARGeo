package vyglab.argeo.app.controller.ListenersTTARView;

import android.app.Activity;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;
import android.view.View;

/**
 * Created by ngazcon on 12/07/2017.
 */

public class ListenerTTARViewTouchNormal implements View.OnTouchListener {

    private Activity m_activity;
    private ScaleGestureDetector m_scale_detector;
    private GestureDetector m_doubletap_detector;

    public ListenerTTARViewTouchNormal(Activity activity) {

        m_activity = activity;
        m_scale_detector = new ScaleGestureDetector(activity.getApplicationContext(), new ListenerTTARViewScale(m_activity));
        m_doubletap_detector = new GestureDetector(activity.getApplicationContext(), new ListenerTTARViewDoubleTap());
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        Log.d("-- TOUCH EVENT: ", "listener NORMAL");
        m_scale_detector.onTouchEvent(event);
        if (!m_scale_detector.isInProgress()) {
            m_doubletap_detector.onTouchEvent(event);
        }

        return true;
    }

}
