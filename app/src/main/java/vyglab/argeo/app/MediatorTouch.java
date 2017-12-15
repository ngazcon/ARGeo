package vyglab.argeo.app;

/**
 * Created by ngazcon on 14/12/2017.
 */

public class MediatorTouch {
    private static MediatorTouch m_instance = null;

    private MainActivityState m_activity_state;

    protected MediatorTouch() {
        // Nothing to do here
    }

    public static MediatorTouch getInstance() {
        if (m_instance == null) {
            m_instance = new MediatorTouch();
        }

        return m_instance;
    }

    public void init(MainActivityState activity_state) {
        m_activity_state = activity_state;
    }

    public void changeTouchMode(MainActivityState.TouchMode mode) {
        m_activity_state.setTouchMode(mode);
    }

}