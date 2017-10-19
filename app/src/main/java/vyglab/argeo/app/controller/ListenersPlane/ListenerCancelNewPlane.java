package vyglab.argeo.app.controller.ListenersPlane;

import android.app.Activity;
import android.view.View;

import vyglab.argeo.app.MainActivityState;
import vyglab.argeo.app.utils.HandyPlane;

/**
 * Created by root on 16/02/17.
 */

public class ListenerCancelNewPlane implements View.OnClickListener {
    private Activity m_activity;
    private MainActivityState m_main_activity_state;

    public ListenerCancelNewPlane(Activity activity, MainActivityState activity_state){
        m_activity = activity;
        m_main_activity_state = activity_state;
    }

    @Override
    public void onClick(View v){
        // Get the fragment for poi edition
        //ViewPager pager = (ViewPager) m_activity.findViewById(R.id.pager);
        //TabSectionsPagerAdapter tab_adapter = (TabSectionsPagerAdapter) pager.getAdapter();
        //TabFragmentPOI tab_fragment_poi = (TabFragmentPOI) tab_adapter.getItem(1);

        // Setup the fragment for poi edition
        //tab_fragment_poi.cleanView();
        HandyPlane.getInstance().clear(false);

        // Set the touch mode
        m_main_activity_state.setTouchMode(MainActivityState.TouchMode.DEFAULT);
        //HandyPOI.getInstance().removeListener(tab_fragment_poi);

        // Show POI list tab
        //pager.setCurrentItem(0);
        m_main_activity_state.getSecondaryFabContext().goNextState(this);
        m_main_activity_state.getSecondaryFabContext().getState().handle();
    }
}