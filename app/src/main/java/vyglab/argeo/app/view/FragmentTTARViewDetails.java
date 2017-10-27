package vyglab.argeo.app.view;

import android.graphics.Bitmap;
import android.graphics.Matrix;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.widget.SwitchCompat;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.TextView;

import vyglab.argeo.jni.ArgeoFragment;
import vyglab.argeo.jni.Camera;
import vyglab.argeo.jni.RenderLoopRunnable;
import vyglab.argeo.jni.SnapshotListener;
import vyglab.argeo.app.MainActivityState;
import vyglab.argeo.R;
import vyglab.argeo.app.controller.PositionPoiChanged;
import vyglab.argeo.app.model.Geodetic3D;
import vyglab.argeo.app.model.TTARView;

/**
 * Created by root on 28/02/17.
 */

public class FragmentTTARViewDetails extends Fragment implements SnapshotListener, PositionPoiChanged {
    private static final String ARG_SECTION_NUMBER = "section_number";

    private MainActivityState m_main_activity_state;
    private ArgeoFragment m_argeo_fragment;

    private String m_name;
    private String m_description;
    private double m_latitude;
    private double m_longitude;
    private double m_height;
    private Bitmap m_reality_snapshot;
    private Bitmap m_ttar_snapshot;
    private Camera m_camera;
    private Bitmap m_updated_snapshot;
    private TTARView m_current_ttarview_item;
    private boolean ttarview_creation;
    private int m_ttarview_w;
    private int m_ttarview_h;

    public interface FragmentTTARViewDetailsdListener {
        void onTTARViewDetailSketchSwitchToggled(boolean toggle);
    }

    public static FragmentTTARViewDetails newInstance(int sectionNumber){
        FragmentTTARViewDetails fragment = new FragmentTTARViewDetails();
        Bundle args = new Bundle();
        args.putInt(ARG_SECTION_NUMBER, sectionNumber);

        return fragment;
    }

    public FragmentTTARViewDetails(){
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View rootView = inflater.inflate(R.layout.fragment_ttarview_details, container, false);

        /*SwitchCompat switch_sketch = (SwitchCompat) getView().findViewById(R.id.switch_sketch);
        switch_sketch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
            }
        });*/

        return rootView;
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstanceState) {
        cleanView();
    }

    public void setMainActivityState(MainActivityState state){
        m_main_activity_state = state;
    }

    public void setArgeoFragment(ArgeoFragment fragment){
        m_argeo_fragment = fragment;
    }

    public void setTTARViewWandH(int w, int h) {
        m_ttarview_w = w;
        m_ttarview_h = h;
    }

    public TTARView getCurrentTTARView() {
        return m_current_ttarview_item;
    }

    public void cleanView(){
        m_name= "";
        m_description = "";
        m_latitude = 0.0;
        m_longitude = 0.0;
        m_height = 0.0;
        m_reality_snapshot = null;
        m_camera = null;
        m_updated_snapshot = null;
        m_current_ttarview_item = null;
        ttarview_creation = true;

        EditText edittext = (EditText) getView().findViewById(R.id.editText_ttarview_name);
        edittext.setText("");
        edittext.setEnabled(false);

        edittext = (EditText) getView().findViewById(R.id.editText_ttarview_description);
        edittext.setText("");
        edittext.setEnabled(false);

        TextView textview = (TextView) getView().findViewById(R.id.textView_ttarview_lat);
        textview.setText("---");

        textview = (TextView) getView().findViewById(R.id.textView_ttarview_long);
        textview.setText("---");

        textview = (TextView) getView().findViewById(R.id.textView_ttarview_height);
        textview.setText("---");

        ImageButton imagebutton = (ImageButton) getView().findViewById(R.id.imagebutton_open_initial_ttarview);
        imagebutton.setEnabled(false);

        Button button = (Button) getView().findViewById(R.id.button_ttarview_delete);
        button.setEnabled(false);

        ImageView imageview = (ImageView) getView().findViewById(R.id.imageview_ttarview_details_initial_image);
        imageview.setImageResource(R.drawable.ic_menu_camera);

        imageview = (ImageView) getView().findViewById(R.id.imageview_ttarview_details_updated_image);
        imageview.setImageResource(R.drawable.ic_menu_camera);

        SwitchCompat switch_sketch = (SwitchCompat) getView().findViewById(R.id.switch_sketch);
        switch_sketch.setChecked(false);
        switch_sketch.setEnabled(false);
    }

    public void loadTTARView(TTARView item) {
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
        imageview.setImageBitmap(item.getInitialaView());

        imageview = (ImageView) this.getView().findViewById(R.id.imageview_ttarview_details_updated_image);
        imageview.setImageBitmap(item.getUpdatedView());

        Button button = (Button) getView().findViewById(R.id.button_ttarview_delete);
        button.setEnabled(true);

        SwitchCompat switch_sketch = (SwitchCompat) getView().findViewById(R.id.switch_sketch);
        switch_sketch.setEnabled(true);

        m_current_ttarview_item  = item;
    }

    public TTARView getTTARViewFromView() {
        TTARView ttarview = new TTARView();
        ttarview.setId("");
        ttarview.setPosition(new Geodetic3D(
                m_camera.getPositionGeodetic().getLatitude(),
                m_camera.getPositionGeodetic().getLongitude(),
                m_camera.getPositionGeodetic().getAltitude()));

        EditText edittext = (EditText) getView().findViewById(R.id.editText_ttarview_name);
        ttarview.setName(edittext.getText().toString());

        edittext = (EditText) getView().findViewById(R.id.editText_ttarview_description);
        ttarview.setDescription(edittext.getText().toString());

        ttarview.setCamera(m_camera);
        ttarview.setInitialCameraFrame(m_reality_snapshot);
        ttarview.setInitialaView(m_updated_snapshot);
        ttarview.setUpdatedView(m_updated_snapshot);

        return ttarview;
    }

    public void setForCreation(int ttarview_snapshot_w, int ttarview_snapshot_h){
        m_ttarview_w = ttarview_snapshot_w;
        m_ttarview_h = ttarview_snapshot_h;

        EditText edittext = (EditText) getView().findViewById(R.id.editText_ttarview_name);
        edittext.setEnabled(true);

        edittext = (EditText) getView().findViewById(R.id.editText_ttarview_description);
        edittext.setEnabled(true);

        ImageButton imagebutton = (ImageButton) getView().findViewById(R.id.imagebutton_open_initial_ttarview);
        imagebutton.setEnabled(false);

        Button button = (Button) getView().findViewById(R.id.button_ttarview_delete);
        button.setEnabled(false);

        m_argeo_fragment.getViewer().getScene().addSnapshotListener(this);
        m_argeo_fragment.getViewer().executeOnRenderLoop(
                new RenderLoopRunnable() {

                    @Override
                    public void run() {
                        m_argeo_fragment.getViewer().getScene().takeTTARSnapshot(
                                m_reality_snapshot,
                                m_ttar_snapshot,
                                m_camera,
                                m_ttarview_w,
                                m_ttarview_h);
                    }

                });
    }

    public void prepareForPictureInPictureARView() {
        //ttarview_creation = false;
        //HandyPOI.getInstance().addListener(this);

        /*//m_argeo_fragment.getViewer().getScene().addSnapshotListener(this);
        m_argeo_fragment.getViewer().executeOnRenderLoop(
                new RenderLoopRunnable() {

                    @Override
                    public void run() {
                        m_argeo_fragment.getViewer().getScene().takeTTARSnapshot(m_current_ttarview_item.getInitialCameraFrame(), m_current_ttarview_item.getCamera());
                    }

                });*/
    }

    @Override
    public void onSnapshotAvailable(Bitmap snapshot) {
        // Nothing to do here
    }

    @Override
    public void onARBufferSnapshotAvailable(
            Bitmap reality_snapshot,
            Bitmap virtuality_snapshot,
            Bitmap mixed_reality_snapshot) {
        // Nothing to do here
    }

    @Override
    public void onTTARSnapshotAvailable(
            Bitmap reality_snapshot,
            Bitmap ttar_snapshot,
            Camera camera) {

        if (ttarview_creation) {
            m_argeo_fragment.getViewer().getScene().removeSnapshotListener(this);

            // Flip bitmaps vertically
            //Matrix matrix = new Matrix();
            //matrix.preScale(1.0f, -1.0f);
            //reality_snapshot = Bitmap.createBitmap(reality_snapshot, 0, 0, ttar_snapshot.getWidth(), ttar_snapshot.getHeight(), matrix, true);
            //ttar_snapshot = Bitmap.createBitmap(ttar_snapshot, 0, 0, ttar_snapshot.getWidth(), ttar_snapshot.getHeight(), matrix, true);

            if (m_camera == null) {
                m_camera = camera;
                final TextView textview_lat = (TextView) getView().findViewById(R.id.textView_ttarview_lat);
                textview_lat.post(new Runnable() {
                    @Override
                    public void run() {
                        textview_lat.setText(Geodetic3D.coordinateToPrintableText(m_camera.getPositionGeodetic().getLatitude()));
                    }
                });

                final TextView textview_long = (TextView) getView().findViewById(R.id.textView_ttarview_long);
                textview_long.post(new Runnable() {
                    @Override
                    public void run() {
                        textview_long.setText(Geodetic3D.coordinateToPrintableText(m_camera.getPositionGeodetic().getLongitude()));
                    }
                });

                final TextView textview_height = (TextView) getView().findViewById(R.id.textView_ttarview_height);
                textview_height.post(new Runnable() {
                    @Override
                    public void run() {
                        textview_height.setText(Geodetic3D.heightToPrintableText(m_camera.getPositionGeodetic().getAltitude()));
                    }
                });
            }

            if (m_reality_snapshot == null) {
                m_reality_snapshot = reality_snapshot;
            }

            if(m_updated_snapshot == null) {
                m_updated_snapshot = ttar_snapshot;
            }

            final ImageView imageview_reality = (ImageView) this.getView().findViewById(R.id.imageview_ttarview_details_initial_image);
            imageview_reality.post(new Runnable() {
                @Override
                public void run() {
                    imageview_reality.setImageBitmap(m_reality_snapshot);
                }
            });

            final ImageView imageview_updated = (ImageView) this.getView().findViewById(R.id.imageview_ttarview_details_updated_image);
            imageview_updated.post(new Runnable() {
                @Override
                public void run() {
                    imageview_updated.setImageBitmap(m_updated_snapshot);
                }
            });
        } else { // It is in Picture-in-Picture mode
            m_updated_snapshot = ttar_snapshot;
            final ImageView imageview_updated = (ImageView) this.getView().findViewById(R.id.imageview_ttarview_details_updated_image);
            imageview_updated.post(new Runnable() {
                @Override
                public void run() {
                    imageview_updated.setImageBitmap(m_updated_snapshot);
                }
            });
        }

    }

    @Override
    public void onPositionPoiChanged() {
        /*
        m_argeo_fragment.getViewer().executeOnRenderLoop(
                new RenderLoopRunnable() {

                    @Override
                    public void run() {
                        m_argeo_fragment.getViewer().getScene().takeTTARSnapshot(
                                m_current_ttarview_item.getInitialCameraFrame(), m_current_ttarview_item.getCamera(), 512, 512);
                    }

                });
                */
    }
}
