package vyglab.argeo.app.controller.SecondaryFabStateMachine.TTARViewStates;

import android.support.design.widget.FloatingActionButton;
import android.view.View;

import vyglab.argeo.R;
import vyglab.argeo.app.controller.SecondaryFabStateMachine.SecondaryFabContext;
import vyglab.argeo.app.controller.SecondaryFabStateMachine.SecondaryFabState;

/**
 * Created by ngazcon on 08/03/2017.
 */

public class StateTTARViewPictureInPicture extends SecondaryFabState {

    public StateTTARViewPictureInPicture(FloatingActionButton fab_first, FloatingActionButton fab_Second)
    {
        super(fab_first, fab_Second, null, null, null, null);
    }

    @Override
    public void handle(){
        m_fab_first.setOnClickListener(m_listeners.get(Transitions.SECONDARY_FAB_1));

        m_fab_first.setImageResource(R.drawable.ic_menu_broken_image);

        m_fab_first.setVisibility(View.VISIBLE);
        m_fab_second.setVisibility(View.GONE);
    }

    @Override
    public void nextState(SecondaryFabContext context, View.OnClickListener caller){
        // nothing to do
    }

    public void nextState(SecondaryFabContext context, String transition_label){
        SecondaryFabState state = m_transitions.get(transition_label);
        context.setState(state);
    }
}
