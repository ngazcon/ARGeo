package vyglab.argeo.app.utils;

import android.content.Context;
import android.content.SharedPreferences;

import vyglab.argeo.app.MainActivity;

/**
 * Created by ngazcon on 21/11/2017.
 */

public class PreferencesManager {
    private static final String SHARED_PREFERENCES_NAME = "ArgeoPreferences";

    private SharedPreferences m_preferences;
    private SharedPreferences.Editor m_editor;
    private MainActivity m_activity;

    public PreferencesManager(MainActivity activity) {
        m_activity = activity;
        m_preferences = m_activity.getSharedPreferences(SHARED_PREFERENCES_NAME, Context.MODE_PRIVATE);
        m_editor = m_preferences.edit();
    }

    public void putInt(int key, int value) {
        m_editor.putInt(m_activity.getString(key), value);
        m_editor.commit();
    }

    public int getInt(int key, int default_key) {
        int value = m_preferences.getInt(m_activity.getString(key), m_activity.getResources().getInteger(default_key));

        return value;
    }

    public int getIntProvidingDefault(int key, int default_value) {
        int value = m_preferences.getInt(m_activity.getString(key), default_value);

        return value;
    }
}
