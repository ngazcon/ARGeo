package vyglab.argeo.app.view;

import android.support.v4.app.ListFragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ListView;

import vyglab.argeo.R;
import vyglab.argeo.app.controller.PlaneListAdapter;
import vyglab.argeo.app.model.Plane;
import vyglab.argeo.app.model.PlaneRepository;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by ngazcon on 09/03/2017.
 */

public class FragmentPlaneList extends ListFragment implements PlaneRepository.PlaneRepositoryChanged {

    private static final String ARG_SECTION_NUMBER = "section_number";
    private int m_selected;
    private boolean m_item_selection_enabled;
    private PlaneListAdapter m_plane_adapter;
    private List<FragmentPlaneListItemStateChangedListener> m_list_item_state_listeners = new ArrayList<FragmentPlaneListItemStateChangedListener>();

    public interface FragmentPlaneListItemStateChangedListener {
        void onPlaneListItemSelected(Plane item);
        void onPlaneListItemUnselected();
    }

    public FragmentPlaneList() {
        // Required empty public constructor
    }

    public static FragmentPlaneList newInstance(int sectionNumber){
        FragmentPlaneList fragment = new FragmentPlaneList();
        Bundle args = new Bundle();
        args.putInt(ARG_SECTION_NUMBER, sectionNumber);

        return fragment;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View root = inflater.inflate(R.layout.fragment_plane_list, container, false);

        m_selected = -1;
        m_item_selection_enabled = true;

        /*
        List<Plane> objects = new ArrayList<Plane>();
        Plane p = new Plane("id-1", "jonsi", "descripcion jonsi");
        p.setPosition(new Geodetic3D(10.0, 11.0, 12.0));
        objects.add(p);
        p = new Plane("id-2", "pancer", "descripcion pancer");
        p.setPosition(new Geodetic3D(10.0, 11.0, 12.0));
        objects.add(p);
        p = new Plane("id-3", "jacer", "descripcion jacer");
        p.setPosition(new Geodetic3D(10.0, 11.0, 12.0));
        objects.add(p);
        m_plane_adapter = new PlaneListAdapter(getActivity(), objects, this);
        */
        m_plane_adapter = new PlaneListAdapter(getActivity(), PlaneRepository.getInstance().getPlanes(), this);
        PlaneRepository.getInstance().addListener(this);

        // Set adapter
        this.setListAdapter(m_plane_adapter);

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
                if ( m_selected == -1 ) { // There is no item selected
                    m_selected = position;
                    getListView().invalidateViews(); // Para que se dibuje nuevamente
                    notifyPlaneListItemSelected((Plane) getListView().getAdapter().getItem(position));

                    return true;
                }

                return false;
            }
        });
    }

    @Override
    public void onListItemClick(ListView l, View view, int position, long id) {
        //super.onListItemClick(l, view, position, id);

        if ((m_selected != -1) && (m_selected == position)){
            m_selected = -1;
            getListView().invalidateViews();
            notifyPlaneListItemUnselected();
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

    public boolean isListInteractionEnabled() {
        return m_item_selection_enabled;
    }


    @Override
    public void onPlaneAdded(Plane plane) {
        m_plane_adapter.add(plane);
    }

    // Methods for update location listeners
    public void addListener(FragmentPlaneListItemStateChangedListener listener) {
        m_list_item_state_listeners.add(listener);
    }

    public void removeListener(FragmentPlaneListItemStateChangedListener listener) {
        m_list_item_state_listeners.remove(listener);
    }

    void notifyPlaneListItemSelected(Plane item){
        for(FragmentPlaneListItemStateChangedListener listener : m_list_item_state_listeners){
            listener.onPlaneListItemSelected(item);
        }
    }

    void notifyPlaneListItemUnselected(){
        for(FragmentPlaneListItemStateChangedListener listener : m_list_item_state_listeners){
            listener.onPlaneListItemUnselected();
        }
    }
}