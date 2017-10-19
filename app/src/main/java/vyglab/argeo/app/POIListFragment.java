package vyglab.argeo.app;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.ListFragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;
import android.widget.Toast;

import vyglab.argeo.R;
import vyglab.argeo.app.controller.PoiRepositoryChanged;
import vyglab.argeo.app.model.POI;
import vyglab.argeo.app.model.POIRepository;

/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link POIListFragment.OnPOIListFragmentInteractionListener} interface
 * to handle interaction events.
 */
public class POIListFragment extends ListFragment implements PoiRepositoryChanged {

    private static final String ARG_SECTION_NUMBER = "section_number";
    private MainActivityState m_main_activity_state;
    private int m_selected;
    private POIListAdapter m_poilist_adapter;

    public POIListFragment() {
        // Required empty public constructor
    }

    public static POIListFragment newInstance(int sectionNumber){
        POIListFragment fragment = new POIListFragment();
        Bundle args = new Bundle();
        args.putInt(ARG_SECTION_NUMBER, sectionNumber);

        return fragment;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View root = inflater.inflate(R.layout.fragment_poi_list, container, false);

        // Obtain the ListView
        //ListView list = (ListView) root.findViewById(R.id.);

        // Initialize the adapter
        m_poilist_adapter = new POIListAdapter(getActivity(), POIRepository.getInstance().getPOIs());

        // Set adapter
        //list.setAdapter(adapter);
        this.setListAdapter(m_poilist_adapter);

        POIRepository.getInstance().addListener(this);

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
    public void onListItemClick(ListView l, View v, int position, long id) {
        super.onListItemClick(l, v, position, id);

        //TODO Agregar el codigo para cdo selecciono un item poi
        POI poi = (POI) getListAdapter().getItem(position);
        double degrees;
        double minutes;
        double seconds;
        Double d;
        int degrees_int;
        int minutes_int;
        double lat = poi.getPosition().getLat();

        degrees = Math.floor(lat);
        minutes = Math.floor((lat - degrees) * 60);
        seconds = 3600 * (lat - degrees) - (60 * minutes);

        d = degrees;
        degrees_int = d.intValue();
        d = minutes;
        minutes_int = d.intValue();

        String text_lat = String.valueOf(degrees_int).concat("º ").concat(String.valueOf(minutes_int)).concat("' ").concat(String.format("%.2f", seconds)).concat("''");

        double lo = poi.getPosition().getLong();

        degrees = Math.floor(lo);
        minutes = Math.floor((lo - degrees) * 60);
        seconds = 3600 * (lo - degrees) - (60 * minutes);

        d = degrees;
        degrees_int = d.intValue();
        d = minutes;
        minutes_int = d.intValue();

        String text_long = String.valueOf(degrees_int).concat("º ").concat(String.valueOf(minutes_int)).concat("' ").concat(String.format("%.2f", seconds)).concat("''");
        String message = new String("Item clicked: " + poi.getId() + " Lat: " + text_lat + " Long: " + text_long);
        Toast.makeText(getActivity().getApplicationContext(), message, Toast.LENGTH_LONG).show();
        m_selected = position;
    }

    public void setMainActivityState(MainActivityState state){
        m_main_activity_state = state;
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

    //TODO prueba... sacar y arreglar
    public void resetSelected(){
        if (getView() != null) {
            getListView().setItemChecked(m_selected, false);
        }
    }

    @Override
    public void onPoiAdded(POI poi){
        m_poilist_adapter.add(poi);
    }
}
