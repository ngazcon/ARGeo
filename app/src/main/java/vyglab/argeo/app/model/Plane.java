package vyglab.argeo.app.model;

import vyglab.argeo.jni.Entity;
import vyglab.argeo.jni.PlaneGraphics;

/**
 * Created by ngazcon on 09/03/2017.
 */

public class Plane extends Object {
    private String m_name;
    private String m_description;

    private Geodetic3D m_position;
    private int m_dip; // -90..90 degrees
    private int m_vorientation; // 0..360 degress
    private int m_strike; // 0..360 degrees

    private Entity m_strike_plane_graphic;
    private Entity m_dipping_plane_graphic;
    private PlaneGraphics m_strike_plane;
    private PlaneGraphics m_dipping_plane;
    private int m_size;
    private int m_thickness;

    private boolean m_visible;
    private boolean m_clamped;

    public Plane(){
        super();
    }
    public Plane( String id ){ super(id); }
    public Plane( String id, String name, String description){
        super(id);
        m_name = name;
        m_description = description;
    }

    public void setName(String name){
        m_name = name;
    }
    public String getName() { return m_name; }

    public void setDescription(String description){
        m_description = description;
    }
    public String getDescription() { return m_description; }

    public void setPosition( Geodetic3D position) { m_position = position; }
    public Geodetic3D getPosition() {  return m_position; }

    public void setDip(int dip) {
        m_dip = dip;
    }
    public int getDip() {
        return m_dip;
    }

    public void setVOrientation(int vorientation) {
        m_vorientation = vorientation;
    }
    public int getVOrientation() {
        return m_vorientation;
    }

    public void setStrike(int strike) {
        m_strike = strike;
    }
    public int getStrike() {
        return m_strike;
    }

    public void setStrikePlaneGraphic( Entity entity ){ m_strike_plane_graphic = entity; }
    public Entity getStrikePlaneGraphic(){ return m_strike_plane_graphic; }

    public void setDippingPlaneGraphic( Entity entity ){ m_dipping_plane_graphic = entity; }
    public Entity getDippingPlaneGraphic(){ return m_dipping_plane_graphic; }

    public void setStrikePlane( PlaneGraphics plane ){ m_strike_plane = plane; }
    public PlaneGraphics getStrikePlane(){ return m_strike_plane; }

    public void setDippingPlane( PlaneGraphics plane ){ m_dipping_plane = plane; }
    public PlaneGraphics getDippingPlane(){ return m_dipping_plane; }

    public void setSize(int size){
        m_size = size;
    }
    public int getSize() {
        return m_size;
    }

    public void setThickness(int thickness) {
        m_thickness = thickness;
    }
    public int getThickness() {
        return m_thickness;
    }

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