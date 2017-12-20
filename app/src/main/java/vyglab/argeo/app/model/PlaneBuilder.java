package vyglab.argeo.app.model;

import vyglab.argeo.jni.ArgeoFragment;
import vyglab.argeo.jni.EllipsoidTangentPlane;
import vyglab.argeo.jni.EllipsoidTransformations;
import vyglab.argeo.jni.Entity;
import vyglab.argeo.jni.Geocentric3D;
import vyglab.argeo.jni.PlaneGraphics;
import vyglab.argeo.jni.Quaternion;

/**
 * Created by root on 02/12/17.
 */

public class PlaneBuilder {
    private ArgeoFragment m_argeo_fragment;

    private String m_id;
    private String m_name;
    private String m_description;
    private vyglab.argeo.jni.Geodetic3D m_position_geodetic;
    private Geocentric3D m_position_geocentric;
    private int m_dip;
    private int m_virtual_orientation;
    private int m_strike;
    private int m_size;
    private int m_thickness;
    private boolean m_show_virtual_orientation_plane;

    public PlaneBuilder(ArgeoFragment argeo_fragment) {
        m_argeo_fragment = argeo_fragment;
        clearBuilder();
    }

    public PlaneBuilder setId(String id) {
        m_id = id;
        return this;
    }

    public PlaneBuilder setName(String name) {
        m_name = name;
        return this;
    }

    public PlaneBuilder setDescription(String description) {
        m_description = description;
        return this;
    }

    public PlaneBuilder setPosition(vyglab.argeo.jni.Geocentric3D position_geocentric) {
        m_position_geocentric = position_geocentric;
        m_position_geodetic = EllipsoidTransformations.geodetic3DFromGeocentric3D(position_geocentric);
        return this;
    }

    public PlaneBuilder setDip(int dip) {
        m_dip = dip;
        return this;
    }

    public PlaneBuilder setVirtualOrientation(int virtual_orientation) {
        m_virtual_orientation = virtual_orientation;
        return this;
    }

    public PlaneBuilder setStrike(int strike) {
        m_strike = strike;
        return this;
    }

    public PlaneBuilder setSize(int size) {
        m_size = size;
        return this;
    }

    public PlaneBuilder setThickness(int thickness) {
        m_thickness = thickness;
        return this;
    }

    public PlaneBuilder setShowVirtualOrientationPlane(boolean value) {
        m_show_virtual_orientation_plane = value;
        return this;
    }

    public Plane build() {
        Plane plane = new Plane(m_id, m_name, m_description);
        plane.setPosition(new Geodetic3D(m_position_geodetic.getLatitude(), m_position_geodetic.getLongitude(), m_position_geodetic.getAltitude()));
        plane.setDip(m_dip);
        plane.setVirtualOrientation(m_virtual_orientation);
        plane.setStrike(m_strike);
        plane.setSize(m_size);
        plane.setThickness(m_thickness);
        plane.setShowwVirtualOrientationPlane(m_show_virtual_orientation_plane);
        createPlaneGraphics(plane);

        return plane;
    }

    public void clearBuilder() {
        // Something to do???
    }

    //**********************************************************************************************
    protected void createPlaneGraphics(Plane plane) {
        // Set Planes graphic and their properties (tangent plane needed for the plane positioning)
        plane.setVirtualOrientationPlaneGraphic(new PlaneGraphics(new EllipsoidTangentPlane(EllipsoidTransformations.geodetic3DFromGeocentric3D(m_position_geocentric)), true)); // true == red
        plane.getVirtualOrientationPlaneGraphic().setWidth(m_size);
        plane.getVirtualOrientationPlaneGraphic().setHeight(m_thickness);

        plane.setDippingPlaneGraphic(new PlaneGraphics(new EllipsoidTangentPlane(EllipsoidTransformations.geodetic3DFromGeocentric3D(m_position_geocentric)), false));
        plane.getDippingPlaneGraphic().setWidth(m_size);
        plane.getDippingPlaneGraphic().setHeight(m_thickness);

        // Create Entities
        plane.setVirtualOrientationPlaneEntity(new Entity(plane.getId(), plane.getVirtualOrientationPlaneGraphic()));
        plane.setDippingPlaneEntity(new Entity(plane.getId().concat("dip"), plane.getDippingPlaneGraphic()));

        // Set plane orientations and add planes to the render engine
        setOrientations(plane);
        if (m_show_virtual_orientation_plane) {
            m_argeo_fragment.getViewer().getEntities().add(plane.getVirtualOrientationPlaneEntity());
        }
        m_argeo_fragment.getViewer().getEntities().add(plane.getDippingPlaneEntity());
    }

    private void setOrientations(Plane plane) {
        double value_virtual_orientation = m_virtual_orientation;
        double value_dip = m_dip;

        EllipsoidTangentPlane tangent = new EllipsoidTangentPlane(EllipsoidTransformations.geodetic3DFromGeocentric3D(m_position_geocentric));
        Quaternion q_s = Quaternion.fromAxisAngle(tangent.getNormal(), value_virtual_orientation / -180 * 3.14);
        Quaternion q_d = Quaternion.fromAxisAngle(tangent.getXAxis(), value_dip / 180.0 * 3.14);
        Quaternion q_d_final;

        // Quaternion Math
        double x1 = q_s.getX();
        double y1 = q_s.getY();
        double z1 = q_s.getZ();
        double w1 = q_s.getW();

        double x2 = q_d.getX();
        double y2 = q_d.getY();
        double z2 = q_d.getZ();
        double w2 = q_d.getW();

        double x = x1 * w2 + y1 * z2 - z1 * y2 + w1 * x2;
        double y = -x1 * z2 + y1 * w2 + z1 * x2 + w1 * y2;
        double z = x1 * y2 - y1 * x2 + z1 * w2 + w1 * z2;
        double w = -x1 * x2 - y1 * y2 - z1 * z2 + w1 * w2;

        double n = Math.sqrt(x * x + y * y + z * z + w * w);
        x /= n;
        y /= n;
        z /= n;
        w /= n;

        q_d_final = new Quaternion(w, x, y, z);

        plane.getVirtualOrientationPlaneEntity().setOrientation(q_s);
        plane.getDippingPlaneEntity().setOrientation(q_d_final);
    }

}
