package vyglab.argeo.app.controller.ListenersTerrain;

import android.app.Activity;
import android.view.View;
import android.widget.Toast;

import vyglab.argeo.jni.ArgeoFragment;
import vyglab.argeo.app.MainActivityState;

/**
 * Created by root on 05/12/16.
 */

public class ListenerCancelRotateCamera implements View.OnClickListener {
    private Activity m_activity;
    private MainActivityState m_main_activity_state;
    private ArgeoFragment m_argeoFragment;

    public ListenerCancelRotateCamera(Activity activity, MainActivityState activity_state, ArgeoFragment fragment){
        m_activity = activity;
        m_main_activity_state = activity_state;
        m_argeoFragment = fragment;
    }

    @Override
    public void onClick(View v){
        //TODO Seteos para el translate camara
        Toast.makeText(m_activity.getApplicationContext(), "Camera Rotation canceled!", Toast.LENGTH_SHORT).show();

        // Enable location provider
        m_argeoFragment.getViewer().getDevicePoseCameraController().enableLocationUpdates();

        // Restore camera state
        m_argeoFragment.getViewer().getScene().restoreCameraState();

        // Set the touch mode
        m_main_activity_state.setTouchMode(MainActivityState.TouchMode.DEFAULT);
        m_main_activity_state.getSecondaryFabContext().goNextState(this);
        m_main_activity_state.getSecondaryFabContext().getState().handle();
    }
}