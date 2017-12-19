package vyglab.argeo.app.utils;

import vyglab.argeo.jni.ArgeoFragment;
import vyglab.argeo.jni.EllipsoidTangentPlane;
import vyglab.argeo.jni.EllipsoidTransformations;
import vyglab.argeo.jni.Entity;
import vyglab.argeo.jni.Geocentric3D;
import vyglab.argeo.jni.PlaneGraphics;
import vyglab.argeo.jni.Quaternion;
import vyglab.argeo.app.model.Plane;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by ngazcon on 09/03/2017.
 */

public class HandyPlane {
    private static HandyPlane m_instance = null;
    private Plane m_plane = null;
    private ArgeoFragment m_ArgeoFragment;
    private List<PlaneChanged> m_plane_listeners = new ArrayList<PlaneChanged>();

    private Geocentric3D m_geocentric;

    private int m_virtual_orientation;
    private int m_dip;
    private int m_strike;

    private final int m_dip_start = 0;
    private boolean m_show_virtual_orientation_plane = true;
    private double m_size;
    private double m_thickness;

    public interface PlaneChanged {
        void onPlanePositionChanged();

        void onPlaneVirtualOrientationChanged();

        void onPlaneDipChanged();

        void onPlaneSizeChanged();

        void onPlaneThicknessChanged();
    }

    protected HandyPlane() {
        clear(true);
    }

    public static HandyPlane getInstance() {
        if (m_instance == null) {
            m_instance = new HandyPlane();
        }

        return m_instance;
    }

    public void setup(ArgeoFragment fragment) {
        m_ArgeoFragment = fragment;
    }

    protected void updatePlaneGraphics() {
        // Reset the local plane graphics
        clearGraphics();

        // Set Planes graphic and their properties (tangent plane needed for the plane positioning)
        m_plane.setVirtualOrientationPlaneGraphic(new PlaneGraphics(new EllipsoidTangentPlane(EllipsoidTransformations.geodetic3DFromGeocentric3D(m_geocentric)), true)); // true == red
        m_plane.getVirtualOrientationPlaneGraphic().setWidth(m_size);
        m_plane.getVirtualOrientationPlaneGraphic().setHeight(m_thickness);

        m_plane.setDippingPlaneGraphic(new PlaneGraphics(new EllipsoidTangentPlane(EllipsoidTransformations.geodetic3DFromGeocentric3D(m_geocentric)), false));
        m_plane.getDippingPlaneGraphic().setWidth(m_size);
        m_plane.getDippingPlaneGraphic().setHeight(m_thickness);

        // Create Entities
        m_plane.setVirtualOrientationPlaneEntity(new Entity(m_plane.getId(), m_plane.getVirtualOrientationPlaneGraphic()));
        m_plane.setDippingPlaneEntity(new Entity(m_plane.getId().concat("dip"), m_plane.getDippingPlaneGraphic()));

        // Set plane orientations and add planes to the render engine
        setOrientations(m_plane);
        if (m_show_virtual_orientation_plane) {
            m_ArgeoFragment.getViewer().getEntities().add(m_plane.getVirtualOrientationPlaneEntity());
        }
        m_ArgeoFragment.getViewer().getEntities().add(m_plane.getDippingPlaneEntity());
    }

    /*public void updatePlaneLocation(Geocentric3D location) {
        // Reset the local plane
        clear(false);

        // Save location
        m_geocentric = location;

        // Create a local plane
        m_plane = new Plane("local-plane", "", "");
        //m_plane.setVirtualOrientationPlaneEntity(new Entity(m_plane.getId(), new PlaneGraphics(tangent, true)));
        //m_plane.setDippingPlaneEntity(new Entity(m_plane.getId().concat("dip"), new PlaneGraphics(new EllipsoidTangentPlane(EllipsoidTransformations.geodetic3DFromGeocentric3D(location)), false)));
        //m_plane.getDippingPlaneEntity().setOrientation(Quaternion.fromAxisAngle(tangent.getXAxis(), m_dip / 180.0 * 3.14));

        // Set Planes graphic and their properties (tangent plane needed for the plane positioning)
        m_plane.setVirtualOrientationPlaneGraphic(new PlaneGraphics(new EllipsoidTangentPlane(EllipsoidTransformations.geodetic3DFromGeocentric3D(location)), true)); // true == red
        m_plane.getVirtualOrientationPlaneGraphic().setWidth(m_size);
        m_plane.getVirtualOrientationPlaneGraphic().setHeight(m_thickness);

        m_plane.setDippingPlaneGraphic(new PlaneGraphics(new EllipsoidTangentPlane(EllipsoidTransformations.geodetic3DFromGeocentric3D(location)), false));
        m_plane.getDippingPlaneGraphic().setWidth(m_size);
        m_plane.getDippingPlaneGraphic().setHeight(m_thickness);

        // Create Entities
        m_plane.setVirtualOrientationPlaneEntity(new Entity(m_plane.getId(), m_plane.getVirtualOrientationPlaneGraphic()));
        m_plane.setDippingPlaneEntity(new Entity(m_plane.getId().concat("dip"), m_plane.getDippingPlaneGraphic()));
        //m_plane.getDippingPlaneEntity().setOrientation(Quaternion.fromAxisAngle(tangent.getXAxis(), m_dip / 180.0 * 3.14));
        setOrientations(m_plane);
        if (m_show_virtual_orientation_plane) {
            m_ArgeoFragment.getViewer().getEntities().add(m_plane.getVirtualOrientationPlaneEntity());
        }
        m_ArgeoFragment.getViewer().getEntities().add(m_plane.getDippingPlaneEntity());

        // Finally notify that local plane position has changed
        notifyPlanePositionChanged();
    }*/
    public void updatePlaneLocation(Geocentric3D location) {
        // Save the new location
        m_geocentric = location;

        // Check if a plane object already exists
        if (m_plane == null) {
            m_plane = new Plane("local-plane", "", "");
        }

        // Update plane graphics
        updatePlaneGraphics();

        // Finally notify that local plane position has changed
        notifyPlanePositionChanged();
    }

    private void setOrientations(Plane plane) {
        double value_virtual_orientation = m_virtual_orientation;
        double value_dip = m_dip;

        EllipsoidTangentPlane tangent = new EllipsoidTangentPlane(EllipsoidTransformations.geodetic3DFromGeocentric3D(m_geocentric));
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

    public void updatePlaneVirtualOrientation(int virtual_orientation) {
        if (m_plane != null) {
            m_virtual_orientation = virtual_orientation;

            setOrientations(m_plane);

            notifyPlaneVirtualOrientationChanged();
        }
    }

    public void updatePlaneDip(int dip) {
        if (m_plane != null) {
            m_dip = dip + m_dip_start;

            // It will unnecessarily update the virtual orientation orientation, nayway it will overwrites the same value
            setOrientations(m_plane);

            notifyPlaneDipChanged();
        }
    }

    public void updatePlaneSize(int size) {
        m_size = size;

        if (m_plane != null) {
            updatePlaneGraphics();
            notifyPlaneSizeChanged();
        }
    }

    public void updatePlaneThickness(int thickness) {
        m_thickness = thickness;

        if (m_plane != null) {
            updatePlaneGraphics();
            notifyPlaneThicknessChanged();
        }
    }

    public void updateShowVirtualOrientationPlane(boolean value) {
        // If it was showing the Virtual Orientation Plane, remove it from the render engine
        if (m_show_virtual_orientation_plane) {
            m_ArgeoFragment.getViewer().getEntities().remove(m_plane.getVirtualOrientationPlaneEntity());
        }

        // Update the value and the plane graphics
        m_show_virtual_orientation_plane = value;
        updatePlaneGraphics();
    }

    public void clear(boolean angles) {
        if (m_plane != null) {
            if (m_show_virtual_orientation_plane) {
                m_ArgeoFragment.getViewer().getEntities().remove(m_plane.getVirtualOrientationPlaneEntity());
            }
            m_ArgeoFragment.getViewer().getEntities().remove(m_plane.getDippingPlaneEntity());
            m_plane.setVirtualOrientationPlaneEntity(null);
            m_plane.setDippingPlaneEntity(null);
            m_plane.setVirtualOrientationPlaneGraphic(null);
            m_plane.setDippingPlaneGraphic(null);
            m_plane = null;
            m_geocentric = null;
        }
        if (angles) {
            m_virtual_orientation = 0;
            m_dip = m_dip_start;
            m_strike = 0;
            m_size = 30.0;
            m_thickness = 1.0;
        }
    }

    public void clearGraphics() {
        // Check if there is a plane
        if (m_plane != null) {
            // Check if the plane has plane graphics
            if ((m_plane.getVirtualOrientationPlaneGraphic() != null) && (m_plane.getDippingPlaneGraphic() != null)) {
                if (m_show_virtual_orientation_plane) {
                    m_ArgeoFragment.getViewer().getEntities().remove(m_plane.getVirtualOrientationPlaneEntity());
                }
                m_ArgeoFragment.getViewer().getEntities().remove(m_plane.getDippingPlaneEntity());
                m_plane.setVirtualOrientationPlaneEntity(null);
                m_plane.setDippingPlaneEntity(null);
                m_plane.setVirtualOrientationPlaneGraphic(null);
                m_plane.setDippingPlaneGraphic(null);
            }
        }
    }

    public Plane getPlane() {
        return m_plane;
    }

    // Methods for update location listeners
    public void addListener(PlaneChanged listener) {
        m_plane_listeners.add(listener);
    }

    public void removeListener(PlaneChanged listener) {
        m_plane_listeners.remove(listener);
    }

    private void notifyPlanePositionChanged() {
        for (PlaneChanged listener : m_plane_listeners) {
            listener.onPlanePositionChanged();
        }
    }

    private void notifyPlaneVirtualOrientationChanged() {
        for (PlaneChanged listener : m_plane_listeners) {
            listener.onPlaneVirtualOrientationChanged();
        }
    }

    private void notifyPlaneDipChanged() {
        for (PlaneChanged listener : m_plane_listeners) {
            listener.onPlaneDipChanged();
        }
    }

    private void notifyPlaneSizeChanged() {
        for (PlaneChanged listener : m_plane_listeners) {
            listener.onPlaneSizeChanged();
        }
    }

    private void notifyPlaneThicknessChanged() {
        for (PlaneChanged listener : m_plane_listeners) {
            listener.onPlaneSizeChanged();
        }
    }

    //TODO volarlo y hacerlo bien
    public Plane clonePlane() {
        Geocentric3D location = m_geocentric;
        clear(false);
        m_geocentric = location;
        Plane plane = new Plane("local-plane", "", "");
        EllipsoidTangentPlane tangent = new EllipsoidTangentPlane(EllipsoidTransformations.geodetic3DFromGeocentric3D(location));
        //m_plane.setVirtualOrientationPlaneEntity(new Entity(m_plane.getId(), new PlaneGraphics(tangent, true)));
        //m_plane.setDippingPlaneEntity(new Entity(m_plane.getId().concat("dip"), new PlaneGraphics(new EllipsoidTangentPlane(EllipsoidTransformations.geodetic3DFromGeocentric3D(location)), false)));
        //m_plane.getDippingPlaneEntity().setOrientation(Quaternion.fromAxisAngle(tangent.getXAxis(), m_dip / 180.0 * 3.14));
        plane.setVirtualOrientationPlaneGraphic(new PlaneGraphics(tangent, true));
        plane.setDippingPlaneGraphic(new PlaneGraphics(new EllipsoidTangentPlane(EllipsoidTransformations.geodetic3DFromGeocentric3D(location)), false));
        plane.getVirtualOrientationPlaneGraphic().setWidth(m_size);
        plane.getDippingPlaneGraphic().setWidth(m_size);
        plane.setVirtualOrientationPlaneEntity(new Entity(plane.getId(), plane.getVirtualOrientationPlaneGraphic()));
        plane.setDippingPlaneEntity(new Entity(plane.getId().concat("dip"), plane.getDippingPlaneGraphic()));
        //m_plane.getDippingPlaneEntity().setOrientation(Quaternion.fromAxisAngle(tangent.getXAxis(), m_dip / 180.0 * 3.14));
        setOrientations(plane);
        if (m_show_virtual_orientation_plane) {
            m_ArgeoFragment.getViewer().getEntities().add(plane.getVirtualOrientationPlaneEntity());
        }
        m_ArgeoFragment.getViewer().getEntities().add(plane.getDippingPlaneEntity());
        notifyPlanePositionChanged();

        return plane;
    }
}