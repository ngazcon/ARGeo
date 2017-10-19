package vyglab.argeo.app.controller.ListenersTerrain;

import android.app.Activity;
import android.view.View;
import android.widget.Toast;

import vyglab.argeo.jni.ArgeoFragment;
import vyglab.argeo.app.MainActivityState;

/**
 * Created by root on 05/12/16.
 */

public class ListenerAcceptRotateCamera implements View.OnClickListener {
    private Activity m_activity;
    private MainActivityState m_main_activity_state;
    private ArgeoFragment m_argeoFragment;

    public ListenerAcceptRotateCamera(Activity activity, MainActivityState activity_state, ArgeoFragment fragment){
        m_activity = activity;
        m_main_activity_state = activity_state;
        m_argeoFragment = fragment;
    }

    @Override
    public void onClick(View v){
        //TODO Codigo para el rotate camera
        Toast.makeText(m_activity.getApplicationContext(), "Camera Rotation accepted!", Toast.LENGTH_SHORT).show();

        // Enable location provider
        m_argeoFragment.getViewer().getDevicePoseCameraController().enableLocationUpdates();

        // State transition
        m_main_activity_state.setTouchMode(MainActivityState.TouchMode.DEFAULT);
        m_main_activity_state.getSecondaryFabContext().goNextState(this);
        m_main_activity_state.getSecondaryFabContext().getState().handle();
    }
}
