package vyglab.argeo.app.view;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.widget.SwitchCompat;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.TextView;

import vyglab.argeo.app.model.Geodetic3D;
import vyglab.argeo.app.model.PlaneBuilder;
import vyglab.argeo.jni.ArgeoFragment;
import vyglab.argeo.R;
import vyglab.argeo.app.model.Plane;
import vyglab.argeo.app.utils.HandyPlane;
import vyglab.argeo.jni.EllipsoidTransformations;
import vyglab.argeo.jni.Geocentric3D;

/**
 * Created by root on 16/02/17.
 */

public class FragmentPlaneDetails extends Fragment
        implements HandyPlane.PlaneChanged {
    private static final String ARG_SECTION_NUMBER = "section_number";

    private ArgeoFragment m_argeo_fragment;

    private String m_name;
    private String m_description;
    private double m_latitude;
    private double m_longitude;
    private double m_height;
    private int m_seekbar_virtual_orientation;
    private int m_seekbar_dip;
    private int m_seekbar_strike;
    private int m_seekbar_size;
    private int m_seekbar_thickness;
    private boolean m_show_orientation_plane;
    private Plane m_current_plane_item;

    public static FragmentPlaneDetails newInstance(int sectionNumber){
        FragmentPlaneDetails fragment = new FragmentPlaneDetails();
        Bundle args = new Bundle();
        args.putInt(ARG_SECTION_NUMBER, sectionNumber);

        return fragment;
    }

    public FragmentPlaneDetails(){
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View rootView = inflater.inflate(R.layout.fragment_plane_details, container, false);

        return rootView;
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        SeekBar seekbar = (SeekBar) getView().findViewById(R.id.seekbar_fragment_plane_details_virtual_orientation);
        seekbar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                m_seekbar_virtual_orientation = i;
                TextView textview = (TextView) getView().findViewById(R.id.textview_fragment_plane_details_virtual);
                textview.setText(String.valueOf(i));
                HandyPlane.getInstance().updatePlaneVirtualOrientation(m_seekbar_virtual_orientation);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        seekbar = (SeekBar) getView().findViewById(R.id.seekBar_fragment_plane_details_dip);
        seekbar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                m_seekbar_dip = dipSeekBarToValue(i);
                TextView textview = (TextView) getView().findViewById(R.id.textview_fragment_plane_details_dip);
                textview.setText(String.valueOf(m_seekbar_dip));
                HandyPlane.getInstance().updatePlaneDip(m_seekbar_dip);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        seekbar = (SeekBar) getView().findViewById(R.id.seekbar_fragment_plane_details_strike);
        seekbar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                m_seekbar_strike = i * 5;
                TextView textview = (TextView) getView().findViewById(R.id.textview_fragment_plane_details_strike);
                textview.setText(String.valueOf(m_seekbar_strike));
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        seekbar = (SeekBar) getView().findViewById(R.id.seekbar_fragment_plane_details_size);
        seekbar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                m_seekbar_size = i + 1 ;
                TextView textview = (TextView) getView().findViewById(R.id.textview_fragment_plane_details_size);
                textview.setText(String.valueOf(m_seekbar_size));
                HandyPlane.getInstance().updatePlaneSize(m_seekbar_size);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        seekbar = (SeekBar) getView().findViewById(R.id.seekbar_fragment_plane_details_thickness);
        seekbar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                m_seekbar_thickness = i + 1 ;
                TextView textview = (TextView) getView().findViewById(R.id.textview_fragment_plane_details_thickness);
                textview.setText(String.valueOf(m_seekbar_thickness));
                HandyPlane.getInstance().updatePlaneThickness(m_seekbar_thickness);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        SwitchCompat switch_button = (SwitchCompat) getView().findViewById(R.id.switchcompat_fragment_plane_virtual_orientation_plane);
        switch_button.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                m_show_orientation_plane = b;
                HandyPlane.getInstance().updateShowVirtualOrientationPlane(m_show_orientation_plane);
            }
        });

        cleanView(false);
    }

    public void setArgeoFragment(ArgeoFragment fragment){
        m_argeo_fragment = fragment;
    }

    public Plane getCurrentPlane() {
        return m_current_plane_item;
    }

    protected int dipValueToSeekBar(int dip_value) {
        return 90 + dip_value;
    }

    protected int dipSeekBarToValue(int dip_seekbar) {
        return dip_seekbar - 90;
    }

    public void cleanView(boolean enable_value){
        m_name = "";
        m_description = "";
        m_latitude = 0.0;
        m_longitude = 0.0;
        m_height = 0.0;
        m_seekbar_virtual_orientation = 0;
        m_seekbar_dip = 0;
        m_seekbar_strike = 0;
        m_seekbar_size = 30;
        m_seekbar_thickness = 1;
        m_show_orientation_plane = true;

        EditText edittext = (EditText) getView().findViewById(R.id.edittext_fragment_plane_details_name);
        edittext.setText("");
        edittext.setEnabled(enable_value);

        edittext = (EditText) getView().findViewById(R.id.edittext_fragment_plane_details_description);
        edittext.setText("");
        edittext.setEnabled(enable_value);

        TextView textview = (TextView) getView().findViewById(R.id.textview_fragment_plane_details_lat);
        textview.setText("---");
        textview.setEnabled(enable_value);

        textview = (TextView) getView().findViewById(R.id.textview_fragment_plane_details_long);
        textview.setText("---");
        textview.setEnabled(enable_value);

        textview = (TextView) getView().findViewById(R.id.textview_fragment_plane_details_height);
        textview.setText("---");
        textview.setEnabled(enable_value);

        SeekBar seekbar = (SeekBar) getView().findViewById(R.id.seekbar_fragment_plane_details_virtual_orientation);
        seekbar.setProgress(0);
        seekbar.setEnabled(enable_value);

        seekbar = (SeekBar) getView().findViewById(R.id.seekBar_fragment_plane_details_dip);
        seekbar.setProgress(dipValueToSeekBar(0));
        seekbar.setEnabled(enable_value);

        seekbar = (SeekBar) getView().findViewById(R.id.seekbar_fragment_plane_details_strike);
        seekbar.setProgress(0);
        seekbar.setEnabled(enable_value);

        seekbar = (SeekBar) getView().findViewById(R.id.seekbar_fragment_plane_details_size);
        seekbar.setProgress(m_seekbar_size - 1);
        seekbar.setEnabled(enable_value);

        seekbar = (SeekBar) getView().findViewById(R.id.seekbar_fragment_plane_details_thickness);
        seekbar.setProgress(0);
        seekbar.setEnabled(enable_value);

        textview = (TextView) getView().findViewById(R.id.textview_fragment_plane_details_virtual);
        textview.setEnabled(enable_value);

        textview = (TextView) getView().findViewById(R.id.textview_fragment_plane_details_dip);
        textview.setEnabled(enable_value);

        textview = (TextView) getView().findViewById(R.id.textview_fragment_plane_details_strike);
        textview.setEnabled(enable_value);

        textview = (TextView) getView().findViewById(R.id.textview_fragment_plane_details_size);
        textview.setEnabled(enable_value);

        textview = (TextView) getView().findViewById(R.id.textview_fragment_plane_details_thickness);
        textview.setEnabled(enable_value);

        SwitchCompat switch_virtual_orientation_plane = (SwitchCompat) getView().findViewById(R.id.switchcompat_fragment_plane_virtual_orientation_plane);
        switch_virtual_orientation_plane.setChecked(true);
        switch_virtual_orientation_plane.setEnabled(enable_value);
    }

    public void loadPlane(Plane item) {
        /*
        TextView textview = (TextView) getView().findViewById(R.id.textView_ttarview_lat);
        textview.setText(Geodetic3D.coordinateToPrintableText(item.getPosition().getLat()));

        textview = (TextView) getView().findViewById(R.id.textView_ttarview_long);
        textview.setText(Geodetic3D.coordinateToPrintableText(item.getPosition().getLong()));

        textview = (TextView) getView().findViewById(R.id.textView_ttarview_height);
        textview.setText(Geodetic3D.heightToPrintableText(item.getPosition().getHeight()));

        EditText edittext = (EditText) getView().findViewById(R.id.editText_ttarview_name);
        edittext.setText(item.getName());

        edittext = (EditText) getView().findViewById(R.id.editText_ttarview_description);
        edittext.setText(item.getDescription());

        ImageButton imagebutton = (ImageButton) getView().findViewById(R.id.imagebutton_open_initial_ttarview);
        imagebutton.setEnabled(true);

        ImageView imageview = (ImageView) this.getView().findViewById(R.id.imageview_ttarview_details_initial_image);
        imageview.setImageBitmap(item.getInitialView());

        imageview = (ImageView) this.getView().findViewById(R.id.imageview_ttarview_details_updated_image);
        imageview.setImageBitmap(item.getUpdatedView());

        Button button = (Button) getView().findViewById(R.id.button_ttarview_delete);
        button.setEnabled(true);

        m_current_ttarview_item  = item;
        */
    }

    public Plane getPlaneFromView() {
        PlaneBuilder builder = new PlaneBuilder(m_argeo_fragment);

        // Obtain values from the view
        EditText edittext = (EditText) getView().findViewById(R.id.edittext_fragment_plane_details_name);
        m_name = edittext.getText().toString();

        edittext = (EditText) getView().findViewById(R.id.edittext_fragment_plane_details_description);
        m_description = edittext.getText().toString();

        // Build the plane
        Plane plane = builder.setId("id" + m_name)
                .setName(m_name)
                .setDescription(m_description)
                .setPosition(HandyPlane.getInstance().getCurrentGeocentric3D())
                .setVirtualOrientation(m_seekbar_virtual_orientation)
                .setDip(m_seekbar_dip)
                .setStrike(m_seekbar_strike)
                .setSize(m_seekbar_size)
                .setThickness(m_seekbar_thickness)
                .setShowVirtualOrientationPlane(m_show_orientation_plane)
                .build();

        return plane;
    }

    public void setForCreation(){
        cleanView(true);
    }

    //region PlaneChanged listeners
    @Override
    public void onPlanePositionChanged() {
        Geocentric3D geocentric = HandyPlane.getInstance().getCurrentGeocentric3D();
        final vyglab.argeo.jni.Geodetic3D geodetic = EllipsoidTransformations.geodetic3DFromGeocentric3D(geocentric);

        // Need to use runnable, otherwise app explodes: ilegal access from thread
        getActivity().runOnUiThread(new Runnable() {
            @Override
            public void run() {
                TextView textview = (TextView) getView().findViewById(R.id.textview_fragment_plane_details_lat);
                textview.setText(Geodetic3D.coordinateToPrintableText(geodetic.getLatitude()));

                textview = (TextView) getView().findViewById(R.id.textview_fragment_plane_details_long);
                textview.setText(Geodetic3D.coordinateToPrintableText(geodetic.getLongitude()));

                textview = (TextView) getView().findViewById(R.id.textview_fragment_plane_details_height);
                textview.setText(Geodetic3D.heightToPrintableText(geodetic.getAltitude()));
            }
        });
    }

    public void onPlaneVirtualOrientationChanged() {
        // Nothing to do
    }

    public void onPlaneDipChanged() {
        // Nothing to do
    }

    public void onPlaneSizeChanged() {
        // Nothing to do
    }

    public void onPlaneThicknessChanged() {
        // Nothing to do
    }
    //endregion
}