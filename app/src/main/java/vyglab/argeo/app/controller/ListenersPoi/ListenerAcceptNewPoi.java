package vyglab.argeo.app.controller.ListenersPoi;

import android.app.Activity;
import android.support.v4.view.ViewPager;
import android.view.View;

import vyglab.argeo.jni.ArgeoFragment;
import vyglab.argeo.app.MainActivityState;
import vyglab.argeo.R;
import vyglab.argeo.app.controller.TabSectionsPagerAdapter;
import vyglab.argeo.app.model.POI;
import vyglab.argeo.app.model.POIRepository;
import vyglab.argeo.app.model.database.DBManager;
import vyglab.argeo.app.utils.HandyPOI;
import vyglab.argeo.app.view.TabFragmentPOI;

/**
 * Created by ngazcon on 20/10/2016.
 */

public class ListenerAcceptNewPoi implements View.OnClickListener {
    private Activity m_activity;
    private MainActivityState m_main_activity_state;
    private ArgeoFragment m_argeoFragment;
    private DBManager m_DBmanager;

    public ListenerAcceptNewPoi(Activity activity, MainActivityState activity_state, ArgeoFragment fragment, DBManager dbmanager){
        m_activity = activity;
        m_main_activity_state = activity_state;
        m_argeoFragment = fragment;
        m_DBmanager = dbmanager;
    }

    @Override
    public void onClick(View v){
        // Get the fragment for poi edition
        ViewPager pager = (ViewPager) m_activity.findViewById(R.id.pager);
        TabSectionsPagerAdapter tab_adapter = (TabSectionsPagerAdapter) pager.getAdapter();
        TabFragmentPOI tab_fragment_poi = (TabFragmentPOI) tab_adapter.getItem(1);

        // Create the poi
        POI poi = tab_fragment_poi.getPoiFromView();
        POIRepository.getInstance().addPOI(poi);
        HandyPOI.getInstance().clear();
        //TODO la siguiente es una solucion de compromiso, pensar bien como modelarlo (donde se hace)
        m_DBmanager.insertPoi(poi);
        m_argeoFragment.getViewer().getEntities().add(poi.getGraphic());

        tab_fragment_poi.cleanView();
        m_main_activity_state.setTouchMode(MainActivityState.TouchMode.DEFAULT);
        HandyPOI.getInstance().removeListener(tab_fragment_poi);

        // Show POI list tab
        pager.setCurrentItem(0);
        m_main_activity_state.getSecondaryFabContext().goNextState(this);
        m_main_activity_state.getSecondaryFabContext().getState().handle();
    }
}
