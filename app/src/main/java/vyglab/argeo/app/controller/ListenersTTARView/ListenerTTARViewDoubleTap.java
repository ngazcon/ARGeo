package vyglab.argeo.app.controller.ListenersTTARView;

import android.view.GestureDetector;
import android.view.MotionEvent;

import vyglab.argeo.app.MediatorTTARView;

/**
 * Created by ngazcon on 21/11/2017.
 */

public class ListenerTTARViewDoubleTap extends GestureDetector.SimpleOnGestureListener {

    public ListenerTTARViewDoubleTap() {

    }

    @Override
    public boolean onDoubleTap(MotionEvent e) {
        MediatorTTARView.getInstance().resetTTARViewPictureInPictureSize();

        return true;
    }
}
