package vyglab.argeo.app;

import vyglab.argeo.app.model.TTARView;

/**
 * Created by ngazcon on 27/10/2017.
 */

public class MediatorTTARView {
    private static MediatorTTARView m_instance = null;

    private MainActivity m_activity;

    protected MediatorTTARView() {
        // Nothing to do here
    }

    public static MediatorTTARView getInstance() {
        if (m_instance == null) {
            m_instance = new MediatorTTARView();
        }

        return m_instance;
    }

    public void init(MainActivity activity) {
        m_activity = activity;
    }


    public float getTTARViewAspectRatio() {
        return m_activity.getScreenAspectRatio();
    }

    public int getTTARViewWidth() {
        return m_activity.getTTARViewSnapshotWidth();
    }

    public int getTTARViewHeight() {
        return m_activity.getTTARViewSnapshotHeight();
    }

    public void showTTARView(TTARView ttarview) {
        m_activity.openPictureInPicture(ttarview);
    }

    public void closeTTARViewOnDeletion(TTARView ttarview) {
        if ( ttarview.equals(m_activity.getCurrentPicturInPictureTTARView()) ) {
            m_activity.closePictureInPicture();
        }
    }

    public void changeTTARViewPictureInPictureSize(int dw, int dh) {
        m_activity.updateTTARViewPictureInPictrueSize(dw, dh);
    }

    public void resetTTARViewPictureInPictureSize() {
        m_activity.resetTTARViewPictureInPictrueSize();
    }

    public int getTTARViewPictureInPictureWidth() {
        return m_activity.getTTARViewPictureInPuctureWidth();
    }

    public int getTTARViewPictureInPictureHeight() {
        return m_activity.getTTARViewPictureInPuctureHeight();
    }
}
