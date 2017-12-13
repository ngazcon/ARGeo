package vyglab.argeo.app.view;

import android.app.Activity;
import android.os.Bundle;
import android.support.design.widget.TabLayout;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentTransaction;
import android.support.v4.view.ViewPager;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import vyglab.argeo.jni.ArgeoFragment;
import vyglab.argeo.app.MainActivityState;
import vyglab.argeo.R;
import vyglab.argeo.app.controller.TabSectionsPagerAdapter;
import vyglab.argeo.app.model.Plane;

/**
 * Created by ngazcon on 09/03/2017.
 */

public class FragmentPlane extends Fragment implements FragmentPlaneList.FragmentPlaneListItemStateChangedListener {

    private ArgeoFragment m_argeo_fragment;
    private TabSectionsPagerAdapter m_tabs_adapter_plane;
    private FragmentPlaneList m_fragment_plane_list;
    private FragmentPlaneDetails m_fragment_plane_details;
    private ViewPager m_viewpager_plane;

    private MainActivityState m_main_activity_state;

    public FragmentPlane() {
        // Nothing to do here
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {

        View rootView = inflater.inflate(R.layout.fragment_plane, container, false);

        return rootView;
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        // 0-- Create tab section adapter
        m_tabs_adapter_plane = new TabSectionsPagerAdapter(getChildFragmentManager());

        // 1-- Create, initialize and add the fragment for TTARView list
        m_fragment_plane_list = FragmentPlaneList.newInstance(1);
        m_tabs_adapter_plane.addFragment(m_fragment_plane_list, "PLANES");

        // 2-- Create, initialize and add the fragment for TTARView creation/edition
        m_fragment_plane_details = FragmentPlaneDetails.newInstance(2);
        m_tabs_adapter_plane.addFragment(m_fragment_plane_details, "PROP");
        m_fragment_plane_details.setArgeoFragment(m_argeo_fragment);

        // 3-- Set the adapter to the viewpager
        m_viewpager_plane =  (ViewPager) view.findViewById(R.id.fragment_plane_pager);
        m_viewpager_plane.setAdapter(m_tabs_adapter_plane);

        // 4-- Set tabs
        TabLayout ttarview_tabs = (TabLayout) view.findViewById(R.id.fragment_plane_tabs);
        ttarview_tabs.setupWithViewPager(m_viewpager_plane);

        // FRAGMENT TTARView - inside fragment TTARView LIST
        m_fragment_plane_list.addListener(this);

        // FRAGMENT TTARView - inside fragment TTARView Details

        FragmentTransaction ft = getActivity().getSupportFragmentManager().beginTransaction();
        ft.hide(this);
        ft.commit();
    }

    @Override
    public void onStart() {
        super.onStart();
    }

    @Override
    public void onPause(){
        super.onPause();
    }

    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);

        // This makes sure that the container activity has implemented
        // the callback interface. If not, it throws an exception
        /*try {
            m_callback = (OnWireframeToggleChangedListener) activity;
        } catch (ClassCastException e) {
            throw new ClassCastException(activity.toString() + " must implement OnWireframeToggleChangedListener callbacks");
        }*/
    }

    public void setMainActivityState(MainActivityState state){
        m_main_activity_state = state;
    }

    public void setArgeoFragment(ArgeoFragment fragment){
        m_argeo_fragment = fragment;
    }

    /*public TTARView getCurrentTTARView() {
        return m_fragment_ttarview_details.getCurrentTTARView();
    }*/

    public void hideFragment() {
        FragmentTransaction ft = getActivity().getSupportFragmentManager().beginTransaction();
        ft.hide(this);
        ft.commit();
    }

    public void showFragment() {
        FragmentTransaction ft = getActivity().getSupportFragmentManager().beginTransaction();
        ft.show(this);
        ft.commit();
    }

    public void setForCreation(){
        m_fragment_plane_list.disableListInteraction();

        m_viewpager_plane.setCurrentItem(1);
        m_fragment_plane_details.setForCreation();
    }

    public void cancelCreation() {
        m_fragment_plane_list.enableListInteraction();

        m_viewpager_plane.setCurrentItem(0);
        m_fragment_plane_details.cleanView(false);
    }

    public Plane acceptCreation() {
        m_fragment_plane_list.enableListInteraction();

        m_viewpager_plane.setCurrentItem(0);
        Plane plane = m_fragment_plane_details.getPlaneFromView();
        m_fragment_plane_details.cleanView(false);

        return plane;
    }

    @Override
    public void onPlaneListItemSelected(Plane item) {
        if (m_fragment_plane_list.isListInteractionEnabled()) {
            //m_fragment_plane_details.loadTTARView(item);
            m_viewpager_plane.setCurrentItem(1);
            //m_main_activity_state.getSecondaryFabContext().goNextState(SecondaryFabState.Transitions.EXTRA_INTERACTION_1);
            //m_main_activity_state.getSecondaryFabContext().getState().handle();
        }
    }

    @Override
    public void onPlaneListItemUnselected() {
        if (m_fragment_plane_list.isListInteractionEnabled()) {
            //m_fragment_ttarview_details.cleanView();
            //m_main_activity_state.getSecondaryFabContext().goNextState(SecondaryFabState.Transitions.EXTRA_INTERACTION_1);
            //m_main_activity_state.getSecondaryFabContext().getState().handle();
        }
    }
}