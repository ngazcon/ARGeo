package vyglab.argeo.app.controller.SecondaryFabStateMachine;

import android.support.design.widget.FloatingActionButton;
import android.view.View;

import java.util.HashMap;
import java.util.LinkedHashMap;

/**
 * Created by root on 25/11/16.
 */

public abstract class SecondaryFabState {
    protected FloatingActionButton m_fab_first;
    protected FloatingActionButton m_fab_second;

    protected LinkedHashMap<String, SecondaryFabState> m_transitions;
    //protected String [] m_transitions_keys_array;
    protected HashMap<String, View.OnClickListener> m_listeners;

    public static class Transitions {
        public static final String SECONDARY_FAB_1 = "FAB_1";
        public static final String SECONDARY_FAB_2 = "FAB_2";
        public static final String EXTRA_MAINSTATE = "MAINSTATE"; // no lo uso, agrego un metodo clean
        public static final String EXTRA_INTERACTION_1 = "INTERACTION_1";
        public static final String EXTRA_INTERACTION_2 = "INTERACTION_2";
        public static final String EXTRA_INTERACTION_3 = "INTERACTION_3";
        public static final String EXTRA_INTERACTION_4 = "INTERACTION_4";
    }

    //TODO Legacy stuff to be refactored
    protected View.OnClickListener m_listener_first;
    protected View.OnClickListener m_listener_second;
    protected SecondaryFabState m_transition_first;
    protected SecondaryFabState m_transition_second;

    public SecondaryFabState(FloatingActionButton fab_first, FloatingActionButton fab_Second,
                             View.OnClickListener listener_first, View.OnClickListener listener_second,
                             SecondaryFabState transition_first, SecondaryFabState transition_second){
        m_fab_first = fab_first;
        m_fab_second = fab_Second;
        m_transitions = new LinkedHashMap<String, SecondaryFabState>();
        m_listeners = new HashMap<String, View.OnClickListener>();

        // Legacy
        m_listener_first = listener_first;
        m_listener_second = listener_second;
        m_transition_first = transition_first;
        m_transition_second = transition_second;
    }

    public SecondaryFabState(FloatingActionButton fab_first, FloatingActionButton fab_Second,
                             View.OnClickListener listener_first, View.OnClickListener listener_second){
        m_fab_first = fab_first;
        m_fab_second = fab_Second;
        m_listener_first = listener_first;
        m_listener_second = listener_second;

        m_transitions = new LinkedHashMap<String, SecondaryFabState>();
        m_listeners = new HashMap<String, View.OnClickListener>();
    }

    public abstract void handle();
    public abstract void nextState(SecondaryFabContext context, View.OnClickListener caller);
    public abstract void nextState(SecondaryFabContext context, String transition_label);

    public void addTransition(String transition_label, SecondaryFabState state, View.OnClickListener listener){
        m_transitions.put(transition_label, state);
        //m_transitions_keys_array = m_transitions.keySet().toArray(new String[m_transitions.keySet().size()] );
        m_listeners.put(transition_label, listener);
    }

    public void setTransitionFrist( SecondaryFabState state){
        m_transition_first = state;
    }

    public void setTransitionSecond( SecondaryFabState state){
        m_transition_second = state;
    }
}
