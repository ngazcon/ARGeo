package vyglab.argeo.app.controller.ListenersTTARView;

import android.app.Activity;
import android.graphics.Matrix;
import android.support.v4.view.MotionEventCompat;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageView;

import vyglab.argeo.R;
import vyglab.argeo.app.model.SketchFilter;
import vyglab.argeo.app.view.FragmentTTARView;

/**
 * Created by ngazcon on 12/07/2017.
 */

public class ListenerTTARViewTouchSketch implements View.OnTouchListener {

    private Activity m_activity;
    private FragmentTTARView m_fragment_ttarview;
    private SketchFilter m_sketch_filter;

    private ImageView m_imageview;

    //private int m_pointerid_rectangle;
    private boolean m_resizing = false;

    public ListenerTTARViewTouchSketch(Activity activity, FragmentTTARView fragment, SketchFilter filter) {
        m_activity = activity;
        m_fragment_ttarview = fragment;
        m_sketch_filter = filter;

        m_imageview = (ImageView) m_activity.findViewById(R.id.imageview_ttarview_picture_in_picture);
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        int action = MotionEventCompat.getActionMasked(event);

        switch (action) {
            case (MotionEvent.ACTION_DOWN):
                    //m_pointerid_rectangle = event.getPointerId(0);
                    m_resizing = false; // Just in case
                return true;

            case (MotionEvent.ACTION_MOVE):
                if (event.getPointerCount() > 1) {
                    // There are at least two pointers, so do the resize stuff
                    float x1 = event.getX(0); // 0-index is the first pointer (the one fired up ACTION_DOWN)
                    float x2 = event.getX(1); // I use the 1-index pointer, ignoring if there are more than two pointers
                    int dx = (int) (x1 - x2);
                    dx = Math.abs(dx);

                    float y1 = event.getY(0);
                    float y2 = event.getY(1);
                    int dy = (int) (y1 - y2);
                    dy = Math.abs(dy);

                    if (!m_resizing) {
                        // It is the first time, so I store de initial distance
                        m_resizing = true;
                        m_sketch_filter.startRectangleResize(dx, dy);
                    } else {
                        // Now resize according to the current distance
                        m_sketch_filter.updateRectangleSize(dx, dy);
                    }
                }
                drawRectangle(event.getX(), event.getY());
                return true;

            case (MotionEvent.ACTION_UP):
                drawRectangle(event.getX(), event.getY()); // Just to be sure it is showing the final rectangle that is going to be processed
                m_sketch_filter.cartoon();
                ImageView imgview = (ImageView) m_activity.findViewById(R.id.imageview_ttarview_picture_in_picture);
                imgview.setImageBitmap(m_sketch_filter.getOutputBitmap());
                return true;

            case (MotionEvent.ACTION_POINTER_UP):
                // Pointer other than main pointer is up, so resize is ended
                m_resizing = false;
                return true;

            case (MotionEvent.ACTION_CANCEL):
                return true;

            case (MotionEvent.ACTION_OUTSIDE):
                return true;

            default:
                return false;
        }
    }

    private void drawRectangle(float x, float y) {
        // 1-- Get the corresponding points from the image view to the bitmap
        float[] point = new float[] {x, y};
        Matrix inverse = new Matrix();
        m_imageview.getImageMatrix().invert(inverse);
        inverse.mapPoints(point);

        float density = m_activity.getResources().getDisplayMetrics().density;
        point[0] /= density;
        point[1] /= density;
        // -------------------

        // 2-- Update the rectangle on the bitmap
        m_sketch_filter.updateRectangle((int) point[1], (int) point[0]);

        // 3-- Set the bitmap to the image view
        m_imageview.setImageBitmap(m_sketch_filter.getRectangleBitmap());
    }
}
