package vyglab.argeo.app.controller.ListenersTTARView;

import android.app.Activity;
import android.util.Log;
import android.view.ScaleGestureDetector;
import vyglab.argeo.app.MediatorTTARView;

/**
 * Created by ngazcon on 13/11/2017.
 */

public class ListenerTTARViewScale extends ScaleGestureDetector.SimpleOnScaleGestureListener {
    private Activity m_activity;
    private float m_initial_span;
    private float m_scale;
    private float m_width;
    private float m_height;

    public ListenerTTARViewScale(Activity activity) {
        m_activity = activity;
    }

    @Override
    public boolean onScale(ScaleGestureDetector detector) {
        float current = detector.getCurrentSpan();

        //todo por alguna razon cdo cierro pega un salto
        m_scale = current / m_initial_span;

        int delta_w = (int) (m_width * m_scale);
        int delta_h = (int) (m_height * m_scale);

        Log.d("-- TOUCH EVENT: ", String.valueOf(m_scale));
        Log.d("-- TOUCH EVENT: dw", String.valueOf(delta_w));
        Log.d("-- TOUCH EVENT: dh", String.valueOf(delta_h));
        MediatorTTARView.getInstance().changeTTARViewPictureInPictureSize(delta_w, delta_h);

        return true;
    }

    @Override
    public boolean onScaleBegin(ScaleGestureDetector detector) {
        //todo revisar landscape 3
        m_initial_span = detector.getCurrentSpan();
        m_width = MediatorTTARView.getInstance().getTTARViewWidth();
        m_height = MediatorTTARView.getInstance().getTTARViewHeight();
        Log.d("-- TOUCH EVENT: init w", String.valueOf(m_width));
        Log.d("-- TOUCH EVENT: init h", String.valueOf(m_height));

        return true;
    }

    @Override
    public void onScaleEnd(ScaleGestureDetector detector) {
    }
}
