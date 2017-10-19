package vyglab.argeo.app.controller.ListenersTTARView;

import android.view.View;

import vyglab.argeo.app.MainActivity;
import vyglab.argeo.app.MainActivityState;
import vyglab.argeo.app.controller.FabOnClickListener;
import vyglab.argeo.app.view.FragmentTTARView;

/**
 * Created by root on 16/02/17.
 */

public class ListenerNewTTARView implements View.OnClickListener {
    private MainActivity m_activity;
    private MainActivityState m_main_activity_state;
    private FragmentTTARView m_fragment_ttarview;
    private FabOnClickListener m_fab_listener;

    public ListenerNewTTARView(MainActivity activity, MainActivityState activity_state, FragmentTTARView fragment_ttarview, FabOnClickListener fab_listener){
        m_activity = activity;
        m_main_activity_state = activity_state;
        m_fragment_ttarview = fragment_ttarview;
        m_fab_listener = fab_listener;
    }

    @Override
    public void onClick(View v){
        // Check if right menu is open
        m_fab_listener.requestOpen();

        // Prepare fragment
        m_fragment_ttarview.setForCreation();

        // Go to next State
        m_main_activity_state.getSecondaryFabContext().goNextState(this);
        m_main_activity_state.getSecondaryFabContext().getState().handle();
    }
}

/*public class ListenerNewTTARView implements View.OnClickListener, SnapshotListener {
    private Activity m_activity;
    private MainActivityState m_main_activity_state;
    private FabOnClickListener m_fab_listener;

    private Camera m_camera;
    private Bitmap m_realitySnapshot;
    private ArgeoFragment m_argeoFragment;

    public ListenerNewTTARView(
            Activity activity,
            ArgeoFragment fragment,
            MainActivityState activity_state,
            FabOnClickListener fab_listener){
        m_argeoFragment = fragment;
        m_activity = activity;
        m_main_activity_state = activity_state;
        m_fab_listener = fab_listener;
    }

    @Override
    public void onClick(View v){
        // Check if right menu is open
        //m_fab_listener.requestOpen();

        m_argeoFragment.getViewer().getScene().addSnapshotListener(this);

        Toast.makeText(m_activity.getApplicationContext(), "Screenshot!", Toast.LENGTH_SHORT).show();

        m_argeoFragment.getViewer().executeOnRenderLoop(
                new RenderLoopRunnable() {

                    @Override
                    public void run() {
                        m_argeoFragment.getViewer().getScene().takeTTARSnapshot(m_realitySnapshot, m_camera);
                    }

                });

        // Set the touch mode
        //m_main_activity_state.setTouchMode(MainActivityState.TouchMode.PLANE_CREATION);
        //m_main_activity_state.getSecondaryFabContext().goNextState(this);
        //m_main_activity_state.getSecondaryFabContext().getState().handle();
    }

    @Override
    public void onSnapshotAvailable(Bitmap snapshot) { }

    @Override
    public void onARBufferSnapshotAvailable(
            Bitmap reality_snapshot,
            Bitmap virtuality_snapshot,
            Bitmap mixed_reality_snapshot) { }

    @Override
    public void onTTARSnapshotAvailable(
            Bitmap reality_snapshot,
            Bitmap ttar_snapshot,
            Camera camera) {

        m_argeoFragment.getViewer().getScene().removeSnapshotListener(this);

        if (m_camera == null) {
            m_camera = camera;
        }

        if (m_realitySnapshot == null) {
            m_realitySnapshot = reality_snapshot;
        }

        String root = Environment.getExternalStorageDirectory().toString();
        //TODO ver donde definimos que se guarda esto
        File myDir = new File(root + "/screenshots");
        myDir.mkdirs();

        Calendar c = Calendar.getInstance();
        SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        String formattedDate = df.format(c.getTime());

        String fname = formattedDate + ".png";
        File file = new File(myDir, fname);

        try {
            FileOutputStream out = new FileOutputStream(file);
            ttar_snapshot.compress(Bitmap.CompressFormat.PNG, 100, out);
            out.close();
        } catch (Exception e) {
            e.printStackTrace();
        }

        fname = formattedDate + "_real" + ".png";
        file = new File(myDir, fname);

        try {
            FileOutputStream out = new FileOutputStream(file);
            reality_snapshot.compress(Bitmap.CompressFormat.PNG, 100, out);
            out.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}*/
