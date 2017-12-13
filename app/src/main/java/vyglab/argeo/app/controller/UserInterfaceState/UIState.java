package vyglab.argeo.app.controller.UserInterfaceState;

import android.view.View;

import java.util.HashMap;

/**
 * Created by ngazcon on 25/10/2017.
 */

public abstract class UIState {
    protected HashMap<Integer, UIState> m_transitions;
    protected HashMap<Integer, View.OnClickListener> m_listeners;

    public static class Interactions {
        public static final int NO_INTERACTION = 0;
        public static final int FAB = 1;
        public static final int SECONDARY_FAB_1 = 2;
        public static final int SECONDARY_FAB_2 = 3;
        public static final int EXTRA_INTERACTION_1 = 4;
        public static final int EXTRA_INTERACTION_2 = 5;
        public static final int EXTRA_INTERACTION_3 = 6;
        public static final int EXTRA_INTERACTION_4 = 7;
    }

    public UIState() {
        m_transitions = new HashMap<Integer, UIState>();
        m_listeners = new HashMap<Integer, View.OnClickListener>();
    }

    public abstract void handle();
    public abstract void nextState(UIContext context, int interaction);
    public abstract void enterAction(int interaction);
    public abstract void exitAction(int interaction);

    public void addTransition(int transition_key, UIState state){
        m_transitions.put(transition_key, state);
    }

    public void addInteraction(Integer interaction_key, View.OnClickListener interaction) {
        m_listeners.put(interaction_key, interaction);
    }
}
