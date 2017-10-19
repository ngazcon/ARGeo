package vyglab.argeo.app.controller;

import android.app.Activity;
import android.graphics.Bitmap;
import android.view.View;
import android.widget.ImageView;
import android.widget.Toast;

import vyglab.argeo.jni.ArgeoFragment;
import vyglab.argeo.jni.Camera;
import vyglab.argeo.jni.RenderLoopRunnable;
import vyglab.argeo.jni.SnapshotListener;
import vyglab.argeo.R;

/**
 * Created by root on 18/02/17.
 */

public class ListenerScreenShot implements View.OnClickListener, SnapshotListener {

    private Camera m_camera;

    private Bitmap m_ttarSnapshot;
    private Bitmap m_realitySnapshot;

    private Activity m_activity;
    private ArgeoFragment m_argeoFragment;

    public ListenerScreenShot(Activity activity, ArgeoFragment fragment) {
        m_activity = activity;
        m_argeoFragment = fragment;
    }

    @Override
    public void onClick(View view) {
        m_argeoFragment.getViewer().getScene().addSnapshotListener(this);

        Toast.makeText(m_activity.getApplicationContext(), "Screenshot!", Toast.LENGTH_SHORT).show();

        m_argeoFragment.getViewer().executeOnRenderLoop(
                new RenderLoopRunnable() {

                    @Override
                    public void run() {
                        m_argeoFragment.getViewer().getScene().takeTTARSnapshot(
                                m_realitySnapshot,
                                m_ttarSnapshot,
                                m_camera,
                                512,
                                512);
                    }

                });
    }

    @Override
    public void onSnapshotAvailable(Bitmap snapshot) {

    }

    @Override
    public void onARBufferSnapshotAvailable(
            Bitmap reality_snapshot,
            Bitmap virtuality_snapshot,
            Bitmap mixed_reality_snapshot) {

    }

    @Override
    public void onTTARSnapshotAvailable(
            Bitmap reality_snapshot,
            Bitmap ttar_snapshot,
            Camera camera) {

        if (m_camera == null) {
            m_camera = camera;
        }

        if (m_realitySnapshot == null) {
            m_realitySnapshot = reality_snapshot;
        }

        /*
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
        */
        int w = ttar_snapshot.getWidth();
        int h = ttar_snapshot.getHeight();
        if (ttar_snapshot != null){
            final Bitmap render = ttar_snapshot;
            final ImageView image = (ImageView) m_activity.findViewById(R.id.imageview_ttarview_picture_in_picture);
            image.post(new Runnable() {
                @Override
                public void run() {
                    image.setImageBitmap(render);
                }
            });
        }
    }
}
