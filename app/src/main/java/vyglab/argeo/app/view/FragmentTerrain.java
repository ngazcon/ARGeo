package vyglab.argeo.app.view;

import android.app.Activity;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.widget.SwitchCompat;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CompoundButton;
import android.support.v7.widget.AppCompatSeekBar;
import android.widget.ImageButton;
import android.widget.SeekBar;
import android.widget.TextView;

import vyglab.argeo.R;
import vyglab.argeo.app.MainActivity;
import vyglab.argeo.jni.Camera;
import vyglab.argeo.app.model.Geodetic3D;

/**
 * Created by root on 02/11/16.
 */

public class FragmentTerrain extends Fragment {
    OnWireframeToggleChangedListener m_callback;
    private int hud_width_i;

    // Container Activity must implement this interface
    public interface OnWireframeToggleChangedListener {
        public void onHideTerrainToggled(boolean value);
        public void onWireframeToggled(boolean value);
        public void onIlluminationChanged(boolean value);
        public void onOpacityChanged(float value);
        public void onExaggerationChanged(double value);
        public double onCameraHeightChanged(double value);
        public void onPositioningSensivityChanged(double value);
        public void onOrientationSensivityChanged(double value);
        public void onKalmanToggled(boolean value);
        public void onHudOpacityChanged(int value);
        public void onHudWidthChanged(int value);
        public void onDefaultButtonClicked();
    }

    public FragmentTerrain() {
        // Nothing to do here
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        hud_width_i = 2;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {

        View rootView = inflater.inflate(R.layout.fragment_terrain, container, false);

        SwitchCompat switch_button = (SwitchCompat) rootView.findViewById(R.id.switch_hide_terrain);
        switch_button.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                m_callback.onHideTerrainToggled(b);
            }
        });

        switch_button = (SwitchCompat) rootView.findViewById(R.id.switch_wireframe);
        switch_button.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
               m_callback.onWireframeToggled(b);
            }
        });

        switch_button = (SwitchCompat) rootView.findViewById(R.id.switch_illumination);
        switch_button.setChecked(false);
        switch_button.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                m_callback.onIlluminationChanged(b);
            }
        });

        AppCompatSeekBar seekBar = (AppCompatSeekBar) rootView.findViewById(R.id.seekBar_opacity);
        seekBar.setOnSeekBarChangeListener(new AppCompatSeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                float value = i / 10.0f;
                TextView text = (TextView) getActivity().findViewById(R.id.textView_opacity);
                text.setText(String.valueOf(value));
                m_callback.onOpacityChanged(value);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        seekBar = (AppCompatSeekBar) rootView.findViewById(R.id.seekBar_exaggeration);
        seekBar.setOnSeekBarChangeListener(new AppCompatSeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                double value = i /10.0;
                TextView text = (TextView) getActivity().findViewById(R.id.textView_exaggeration);
                text.setText(String.valueOf(value));
                m_callback.onExaggerationChanged(value);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        seekBar = (AppCompatSeekBar) rootView.findViewById(R.id.seekBar_camera_height);
        seekBar.setOnSeekBarChangeListener(new AppCompatSeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                double value = i;
                double newHeight = m_callback.onCameraHeightChanged(value);
                TextView text = (TextView) getActivity().findViewById(R.id.textView_camera_height);
                text.setText(String.valueOf(newHeight).concat("m"));
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        seekBar = (AppCompatSeekBar) rootView.findViewById(R.id.seekBar_camera_positioning_sensitivity);
        seekBar.setOnSeekBarChangeListener(new AppCompatSeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                double value = (i + 1) * 10;
                m_callback.onPositioningSensivityChanged(value);
                TextView text = (TextView) getActivity().findViewById(R.id.textView_camera_positioning_sensitivity);
                text.setText(String.valueOf((int)value));
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        seekBar = (AppCompatSeekBar) rootView.findViewById(R.id.seekBar_camera_orientation_sensitivity);
        seekBar.setOnSeekBarChangeListener(new AppCompatSeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                double value = (i + 1) * 0.25;
                m_callback.onOrientationSensivityChanged(value);
                TextView text = (TextView) getActivity().findViewById(R.id.textView_camera_orientation_sensitivity);
                String number = String.format("%.2f", value);
                text.setText(number);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        seekBar = (AppCompatSeekBar) rootView.findViewById(R.id.seekBar_hud_opacity);
        seekBar.setOnSeekBarChangeListener(new AppCompatSeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                int value = i * 10;
                TextView text = (TextView) getActivity().findViewById(R.id.textView_hud_opacity);
                text.setText(String.valueOf(value).concat("%"));
                m_callback.onHudOpacityChanged(value);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        ImageButton image_button = (ImageButton) rootView.findViewById(R.id.imageButton_hud_width_plus);
        image_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if ( hud_width_i < 10 ){
                    hud_width_i++;
                    int new_value = 100 + hud_width_i * 50;
                    TextView text = (TextView) getActivity().findViewById(R.id.textView_hud_with);
                    text.setText(String.valueOf(new_value));
                    m_callback.onHudWidthChanged(new_value);
                }
            }
        });

        image_button = (ImageButton) rootView.findViewById(R.id.imageButton_hud_width_minus);
        image_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if ( hud_width_i > 0 ){
                    hud_width_i--;
                    int new_value = 100 + hud_width_i * 50;
                    TextView text = (TextView) getActivity().findViewById(R.id.textView_hud_with);
                    text.setText(String.valueOf(new_value));
                    m_callback.onHudWidthChanged(new_value);
                }
            }
        });

        switch_button = (SwitchCompat) rootView.findViewById(R.id.switch_kalman);
        switch_button.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                m_callback.onKalmanToggled(b);
            }
        });
        switch_button.setVisibility(View.GONE);

        Button button = (Button) rootView.findViewById(R.id.button_terrain_default);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                m_callback.onDefaultButtonClicked();
            }
        });

        return rootView;
    }

    @Override
    public void onStart() {
        super.onStart();

        //TODO volarloooooo
        hardcodedValues();
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
        try {
            m_callback = (OnWireframeToggleChangedListener) activity;
        } catch (ClassCastException e) {
            throw new ClassCastException(activity.toString() + " must implement OnWireframeToggleChangedListener callbacks");
        }
    }

    public void showCameraPositionParameters(Camera camera){
        TextView text = (TextView) getActivity().findViewById(R.id.textView_camera_lat);
        text.setText(Geodetic3D.coordinateToPrintableText(camera.getPositionGeodetic().getLatitude()));

        text = (TextView) getActivity().findViewById(R.id.textView_camera_long);
        text.setText(Geodetic3D.coordinateToPrintableText(camera.getPositionGeodetic().getLongitude()));

        text = (TextView) getActivity().findViewById(R.id.textView_camera_height);
        text.setText(Geodetic3D.heightToPrintableText(camera.getPositionGeodetic().getAltitude()));
    }

    public void showCameraOrientationParameters(Camera camera){
        TextView text = (TextView) getActivity().findViewById(R.id.textView_camera_yaw);
        text.setText(String.valueOf(camera.getYaw()*180.0/Math.PI).concat("°"));

        text = (TextView) getActivity().findViewById(R.id.textView_camera_pitch);
        text.setText(String.valueOf(camera.getPitch()*180.0/Math.PI).concat("°"));

        text = (TextView) getActivity().findViewById(R.id.textView_camera_roll);
        text.setText(String.valueOf(camera.getRoll()*180.0/Math.PI).concat("°"));
    }

    public void setHudWidth(int value){
        hud_width_i = (value - 100) / 50;
        TextView text = (TextView) getActivity().findViewById(R.id.textView_hud_with);
        text.setText(String.valueOf(value));
        m_callback.onHudWidthChanged(value);
    }

    public void setHudOpacity(int value){
        final AppCompatSeekBar seekBar = (AppCompatSeekBar) getActivity().findViewById(R.id.seekBar_hud_opacity);
        final int hud_opacity_value = value / 10;
        seekBar.post(new Runnable() {
            @Override
            public void run() {
                seekBar.setProgress(hud_opacity_value);
            }
        });
    }

    public void setExaggeration(double value){
        SeekBar seekBar = (SeekBar) getActivity().findViewById(R.id.seekBar_exaggeration);
        seekBar.setProgress((int) Math.round(value * 10.0f));
    }

    //TODO Volarlo cdo esten los callbacks en el terrain
    private void hardcodedValues(){
        final AppCompatSeekBar seekBar = (AppCompatSeekBar) getActivity().findViewById(R.id.seekBar_opacity);
        seekBar.post(new Runnable() {
            @Override
            public void run() {
                seekBar.setProgress(7);
            }
        });

        final AppCompatSeekBar seekBar2 = (AppCompatSeekBar) getActivity().findViewById(R.id.seekBar_exaggeration);
        seekBar2.post(new Runnable() {
            @Override
            public void run() {
                seekBar2.setProgress(10);
            }
        });
    }

    public void resetToDefault(boolean hide, boolean wireframe, boolean illumination, float opacity, double exaggeration, float cameraH, double cameraPos, double cameraOrient, boolean kalman, int hud_opacity, int hud_width){
        SwitchCompat switch_button= (SwitchCompat) getActivity().findViewById(R.id.switch_hide_terrain);
        if (switch_button.isChecked() != hide){
            switch_button.setChecked(hide);
        }

        switch_button = (SwitchCompat) getActivity().findViewById(R.id.switch_wireframe);
        if (switch_button.isChecked() != wireframe){
            switch_button.setChecked(wireframe);
        }

        switch_button = (SwitchCompat) getActivity().findViewById(R.id.switch_illumination);
        if (switch_button.isChecked() != illumination) {
            switch_button.setChecked(illumination);
        }

        final AppCompatSeekBar seekBar = (AppCompatSeekBar) getActivity().findViewById(R.id.seekBar_opacity);
        final int opacity_value = (int) (opacity * 10.0f);
        seekBar.post(new Runnable() {
            @Override
            public void run() {
                seekBar.setProgress(opacity_value);
            }
        });

        final AppCompatSeekBar seekBar2 = (AppCompatSeekBar) getActivity().findViewById(R.id.seekBar_exaggeration);
        final int exaggeration_value = (int) (exaggeration * 10.0);
        seekBar2.post(new Runnable() {
            @Override
            public void run() {
                seekBar2.setProgress(exaggeration_value);
            }
        });

        final AppCompatSeekBar seekBar3 = (AppCompatSeekBar) getActivity().findViewById(R.id.seekBar_camera_height);
        final int camera_height = (int) (cameraH * 2.0f + 40.0f);
        seekBar2.post(new Runnable() {
            @Override
            public void run() {
                seekBar3.setProgress(camera_height);
            }
        });

        final AppCompatSeekBar seekBar4 = (AppCompatSeekBar) getActivity().findViewById(R.id.seekBar_camera_positioning_sensitivity);
        final int camera_positioning = (int) (cameraPos / 10.0) - 1;
        seekBar2.post(new Runnable() {
            @Override
            public void run() {
                seekBar4.setProgress(camera_positioning);
            }
        });

        final AppCompatSeekBar seekBar5 = (AppCompatSeekBar) getActivity().findViewById(R.id.seekBar_camera_orientation_sensitivity);
        final int camera_orientation = (int) (cameraOrient / 0.25) - 1;
        seekBar2.post(new Runnable() {
            @Override
            public void run() {
                seekBar5.setProgress(camera_orientation);
            }
        });

        switch_button = (SwitchCompat) getActivity().findViewById(R.id.switch_kalman);
        if (switch_button.isChecked() != kalman) {
            switch_button.setChecked(kalman);
        }

        final AppCompatSeekBar seekBar6 = (AppCompatSeekBar) getActivity().findViewById(R.id.seekBar_hud_opacity);
        final int hud_opacity_value = hud_opacity / 10;
        seekBar2.post(new Runnable() {
            @Override
            public void run() {
                seekBar6.setProgress(hud_opacity_value);
            }
        });

        hud_width_i = (hud_width - 100) / 50;
        TextView text = (TextView) getActivity().findViewById(R.id.textView_hud_with);
        text.setText(String.valueOf(hud_width));
        m_callback.onHudWidthChanged(hud_width);
    }

}
