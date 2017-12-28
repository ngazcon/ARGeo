package vyglab.argeo.app.utils;

import android.widget.Toast;

import vyglab.argeo.app.MainActivity;

/**
 * Created by ngazcon on 27/12/2017.
 */

public class MessagesManager {
    private static MessagesManager m_instance = null;
    private MainActivity m_activity;

    protected MessagesManager() {
        // Nothing to do
    }

    public static MessagesManager getInstance() {
        if (m_instance == null) {
            m_instance = new MessagesManager();
        }

        return m_instance;
    }

    public void init(MainActivity activity) {
        m_activity = activity;
    }

    public void showShortMessage(String message) {
        Toast.makeText(m_activity.getApplicationContext(), message, Toast.LENGTH_SHORT).show();
    }

    public void showShortMessage(int resource_id) {
        Toast.makeText(m_activity.getApplicationContext(), resource_id, Toast.LENGTH_SHORT).show();
    }

    public void showLongMessage(String message) {
        Toast.makeText(m_activity.getApplicationContext(), message, Toast.LENGTH_LONG).show();
    }

    public void showLongMessage(int resource_id) {
        Toast.makeText(m_activity.getApplicationContext(), resource_id, Toast.LENGTH_LONG).show();
    }
}
