package vyglab.argeo.app.view;

import android.os.Bundle;
import android.support.v4.app.ListFragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ListView;

import vyglab.argeo.R;
import vyglab.argeo.app.controller.TTARViewListAdapter;
import vyglab.argeo.app.model.TTARView;
import vyglab.argeo.app.model.TTARViewRepository;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by root on 28/02/17.
 */

public class FragmentTTARViewList extends ListFragment implements TTARViewRepository.TTARViewRepositoryChanged {

    private static final String ARG_SECTION_NUMBER = "section_number";
    private int m_selected;
    private boolean m_item_selection_enabled;
    private TTARViewListAdapter m_ttarview_adapter;
    private List<FragmentTTARViewListItemStateChangedListener> m_list_item_state_listeners = new ArrayList<FragmentTTARViewListItemStateChangedListener>();

    //private MainActivityState m_main_activity_state;

    public interface FragmentTTARViewListItemStateChangedListener {
        void onTTARViewListItemSelected(TTARView item);
        void onTTARViewListItemUnselected();
    }

    public FragmentTTARViewList() {
        // Required empty public constructor
    }

    public static FragmentTTARViewList newInstance(int sectionNumber){
        FragmentTTARViewList fragment = new FragmentTTARViewList();
        Bundle args = new Bundle();
        args.putInt(ARG_SECTION_NUMBER, sectionNumber);

        return fragment;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View root = inflater.inflate(R.layout.fragment_ttarview_list, container, false);

        m_selected = -1;
        m_item_selection_enabled = true;

        m_ttarview_adapter = new TTARViewListAdapter(getActivity(), TTARViewRepository.getInstance().getTTARViews(), this);
        TTARViewRepository.getInstance().addListener(this);

        // Set adapter
        this.setListAdapter(m_ttarview_adapter);


        return root;
    }

    // TODO: Rename method, update argument and hook method into UI event
    /*
    public void POIListFrag  {
        if (mListener != null) {
            mListener.onFragmentInteraction(uri);
        }
    }*/
/*
    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
        if (context instanceof OnPOIListFragmentInteractionListener) {
            m_listener = (OnPOIListFragmentInteractionListener) context;
        } else {
            throw new RuntimeException(context.toString()
                    + " must implement OnFragmentInteractionListener");
        }
    }

    @Override
    public void onDetach() {
        super.onDetach();
        m_listener = null;
    }
*/
    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        getListView().setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {
            @Override
            public boolean onItemLongClick(AdapterView<?> adapterView, View view, int position, long id) {
                if (m_item_selection_enabled) {
                    if ( m_selected == -1 ) { // There is no item selected
                        m_selected = position;
                        getListView().invalidateViews(); // Para que se dibuje nuevamente
                        notifyTTARViewListItemSelected((TTARView) getListView().getAdapter().getItem(position));

                        return true;
                    }
                }

                return false;
            }
        });
    }

    /*public void setMainActivityState(MainActivityState state){
        m_main_activity_state = state;
    }*/

    @Override
    public void onListItemClick(ListView l, View view, int position, long id) {
        //super.onListItemClick(l, view, position, id);

        if (m_item_selection_enabled) {
            if ((m_selected != -1) && (m_selected == position)){
                m_selected = -1;
                getListView().invalidateViews();
                notifyTTARViewListItemUnselected();
            }
        }
    }

    /**
     * This interface must be implemented by activities that contain this
     * fragment to allow an interaction in this fragment to be communicated
     * to the activity and potentially other fragments contained in that
     * activity.
     * <p/>
     * See the Android Training lesson <a href=
     * "http://developer.android.com/training/basics/fragments/communicating.html"
     * >Communicating with Other Fragments</a> for more information.
     */
    /*public interface OnPOIListFragmentInteractionListener {
        // TODO: Update argument type and name
        void OnPOIListFragmentInteractionListener(Uri uri);
    }*/

    public int getSelected() {
        return  m_selected;
    }

    public void enableListInteraction() {
        m_item_selection_enabled = true;
    }

    public void disableListInteraction() {
        m_item_selection_enabled = false;
    }

    public void resetSelected(){
        if (m_selected != -1){
            m_selected = -1;
            getListView().invalidateViews();
        }
    }

    public void updateSelected() {
       getListView().invalidateViews();
    }

    @Override
    public void onTTARViewAdded(TTARView ttarview) {
        m_ttarview_adapter.add(ttarview);
    }

    @Override
    public void onTTARViewRemoved(TTARView ttarview) {
        m_ttarview_adapter.remove(ttarview);
    }

    // Methods for update location listeners
    public void addListener(FragmentTTARViewListItemStateChangedListener listener) {
        m_list_item_state_listeners.add(listener);
    }

    public void removeListener(FragmentTTARViewListItemStateChangedListener listener) {
        m_list_item_state_listeners.remove(listener);
    }

    void notifyTTARViewListItemSelected(TTARView item){
        for(FragmentTTARViewListItemStateChangedListener listener : m_list_item_state_listeners){
            listener.onTTARViewListItemSelected(item);
        }
    }

    void notifyTTARViewListItemUnselected(){
        for(FragmentTTARViewListItemStateChangedListener listener : m_list_item_state_listeners){
            listener.onTTARViewListItemUnselected();
        }
    }
}
