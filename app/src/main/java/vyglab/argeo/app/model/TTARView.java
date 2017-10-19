package vyglab.argeo.app.model;

import android.graphics.Bitmap;

import vyglab.argeo.jni.Camera;
import vyglab.argeo.jni.Entity;

import java.io.ByteArrayOutputStream;
import java.lang.*;

/**
 * Created by ngazcon on 21/02/2017.
 */

public class TTARView extends Object {
    private Geodetic3D m_position;
    private String m_name;
    private String m_description;

    private Camera m_camera;
    private Entity m_graphic;
    private Bitmap m_initial_view;
    private Bitmap m_initial_camera_frame;
    private Bitmap m_ttar_frame;

    private Bitmap m_updated_view;
    private boolean m_visible;
    private boolean m_clamped;

    public TTARView(){
        super();
    }
    public TTARView( String id ){ super(id); }

    public TTARView( String id, String name, String description){
        super(id);
        m_name = name;
        m_description = description;
    }

    public void setPosition( Geodetic3D position) { m_position = position; }
    public Geodetic3D getPosition() {  return m_position; }

    public void setName(String name){
        m_name = name;
    }
    public String getName() { return m_name; }

    public void setDescription(String description){
        m_description = description;
    }
    public String getDescription() { return m_description; }

    public void setCamera(Camera camera){
        m_camera = camera;
    }
    public Camera getCamera() { return m_camera; }

    public void setGraphic( Entity entity ){ m_graphic = entity; }
    public Entity getGraphic(){ return m_graphic; }

    public void setInitialaView(Bitmap initial_view){
        m_initial_view = initial_view;
    }
    public Bitmap getInitialaView() { return m_initial_view; }

    public void setInitialCameraFrame(Bitmap initial_camera_frame){
        m_initial_camera_frame = initial_camera_frame;
    }

    public Bitmap getInitialCameraFrame() { return m_initial_camera_frame; }

    public void setUpdatedView(Bitmap updated_view){
        m_updated_view = updated_view;
    }
    public Bitmap getUpdatedView() { return m_updated_view; }

    public void setVisible(boolean visible){
        m_visible = visible;
    }
    public boolean getVisible(){ return m_visible; }
    public void toogleVisible(){
        m_visible = !m_visible;
    }

    public void setClamped(boolean clamped){
        m_clamped = clamped;
    }
    public boolean getClamped(){ return m_clamped; }
    public void toogleClamped(){
        m_clamped = !m_clamped;
    }

    public void takeTTARView(Camera camera, Bitmap initial_view, Bitmap m_initial_camera_frame){
        //TODO aca va la llamada al motor para que tome
    }

    public void updateTTARView(){
        //TODO aca va la llamada al motor para que tome
    }

    // Method for Bitmap storage
    public byte[] getStoreableBitmap(Bitmap image) {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        image.compress(Bitmap.CompressFormat.PNG, 100, baos);
        byte[] output = baos.toByteArray();

        return output;
    }
}
