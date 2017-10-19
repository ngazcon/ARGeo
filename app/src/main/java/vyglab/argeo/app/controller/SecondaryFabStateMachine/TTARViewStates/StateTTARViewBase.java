package vyglab.argeo.app.controller.SecondaryFabStateMachine.TTARViewStates;

import android.support.design.widget.FloatingActionButton;
import android.view.View;

import vyglab.argeo.R;
import vyglab.argeo.app.controller.SecondaryFabStateMachine.SecondaryFabContext;
import vyglab.argeo.app.controller.SecondaryFabStateMachine.SecondaryFabState;

/**
 * Created by root on 28/02/17.
 */

public class StateTTARViewBase extends SecondaryFabState {

    public StateTTARViewBase(FloatingActionButton fab_first, FloatingActionButton fab_Second,
                             View.OnClickListener listener_first, View.OnClickListener listener_second,
                             SecondaryFabState transition_first, SecondaryFabState transition_second){
        super(fab_first, fab_Second, listener_first, listener_second, transition_first, transition_second);
    }

    public StateTTARViewBase(FloatingActionButton fab_first, FloatingActionButton fab_Second,
                             View.OnClickListener listener_first, View.OnClickListener listener_second){
        super(fab_first, fab_Second, listener_first, listener_second);
        m_transition_first = null;
        m_transition_second = null;
    }

    @Override
    public void handle(){
        m_fab_first.setOnClickListener(m_listener_first);
        m_fab_second.setOnClickListener(m_listener_second);

        m_fab_first.setImageResource(R.drawable.ic_menu_add_photo);

        m_fab_first.setVisibility(View.VISIBLE);
        m_fab_second.setVisibility(View.GONE);
    }

    @Override
    public void nextState(SecondaryFabContext context, View.OnClickListener caller){
        if (caller == m_listener_first) {
            context.setState(m_transition_first);
        }
    }

    @Override
    public void nextState(SecondaryFabContext context, String transition_label){
        SecondaryFabState state = m_transitions.get(transition_label);
        context.setState(state);
    }
}
