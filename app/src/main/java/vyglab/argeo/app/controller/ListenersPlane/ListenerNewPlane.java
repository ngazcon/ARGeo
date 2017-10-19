package vyglab.argeo.app.controller.ListenersPlane;

import android.app.Activity;
import android.view.View;

import vyglab.argeo.app.MainActivityState;
import vyglab.argeo.app.controller.FabOnClickListener;
import vyglab.argeo.app.view.FragmentPlane;

/**
 * Created by root on 16/02/17.
 */

public class ListenerNewPlane implements View.OnClickListener {
    private Activity m_activity;
    private MainActivityState m_main_activity_state;
    FragmentPlane m_fragment_plane;
    private FabOnClickListener m_fab_listener;

    public ListenerNewPlane(Activity activity, MainActivityState activity_state, FragmentPlane fragment_plane, FabOnClickListener fab_listener){
        m_activity = activity;
        m_main_activity_state = activity_state;
        m_fragment_plane = fragment_plane;
        m_fab_listener = fab_listener;
    }

    @Override
    public void onClick(View v){
        // Check if right menu is open
        m_fab_listener.requestOpen();

        // Prepare fragment
        m_fragment_plane.setForCreation();

        // Set the touch mode
        m_main_activity_state.setTouchMode(MainActivityState.TouchMode.PLANE_CREATION);
        m_main_activity_state.getSecondaryFabContext().goNextState(this);
        m_main_activity_state.getSecondaryFabContext().getState().handle();
    }
}
