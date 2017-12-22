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
    private int m_virtual_orientation; // 0..360 degress
    private int m_strike; // 0..360 degrees

    private Entity m_virtual_orientation_plane_entity;
    private Entity m_dipping_plane_entity;
    private PlaneGraphics m_virtual_orientation_plane_graphic;
    private PlaneGraphics m_dipping_plane_graphic;
    private int m_size;
    private int m_thickness;
    private boolean m_show_virtual_orientation_plane;

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

    public void setVirtualOrientation(int vorientation) {
        m_virtual_orientation = vorientation;
    }
    public int getVirtualOrientation() {
        return m_virtual_orientation;
    }

    public void setStrike(int strike) {
        m_strike = strike;
    }
    public int getStrike() {
        return m_strike;
    }

    public void setVirtualOrientationPlaneEntity(Entity entity ){ m_virtual_orientation_plane_entity = entity; }
    public Entity getVirtualOrientationPlaneEntity(){ return m_virtual_orientation_plane_entity; }

    public void setDippingPlaneEntity(Entity entity ){ m_dipping_plane_entity = entity; }
    public Entity getDippingPlaneEntity(){ return m_dipping_plane_entity; }

    public void setVirtualOrientationPlaneGraphic(PlaneGraphics plane ){ m_virtual_orientation_plane_graphic = plane; }
    public PlaneGraphics getVirtualOrientationPlaneGraphic(){ return m_virtual_orientation_plane_graphic; }

    public void setDippingPlaneGraphic(PlaneGraphics plane ){ m_dipping_plane_graphic = plane; }
    public PlaneGraphics getDippingPlaneGraphic(){ return m_dipping_plane_graphic; }

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

    public void setShowwVirtualOrientationPlane(boolean value){
        m_show_virtual_orientation_plane = value;
    }
    public boolean getShowVirtualOrientationPlane(){ return m_show_virtual_orientation_plane; }

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

    public void copyTo(Plane plane, boolean keep_id) {
        if (!keep_id){
            plane.setId(this.getId());
        }

        plane.setName(m_name);
        plane.setDescription(m_description);

        plane.setPosition(this.m_position);
        plane.setDip(this.m_dip);
        plane.setVirtualOrientation(this.m_virtual_orientation);
        plane.setStrike(this.m_strike);

        plane.setVirtualOrientationPlaneEntity(m_virtual_orientation_plane_entity);
        plane.setDippingPlaneEntity(m_dipping_plane_entity);
        plane.setVirtualOrientationPlaneGraphic(m_virtual_orientation_plane_graphic);
        plane.setDippingPlaneGraphic(m_dipping_plane_graphic);

        plane.setSize(m_size);
        plane.setThickness(m_thickness);
        plane.setShowwVirtualOrientationPlane(m_show_virtual_orientation_plane);

        plane.setVisible(m_visible);
        plane.setClamped(m_clamped);
    }
}