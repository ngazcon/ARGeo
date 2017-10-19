package vyglab.argeo.app.controller.ListenersPoi;

import android.app.Activity;
import android.support.v4.view.ViewPager;
import android.view.View;

import vyglab.argeo.app.MainActivityState;
import vyglab.argeo.R;

import vyglab.argeo.app.controller.FabOnClickListener;
import vyglab.argeo.app.controller.TabSectionsPagerAdapter;
import vyglab.argeo.app.utils.HandyPOI;
import vyglab.argeo.app.view.TabFragmentPOI;

/**
 * Created by ngazcon on 20/10/2016.
 */

public class ListenerNewPoi implements View.OnClickListener {
    private Activity m_activity;
    private MainActivityState m_main_activity_state;
    private FabOnClickListener m_fab_listener;

    public ListenerNewPoi(Activity activity, MainActivityState activity_state, FabOnClickListener fab_listener){
        m_activity = activity;
        m_main_activity_state = activity_state;
        m_fab_listener = fab_listener;
    }

    @Override
    public void onClick(View v){
        // Check if right menu is open
        m_fab_listener.requestOpen();

        // Show POI creation tab
        ViewPager pager = (ViewPager) m_activity.findViewById(R.id.pager);
        pager.setCurrentItem(1);

        // Get the fragment for poi edition
        TabSectionsPagerAdapter tab_adapter = (TabSectionsPagerAdapter) pager.getAdapter();
        TabFragmentPOI tab_fragment_poi = (TabFragmentPOI) tab_adapter.getItem(1);

        // Setup the fragment for poi edition
        tab_fragment_poi.cleanView();
        tab_fragment_poi.setNewPoiView();
        HandyPOI.getInstance().addListener(tab_fragment_poi);

        // Set the touch mode
        m_main_activity_state.setTouchMode(MainActivityState.TouchMode.POI_CREATION);
        m_main_activity_state.getSecondaryFabContext().goNextState(this);
        m_main_activity_state.getSecondaryFabContext().getState().handle();
    }
}
