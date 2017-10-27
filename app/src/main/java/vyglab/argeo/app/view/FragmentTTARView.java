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

import vyglab.argeo.app.MainActivityFacade;
import vyglab.argeo.app.controller.UserInterfaceState.UIContext;
import vyglab.argeo.app.controller.UserInterfaceState.UIContextManager;
import vyglab.argeo.app.controller.UserInterfaceState.UIState;
import vyglab.argeo.jni.ArgeoFragment;
import vyglab.argeo.app.MainActivityState;
import vyglab.argeo.R;
import vyglab.argeo.app.controller.SecondaryFabStateMachine.SecondaryFabState;
import vyglab.argeo.app.controller.TabSectionsPagerAdapter;
import vyglab.argeo.app.model.TTARView;

/**
 * Created by root on 28/02/17.
 */

public class FragmentTTARView extends Fragment
        implements FragmentTTARViewList.FragmentTTARViewListItemStateChangedListener{

    private ArgeoFragment m_argeo_fragment;
    private TabSectionsPagerAdapter m_tabs_adapter_ttarview;
    private FragmentTTARViewList m_fragment_ttarview_list;
    private FragmentTTARViewDetails m_fragment_ttarview_details;
    private ViewPager m_viewpager_ttarview;

    private MainActivityState m_main_activity_state;

    private boolean m_list_interaction_enabled;

    public FragmentTTARView() {
        // Nothing to do here
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        m_list_interaction_enabled = true;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {

        View rootView = inflater.inflate(R.layout.fragment_ttarview, container, false);

        return rootView;
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        // 0-- Create tab section adapter
        m_tabs_adapter_ttarview = new TabSectionsPagerAdapter(getChildFragmentManager());

        // 1-- Create, initialize and add the fragment for TTARView list
        m_fragment_ttarview_list = FragmentTTARViewList.newInstance(1);
        m_tabs_adapter_ttarview.addFragment(m_fragment_ttarview_list, "TT AR VIEWS");
        //m_fragment_ttarview_list.setMainActivityState(m_main_activity_state);

        // 2-- Create, initialize and add the fragment for TTARView creation/edition
        m_fragment_ttarview_details = FragmentTTARViewDetails.newInstance(2);
        m_tabs_adapter_ttarview.addFragment(m_fragment_ttarview_details, "PROP");
        m_fragment_ttarview_details.setArgeoFragment(m_argeo_fragment);
        //m_fragment_ttarview_details.setTTARViewWandH(m_ttarview_w, m_ttarview_h);

        // 3-- Set the adapter to the viewpager
        m_viewpager_ttarview =  (ViewPager) view.findViewById(R.id.ttarview_pager);
        m_viewpager_ttarview.setAdapter(m_tabs_adapter_ttarview);

        // 4-- Set tabs
        TabLayout ttarview_tabs = (TabLayout) view.findViewById(R.id.ttarview_tabs);
        ttarview_tabs.setupWithViewPager(m_viewpager_ttarview);

        // FRAGMENT TTARView - inside fragment TTARView LIST
        m_fragment_ttarview_list.addListener(this);

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

    public void setTTARViewWidthAndHeight(int w, int h) {
        m_fragment_ttarview_details.setTTARViewWandH(w,h);
    }

    public TTARView getCurrentTTARView() {
        return m_fragment_ttarview_details.getCurrentTTARView();
    }

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

    public void setForCreation(int ttarview_snapshot_w, int ttarview_snapshot_h){
        //Unable select/Unselect from TTARViewList
        m_list_interaction_enabled = false;
        m_fragment_ttarview_list.disableListInteraction();

        m_viewpager_ttarview.setCurrentItem(1);
        m_fragment_ttarview_details.setForCreation(ttarview_snapshot_w, ttarview_snapshot_h);
    }

    public void cancelCreation() {
        //Enable select/Unselect from TTARViewList
        m_list_interaction_enabled = true;
        m_fragment_ttarview_list.enableListInteraction();

        m_viewpager_ttarview.setCurrentItem(0);
        m_fragment_ttarview_details.cleanView();
    }

    public TTARView acceptCreation() {
        //Enable select/Unselect from TTARViewList
        m_list_interaction_enabled = true;
        m_fragment_ttarview_list.enableListInteraction();

        m_viewpager_ttarview.setCurrentItem(0);
        TTARView ttarview = m_fragment_ttarview_details.getTTARViewFromView();
        m_fragment_ttarview_details.cleanView();

        return ttarview;
    }

    public void prepareForPictureInPictureARView() {
        m_fragment_ttarview_details.prepareForPictureInPictureARView();
        MainActivityFacade.getInstance().showTTARView(m_fragment_ttarview_details.getCurrentTTARView());
    }

    @Override
    public void onTTARViewListItemSelected(TTARView item) {
        if (m_list_interaction_enabled) {
            m_fragment_ttarview_details.loadTTARView(item);
            m_viewpager_ttarview.setCurrentItem(1);
            //m_main_activity_state.getSecondaryFabContext().goNextState(SecondaryFabState.Transitions.EXTRA_INTERACTION_1);
            //m_main_activity_state.getSecondaryFabContext().getState().handle();
            UIContextManager.getInstance().next(MainActivityState.ApplicationMode.TTARVIEW, UIState.Interactions.EXTRA_INTERACTION_1);
            UIContextManager.getInstance().request(MainActivityState.ApplicationMode.TTARVIEW);
        }
    }

    @Override
    public void onTTARViewListItemUnselected() {
        if (m_list_interaction_enabled) {
            m_fragment_ttarview_details.cleanView();
            //m_main_activity_state.getSecondaryFabContext().goNextState(SecondaryFabState.Transitions.EXTRA_INTERACTION_1);
            //m_main_activity_state.getSecondaryFabContext().getState().handle();
            UIContextManager.getInstance().next(MainActivityState.ApplicationMode.TTARVIEW, UIState.Interactions.EXTRA_INTERACTION_2);
            UIContextManager.getInstance().request(MainActivityState.ApplicationMode.TTARVIEW);
        }
    }
}
