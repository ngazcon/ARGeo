package vyglab.argeo.app.controller;

import vyglab.argeo.app.MainActivityState;

/**
 * Created by root on 29/10/16.
 */

public interface TouchModeStateChangedListener {

    void onTouchModeStateChanged(MainActivityState.TouchMode new_state);
}
