package vyglab.argeo.app;

import vyglab.argeo.app.model.TTARView;

/**
 * Created by ngazcon on 27/10/2017.
 */

public class MainActivityFacade {
    private static MainActivityFacade m_instance = null;

    private MainActivity m_activity;
    private float m_ttarview_snapshot_aspect_ratio;
    private int m_ttarview_snapshot_w;
    private int m_ttarview_snapshot_h;

    protected MainActivityFacade() {
        // Nothing to do here
    }

    public static MainActivityFacade getInstance() {
        if (m_instance == null) {
            m_instance = new MainActivityFacade();
        }

        return m_instance;
    }

    public void init(MainActivity activity) {
        m_activity = activity;
    }

    public void setTTARViewAspectRatio(float ratio) {
        m_ttarview_snapshot_aspect_ratio = ratio;
    }

    public void setTTARViewWidthHeight(int w, int h) {
        m_ttarview_snapshot_w = w;
        m_ttarview_snapshot_h = h;
    }

    public float getTTARViewAspectRatio() {
        return m_ttarview_snapshot_aspect_ratio;
    }

    public int getTTARViewWidth() {
        return m_ttarview_snapshot_w;
    }

    public int getTTARViewHeight() {
        return m_ttarview_snapshot_h;
    }

    public void showTTARView(TTARView ttarview) {
        m_activity.openPictureInPicture(ttarview);
    }
}
