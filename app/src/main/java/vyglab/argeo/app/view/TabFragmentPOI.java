package vyglab.argeo.app.view;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.EditorInfo;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.SeekBar;
import android.widget.TextView;

import vyglab.argeo.jni.ArgeoFragment;
import vyglab.argeo.jni.EllipsoidTransformations;
import vyglab.argeo.jni.Geocentric3D;
import vyglab.argeo.jni.Geodetic3D;
import vyglab.argeo.app.MainActivityState;
import vyglab.argeo.R;
import vyglab.argeo.app.controller.PositionPoiChanged;
import vyglab.argeo.app.model.POI;
import vyglab.argeo.app.model.PoiBillboardBuilder;
import vyglab.argeo.app.utils.HandyPOI;

/**
 * Created by root on 23/08/16.
 */
public class TabFragmentPOI extends Fragment implements PositionPoiChanged {
    private static final String ARG_SECTION_NUMBER = "section_number";

    private MainActivityState m_main_activity_state;
    private ArgeoFragment m_argeoFragment;

    private boolean clean_state = false;

    private String m_id;
    private String m_description;
    private double m_step;
    private double m_latitude;
    private double m_longitude;
    private double m_height;
    private double m_elevation;
    //private int m_spinner_billboard_icon;
    private PoiBillboardBuilder.BillboardIcon m_billboard_icon;
    private int m_seekbar_billboard_size;

    public static TabFragmentPOI newInstance(int sectionNumber){
        TabFragmentPOI fragment = new TabFragmentPOI();
        Bundle args = new Bundle();
        args.putInt(ARG_SECTION_NUMBER, sectionNumber);

        return fragment;
    }

    public TabFragmentPOI(){
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View rootView = inflater.inflate(R.layout.fragment_poi, container, false);

        //m_spinner_billboard_icon = 0;
        m_billboard_icon = PoiBillboardBuilder.BillboardIcon.DEFAULT;
        m_elevation = 0;
        m_seekbar_billboard_size = 50;

        EditText edittext = (EditText) rootView.findViewById(R.id.editText_poi_id);
        edittext.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int i, KeyEvent keyEvent) {
                switch (i){
                    case EditorInfo.IME_ACTION_NEXT :
                    case EditorInfo.IME_ACTION_PREVIOUS :
                    case EditorInfo.IME_ACTION_DONE :
                    case EditorInfo.IME_ACTION_SEND:
                        m_id = textView.getText().toString();
                        break;
                }

                return false;
            }
        });

        edittext = (EditText) rootView.findViewById(R.id.editText_poi_description);
        edittext.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int i, KeyEvent keyEvent) {
                switch (i){
                    case EditorInfo.IME_ACTION_NEXT :
                    case EditorInfo.IME_ACTION_PREVIOUS :
                    case EditorInfo.IME_ACTION_DONE :
                    case EditorInfo.IME_ACTION_SEND:
                        m_description = textView.getText().toString();
                        break;
                }

                return false;
            }
        });

        SeekBar seekbar_elevation = (SeekBar) rootView.findViewById(R.id.seekBar_elevation);
        seekbar_elevation.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                int value = i - 50;
                m_elevation = (double) value;
                changeSeekElevationTextValue(value);

                if (!clean_state) {
                    HandyPOI.getInstance().updateLocation(new Geodetic3D(m_latitude, m_longitude, m_height + m_elevation));
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        SeekBar seekbar_step = (SeekBar) rootView.findViewById(R.id.seekBar_step);
        seekbar_step.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                changeSeekBarStepTextValue(++i);
                m_step = i;
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        /*final Spinner spinner_billboard = (Spinner) rootView.findViewById(R.id.spinner_billboard);

        List<String> spinnerArray = new ArrayList<String>();
        spinnerArray.add("Marker");
        spinnerArray.add("Star");
        spinnerArray.add("Flag");

        ArrayAdapter<String> adapter = new ArrayAdapter<String>(getActivity().getBaseContext(), android.R.layout.simple_spinner_item, spinnerArray);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinner_billboard.setAdapter(adapter);

        spinner_billboard.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                m_spinner_billboard_icon = i;
                //spinner_billboard.setSelection(i);
                //HandyPOI.getInstance().updateBillboardIcon(getCorrespondingIcon(i));
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {

            }
        });*/

        RadioButton radioButton_billboard = (RadioButton) rootView.findViewById(R.id.radiobutton_billboard_marker);
        radioButton_billboard.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                onRadioButtonClicked(view);
            }
        });

        radioButton_billboard = (RadioButton) rootView.findViewById(R.id.radiobutton_billboard_star);
        radioButton_billboard.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                onRadioButtonClicked(view);
            }
        });

        radioButton_billboard = (RadioButton) rootView.findViewById(R.id.radiobutton_billboard_flag);
        radioButton_billboard.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                onRadioButtonClicked(view);
            }
        });

        SeekBar seekbar_billboard = (SeekBar) rootView.findViewById(R.id.seekBar_billboard_size);
        seekbar_billboard.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                m_seekbar_billboard_size = i + 50;
                changeSeekBarBillboardTextValue(m_seekbar_billboard_size);

                if (!clean_state) {
                    HandyPOI.getInstance().updateBillboardSize(m_seekbar_billboard_size);
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        return rootView;
    }

    @Override
    public void onStart(){
        super.onStart();
        cleanView();
    }

    private PoiBillboardBuilder.BillboardIcon getCorrespondingIcon(int spinner_item){
        PoiBillboardBuilder.BillboardIcon output = PoiBillboardBuilder.BillboardIcon.DEFAULT;

        switch (spinner_item){
            case 0 :
                output = PoiBillboardBuilder.BillboardIcon.DEFAULT;
                break;
            case 1 :
                output = PoiBillboardBuilder.BillboardIcon.STAR;
                break;
            case 2 :
                output = PoiBillboardBuilder.BillboardIcon.FLAG;
                break;
        }

        return output;
    }

    public void setMainActivityState(MainActivityState state){
        m_main_activity_state = state;
    }

    public void setArgeoFragment(ArgeoFragment fragment){
        m_argeoFragment = fragment;
    }

    private void changeSeekBarStepTextValue(int i){
        TextView textview_value = (TextView) getView().findViewById(R.id.textView_step);
        textview_value.setText(String.valueOf(i));
    }

    private void changeSeekElevationTextValue(int i){
        TextView textview_value = (TextView) getView().findViewById(R.id.textView_elevation);
        textview_value.setText(String.valueOf(i).concat("m"));
    }

    private void changeSeekBarBillboardTextValue(int i){
        TextView textview_value = (TextView) getView().findViewById(R.id.textView_billboard_size);
        textview_value.setText(String.valueOf(i));
    }

    public void cleanView(){
        clean_state = true;

        m_id = "";
        m_description = "";
        m_latitude = 0.0;
        m_longitude = 0.0;
        m_height = 0.0;
        m_elevation = 0.0;
        m_seekbar_billboard_size = 50;
        m_billboard_icon = PoiBillboardBuilder.BillboardIcon.DEFAULT;
        //TODO Hago algo con el step?
        //TODO Hago algo con el size?

        TextView textview = (TextView) getView().findViewById(R.id.textView_poi_lat);
        textview.setText("---");

        textview = (TextView) getView().findViewById(R.id.textView_poi_long);
        textview.setText("---");

        textview = (TextView) getView().findViewById(R.id.textView_poi_height);
        textview.setText("---");

        EditText edittext = (EditText) getView().findViewById(R.id.editText_poi_id);
        edittext.setText("");

        edittext = (EditText) getView().findViewById(R.id.editText_poi_description);
        edittext.setText("");

        SeekBar seekbar = (SeekBar) getView().findViewById(R.id.seekBar_elevation);
        seekbar.setProgress(50);
        seekbar.setEnabled(false);

        RadioButton radiobutton = (RadioButton) getView().findViewById(R.id.radiobutton_billboard_marker);
        radiobutton.setChecked(true);
        radiobutton.setEnabled(false);

        radiobutton = (RadioButton) getView().findViewById(R.id.radiobutton_billboard_star);
        radiobutton.setEnabled(false);

        radiobutton = (RadioButton) getView().findViewById(R.id.radiobutton_billboard_flag);
        radiobutton.setEnabled(false);

        seekbar = (SeekBar) getView().findViewById(R.id.seekBar_billboard_size);
        seekbar.setProgress(0);
        seekbar.setEnabled(false);

        clean_state = false;
    }

    protected String obtainTextFromLatOrLong(double coordinate){
        double degrees;
        double minutes;
        double seconds;
        Double d;
        int degrees_int;
        int minutes_int;

        if (Double.isNaN(coordinate))
            return new String("---");

        degrees = Math.floor(coordinate);
        minutes = Math.floor((coordinate - degrees) * 60);
        seconds = 3600 * (coordinate - degrees) - (60 * minutes);

        d = degrees;
        degrees_int = d.intValue();
        d = minutes;
        minutes_int = d.intValue();

        String text = String.valueOf(degrees_int).concat("º ").concat(String.valueOf(minutes_int)).concat("' ").concat(String.format("%.2f", seconds)).concat("''");

        return text;
    }

    protected void setPositionCoordinatesToTextViews(vyglab.argeo.jni.Geodetic3D position){
        m_latitude = position.getLatitude();
        m_longitude = position.getLongitude();
        m_height = position.getAltitude();

        TextView textview = (TextView) getView().findViewById(R.id.textView_poi_lat);
        textview.setText(obtainTextFromLatOrLong(m_latitude));

        textview = (TextView) getView().findViewById(R.id.textView_poi_long);
        textview.setText(obtainTextFromLatOrLong(m_longitude));

        textview = (TextView) getView().findViewById(R.id.textView_poi_height);
        textview.setText(String.valueOf(Math.round(m_height)).concat("m"));
    }

    public void setNewPoiView(){
        EditText edittext = (EditText) getView().findViewById(R.id.editText_poi_id);
        edittext.setEnabled(true);

        edittext = (EditText) getView().findViewById(R.id.editText_poi_description);
        edittext.setEnabled(true);

        SeekBar seekbar = (SeekBar) getView().findViewById(R.id.seekBar_elevation);
        seekbar.setEnabled(true);

        RadioButton radiobutton = (RadioButton) getView().findViewById(R.id.radiobutton_billboard_marker);
        radiobutton.setEnabled(true);

        radiobutton = (RadioButton) getView().findViewById(R.id.radiobutton_billboard_star);
        radiobutton.setEnabled(true);

        radiobutton = (RadioButton) getView().findViewById(R.id.radiobutton_billboard_flag);
        radiobutton.setEnabled(true);

        seekbar = (SeekBar) getView().findViewById(R.id.seekBar_billboard_size);
        seekbar.setEnabled(true);
    }

    public void setPoiView(POI poi){
        m_id = poi.getId();
        m_description = poi.getDescription();
        m_latitude = poi.getPosition().getLat();
        m_longitude = poi.getPosition().getLong();
        m_height = poi.getPosition().getHeight();
        m_elevation = 0.0;
        m_seekbar_billboard_size = 50;
        //TODO Hago algo con el step?
        //TODO Hago algo con el radio button?
        //TODO Hago algo con el size?

        TextView textview = (TextView) getView().findViewById(R.id.textView_poi_lat);
        textview.setText(vyglab.argeo.app.model.Geodetic3D.coordinateToPrintableText(m_latitude));

        textview = (TextView) getView().findViewById(R.id.textView_poi_long);
        textview.setText(vyglab.argeo.app.model.Geodetic3D.coordinateToPrintableText(m_longitude));

        textview = (TextView) getView().findViewById(R.id.textView_poi_height);
        textview.setText("---");

        EditText edittext = (EditText) getView().findViewById(R.id.editText_poi_id);
        edittext.setText("");

        edittext = (EditText) getView().findViewById(R.id.editText_poi_description);
        edittext.setText("");

        SeekBar seekbar = (SeekBar) getView().findViewById(R.id.seekBar_elevation);
        seekbar.setProgress(50);
        seekbar.setEnabled(false);

        RadioButton radiobutton = (RadioButton) getView().findViewById(R.id.radiobutton_billboard_marker);
        radiobutton.setEnabled(false);
        radiobutton.setChecked(true);

        radiobutton = (RadioButton) getView().findViewById(R.id.radiobutton_billboard_star);
        radiobutton.setEnabled(false);

        radiobutton = (RadioButton) getView().findViewById(R.id.radiobutton_billboard_flag);
        radiobutton.setEnabled(false);

        seekbar = (SeekBar) getView().findViewById(R.id.seekBar_billboard_size);
        seekbar.setProgress(0);
        seekbar.setEnabled(false);

        clean_state = false;
    }

    public void setEditPoiView(){
    }

    public POI getPoiFromView(){
        PoiBillboardBuilder builder = new PoiBillboardBuilder();
        builder.configurePoiBuilder(m_id, m_description, new vyglab.argeo.app.model.Geodetic3D(m_latitude, m_longitude, m_height));
        builder.configureBillboardBuilder(m_billboard_icon, m_seekbar_billboard_size, m_seekbar_billboard_size);
        builder.createNewPoi();

        //TODO - Chanchada
        /*PlaneGraphics p = new PlaneGraphics(new EllipsoidTangentPlane(
                new Geodetic3D(
                        builder.getPoi().getPosition().getLat(),
                        builder.getPoi().getPosition().getLong(),
                        builder.getPoi().getPosition().getHeight())));
        builder.getPoi().setGraphic(new Entity("plano", p));*/

        return builder.getPoi();
    }

    public void onRadioButtonClicked(View view){
        // Is the button now checked?
        boolean checked = ((RadioButton) view).isChecked();

        // Check which radio button was clicked
        switch(view.getId()) {
            case R.id.radiobutton_billboard_marker:
                if (checked)
                    m_billboard_icon = PoiBillboardBuilder.BillboardIcon.DEFAULT;
                break;

            case R.id.radiobutton_billboard_star:
                if (checked)
                    m_billboard_icon = PoiBillboardBuilder.BillboardIcon.STAR;
                break;

            case R.id.radiobutton_billboard_flag:
                if (checked)
                    m_billboard_icon = PoiBillboardBuilder.BillboardIcon.FLAG;
                break;
        }

        if (!clean_state) {
            HandyPOI.getInstance().updateBillboardIcon(m_billboard_icon);
        }
    }

    // Listener for poi position Changed
    @Override
    public void onPositionPoiChanged(){
        Geocentric3D geocentric = HandyPOI.getInstance().getPOI().getGraphic().getPosition();
        final vyglab.argeo.jni.Geodetic3D geodetic = EllipsoidTransformations.geodetic3DFromGeocentric3D(geocentric);
        //TODO Inspeccionar porque esta mierda evita el error con el acceso a la View desde un thread distinto... cdo no tengo idea por qué es otro thread
        getActivity().runOnUiThread(new Runnable() {
            @Override
            public void run() {
                setPositionCoordinatesToTextViews(geodetic);
            }
        });

        m_latitude = geodetic.getLatitude();
        m_longitude = geodetic.getLongitude();
        m_height = geodetic.getAltitude();
    }
}

