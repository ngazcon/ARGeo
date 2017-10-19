package vyglab.argeo.app.model;

import android.graphics.BitmapFactory;

import vyglab.argeo.jni.Camera;
import vyglab.argeo.jni.EllipsoidTransformations;
import vyglab.argeo.jni.Geocentric3D;
import vyglab.argeo.jni.Scene;

/**
 * Created by ngazcon on 05/07/2017.
 */

public class TTARViewBuilder {
    private TTARView m_ttarview;

    private String m_id;
    private String m_name;
    private String m_desc;
    private double m_long;
    private double m_lat;
    private double m_height;
    private double m_yaw;
    private double m_pitch;
    private double m_roll;
    private byte[] m_frame;
    private byte[] m_view;

    // Only for the camera creation
    private Scene m_scene;

    public TTARViewBuilder() {
        m_ttarview = null;
    }

    public void configureTTARViewBuilder(String id, String name, String desc,
                                         double lon, double lat, double height,
                                         double yaw, double pitch, double roll,
                                         byte[] frame, byte[] view,
                                         Scene scene) {
        m_id = id;
        m_name = name;
        m_desc = desc;
        m_long = lon;
        m_lat = lat;
        m_height = height;
        m_yaw = yaw;
        m_pitch = pitch;
        m_roll = roll;
        m_frame = frame;
        m_view = view;
        m_scene =scene;
    }

    public void createTTARView() {
        m_ttarview = new TTARView(m_id, m_name, m_desc);
        m_ttarview.setPosition( new Geodetic3D(m_lat, m_long, m_height) );

        //todo ver por que revienta con el new camera
        Camera camera = m_scene.getCamera();//new Camera(m_scene);
        camera.setView(EllipsoidTransformations.geocentric3DFromGeodetic3D(new vyglab.argeo.jni.Geodetic3D(m_long, m_lat, m_height)), m_yaw, m_pitch, m_roll);
        m_ttarview.setCamera(camera);

        BitmapFactory bf = new BitmapFactory();
        m_ttarview.setInitialCameraFrame(bf.decodeByteArray(m_frame, 0, m_frame.length));
        m_ttarview.setInitialaView(bf.decodeByteArray(m_view, 0, m_view.length));
        m_ttarview.setUpdatedView(bf.decodeByteArray(m_view, 0, m_view.length));
    }

    public TTARView getTTARView() {
        return m_ttarview;
    }
}
