package vyglab.argeo.app.controller.UserInterfaceState;

import android.view.View;

/**
 * Created by ngazcon on 25/10/2017.
 */

public class UIContext {
    private UIState m_current_state;

    public UIContext() {

    }

    public void setState( UIState state) {
        m_current_state = state;
    }

    public UIState getState() {
        return  m_current_state;
    }

    public void next(int interaction) {
        m_current_state.nextState(this, interaction);
    }

    public void request() {
        m_current_state.handle();
    }

}
