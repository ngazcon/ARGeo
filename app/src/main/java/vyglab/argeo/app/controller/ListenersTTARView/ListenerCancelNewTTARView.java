package vyglab.argeo.app.controller.ListenersTTARView;

import android.app.Activity;
import android.view.View;

import vyglab.argeo.app.MainActivityState;
import vyglab.argeo.app.view.FragmentTTARView;

/**
 * Created by root on 16/02/17.
 */

public class ListenerCancelNewTTARView implements View.OnClickListener {
    private Activity m_activity;
    private MainActivityState m_main_activity_state;
    private FragmentTTARView m_fragment_ttarview;

    public ListenerCancelNewTTARView(Activity activity, MainActivityState activity_state, FragmentTTARView fragment_ttarview){
        m_activity = activity;
        m_main_activity_state = activity_state;
        m_fragment_ttarview = fragment_ttarview;
    }

    @Override
    public void onClick(View v){
        m_fragment_ttarview.cancelCreation();

        m_main_activity_state.getSecondaryFabContext().goNextState(this);
        m_main_activity_state.getSecondaryFabContext().getState().handle();
    }
}