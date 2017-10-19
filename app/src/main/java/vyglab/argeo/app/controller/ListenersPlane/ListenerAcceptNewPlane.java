package vyglab.argeo.app.controller.ListenersPlane;

import android.app.Activity;
import android.view.View;

import vyglab.argeo.jni.ArgeoFragment;
import vyglab.argeo.app.MainActivityState;
import vyglab.argeo.app.model.Plane;
import vyglab.argeo.app.model.PlaneRepository;
import vyglab.argeo.app.model.database.DBManager;
import vyglab.argeo.app.utils.HandyPlane;

/**
 * Created by root on 16/02/17.
 */

public class ListenerAcceptNewPlane implements View.OnClickListener {
    private Activity m_activity;
    private MainActivityState m_main_activity_state;
    private ArgeoFragment m_argeoFragment;
    private DBManager m_DBmanager;

    public ListenerAcceptNewPlane(Activity activity, MainActivityState activity_state, ArgeoFragment fragment, DBManager dbmanager){
        m_activity = activity;
        m_main_activity_state = activity_state;
        m_argeoFragment = fragment;
        m_DBmanager = dbmanager;
    }

    @Override
    public void onClick(View v){
        // Create the poi
        Plane plane = HandyPlane.getInstance().clonePlane();
        PlaneRepository.getInstance().addPlane(plane);
        HandyPlane.getInstance().clear(false);

        //tab_fragment_poi.cleanView();
        m_main_activity_state.setTouchMode(MainActivityState.TouchMode.DEFAULT);

        // Show POI list tab
        //pager.setCurrentItem(0);
        m_main_activity_state.getSecondaryFabContext().goNextState(this);
        m_main_activity_state.getSecondaryFabContext().getState().handle();
    }
}