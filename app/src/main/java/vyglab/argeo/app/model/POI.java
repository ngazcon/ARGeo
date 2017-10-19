package vyglab.argeo.app.model;

import vyglab.argeo.jni.Entity;

/**
 * Created by ngazcon on 04/07/2016.
 */
public class POI extends Object {
    private Geodetic3D m_position;
    private Orientation m_orientation;
    private Entity m_graphic;
    private String m_description;
    private boolean m_visible;
    private boolean m_clamped;

    public POI(){
        super();
    }
    public POI( String id ){ super(id); }
    public POI( String id, String description){
        super(id);
        m_description = description;
    }

    public void setPosition( Geodetic3D position) { m_position = position; }
    public Geodetic3D getPosition() {  return m_position; }

    public void setOrientation( Orientation orientation) { m_orientation = orientation; }
    public Orientation getOrientation(){ return m_orientation; }

    public void setGraphic( Entity entity ){ m_graphic = entity; }
    public Entity getGraphic(){ return m_graphic; }

    public void setDescription(String description){
        m_description = description;
    }
    public String getDescription() { return m_description; }

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
}
