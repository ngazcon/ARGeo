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
    private Plane m_plane;
    private ArgeoFragment m_ArgeoFragment;
    private List<PlaneChanged> m_plane_listeners = new ArrayList<PlaneChanged>();

    private Geocentric3D m_geocentric;
    private int m_strike;
    private int m_dip;
    private final int m_dip_start = 0;
    private boolean m_show_strike_plane;
    private double m_size;

    public interface PlaneChanged {
        void onPlanePositionChanged();

        void onPlaneStrikeChanged();

        void onPlaneDipChanged();

        void onPlaneSizeChanged();
    }

    protected HandyPlane() {
        m_plane = null;
        m_geocentric = null;
        m_strike = 0;
        m_dip = m_dip_start;
        m_show_strike_plane = true;
        m_size = 1.0;
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

    // Estos dos los dejo de muestra por si las moscas, sino volarlos
    /*public void updateLocation(Geodetic3D location){
        if (m_poi == null){
            m_poi = new POI("dummy", "");
            m_poi.setGraphic(new Entity(m_poi.getId(), new BillboardGraphics(PoiBillboardBuilder.getIconPath(m_billboard_icon), PoiBillboardBuilder.getIconAtlasId(m_billboard_icon), m_billboard_size, m_billboard_size)));
            m_ArgeoFragment.getViewer().getEntities().add(m_poi.getGraphic());
        }

        m_poi.getGraphic().setPosition(location);
        notifyPositionPoiChanged();
    }

    public void updateLocation(Geocentric3D location){
        if (m_poi == null){
            m_poi = new POI("dummy", "");
            m_poi.setGraphic(new Entity(m_poi.getId(), new BillboardGraphics(PoiBillboardBuilder.getIconPath(m_billboard_icon), PoiBillboardBuilder.getIconAtlasId(m_billboard_icon), m_billboard_size, m_billboard_size)));
            m_ArgeoFragment.getViewer().getEntities().add(m_poi.getGraphic());
        }

        m_poi.getGraphic().setPosition(location);
        notifyPositionPoiChanged();
    }*/

    public void updatePlaneLocation(Geocentric3D location) {
        clear(false);
        m_geocentric = location;
        m_plane = new Plane("dummy", "", "");
        EllipsoidTangentPlane tangent = new EllipsoidTangentPlane(EllipsoidTransformations.geodetic3DFromGeocentric3D(location));
        //m_plane.setStrikePlaneGraphic(new Entity(m_plane.getId(), new PlaneGraphics(tangent, true)));
        //m_plane.setDippingPlaneGraphic(new Entity(m_plane.getId().concat("dip"), new PlaneGraphics(new EllipsoidTangentPlane(EllipsoidTransformations.geodetic3DFromGeocentric3D(location)), false)));
        //m_plane.getDippingPlaneGraphic().setOrientation(Quaternion.fromAxisAngle(tangent.getXAxis(), m_dip / 180.0 * 3.14));
        m_plane.setStrikePlane(new PlaneGraphics(tangent, true));
        m_plane.setDippingPlane(new PlaneGraphics(new EllipsoidTangentPlane(EllipsoidTransformations.geodetic3DFromGeocentric3D(location)), false));
        m_plane.getStrikePlane().setWidth(m_size);
        m_plane.getDippingPlane().setWidth(m_size);
        m_plane.setStrikePlaneGraphic(new Entity(m_plane.getId(), m_plane.getStrikePlane()));
        m_plane.setDippingPlaneGraphic(new Entity(m_plane.getId().concat("dip"), m_plane.getDippingPlane()));
        //m_plane.getDippingPlaneGraphic().setOrientation(Quaternion.fromAxisAngle(tangent.getXAxis(), m_dip / 180.0 * 3.14));
        setOrientations(m_plane);
        if (m_show_strike_plane) {
            m_ArgeoFragment.getViewer().getEntities().add(m_plane.getStrikePlaneGraphic());
        }
        m_ArgeoFragment.getViewer().getEntities().add(m_plane.getDippingPlaneGraphic());
        notifyPlanePositionChanged();
    }

    private void setOrientations(Plane plane) {
        double value_strike = m_strike;
        double value_dip = m_dip;

        EllipsoidTangentPlane tangent = new EllipsoidTangentPlane(EllipsoidTransformations.geodetic3DFromGeocentric3D(m_geocentric));
        Quaternion q_s = Quaternion.fromAxisAngle(tangent.getNormal(), value_strike / 180 * 3.14);
        Quaternion q_d = Quaternion.fromAxisAngle(tangent.getXAxis(), value_dip / 180.0 * 3.14);
        Quaternion q_d_final;

        // a ver que pasa....
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

        plane.getStrikePlaneGraphic().setOrientation(q_s);
        plane.getDippingPlaneGraphic().setOrientation(q_d_final);
    }

    public void updatePlaneStrike(int strike) {
        if (m_plane != null) {
            m_strike = strike;
            double value_strike = m_strike;
            double value_dip = m_dip;

            EllipsoidTangentPlane tangent = new EllipsoidTangentPlane(EllipsoidTransformations.geodetic3DFromGeocentric3D(m_geocentric));
            Quaternion q_s = Quaternion.fromAxisAngle(tangent.getNormal(), value_strike / 180 * 3.14);
            Quaternion q_d = Quaternion.fromAxisAngle(tangent.getXAxis(), value_dip / 180.0 * 3.14);
            Quaternion q_d_final;

            // a ver que pasa....
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

            m_plane.getStrikePlaneGraphic().setOrientation(q_s);
            m_plane.getDippingPlaneGraphic().setOrientation(q_d_final);

            notifyPlaneStrikenChanged();
        }
    }

    public void updatePlaneDip(int dip) {
        if (m_plane != null) {
            m_dip = dip + m_dip_start;
            double value_strike = m_strike;
            double value_dip = m_dip;

            EllipsoidTangentPlane tangent = new EllipsoidTangentPlane(EllipsoidTransformations.geodetic3DFromGeocentric3D(m_geocentric));
            Quaternion q_s = Quaternion.fromAxisAngle(tangent.getNormal(), value_strike / 180 * 3.14);
            Quaternion q_d = Quaternion.fromAxisAngle(tangent.getXAxis(), value_dip / 180.0 * 3.14);
            Quaternion q_d_final;

            // a ver que pasa....
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
            m_plane.getDippingPlaneGraphic().setOrientation(q_d_final);

            notifyPlaneDipChanged();
        }
    }

    public void updatePlaneSize(int size) {
        if (m_plane != null) {
            Geocentric3D location = m_geocentric;
            clear(false);
            m_size = size;
            m_geocentric = location;
            m_plane = new Plane("dummy", "", "");
            EllipsoidTangentPlane tangent = new EllipsoidTangentPlane(EllipsoidTransformations.geodetic3DFromGeocentric3D(m_geocentric));
            m_plane.setStrikePlane(new PlaneGraphics(tangent, true));
            m_plane.setDippingPlane(new PlaneGraphics(new EllipsoidTangentPlane(EllipsoidTransformations.geodetic3DFromGeocentric3D(m_geocentric)), false));
            m_plane.getStrikePlane().setWidth(m_size);
            m_plane.getDippingPlane().setWidth(m_size);
            m_plane.setStrikePlaneGraphic(new Entity(m_plane.getId(), m_plane.getStrikePlane()));
            m_plane.setDippingPlaneGraphic(new Entity(m_plane.getId().concat("dip"), m_plane.getDippingPlane()));
            //m_plane.getDippingPlaneGraphic().setOrientation(Quaternion.fromAxisAngle(tangent.getXAxis(), m_dip / 180.0 * 3.14));
            setOrientations(m_plane);
            if (m_show_strike_plane) {
                m_ArgeoFragment.getViewer().getEntities().add(m_plane.getStrikePlaneGraphic());
            }
            m_ArgeoFragment.getViewer().getEntities().add(m_plane.getDippingPlaneGraphic());

            notifyPlaneSizeChanged();
        }
    }

    public void updateShowStrikePlane(boolean value) {
        if (m_show_strike_plane) {
            m_ArgeoFragment.getViewer().getEntities().remove(m_plane.getStrikePlaneGraphic());
        }
        m_show_strike_plane = value;
        updatePlaneLocation(m_geocentric);
    }

    public void clear(boolean angles) {
        if (m_plane != null) {
            if (m_show_strike_plane) {
                m_ArgeoFragment.getViewer().getEntities().remove(m_plane.getStrikePlaneGraphic());
            }
            m_ArgeoFragment.getViewer().getEntities().remove(m_plane.getDippingPlaneGraphic());
            m_plane.setStrikePlaneGraphic(null);
            m_plane.setDippingPlaneGraphic(null);
            m_plane.setStrikePlane(null);
            m_plane.setDippingPlane(null);
            m_plane = null;
            m_geocentric = null;
            if (angles) {
                m_strike = 0;
                m_dip = m_dip_start;
                m_size = 1.0;
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

    void notifyPlanePositionChanged() {
        for (PlaneChanged listener : m_plane_listeners) {
            listener.onPlanePositionChanged();
        }
    }

    void notifyPlaneStrikenChanged() {
        for (PlaneChanged listener : m_plane_listeners) {
            listener.onPlaneStrikeChanged();
        }
    }

    void notifyPlaneDipChanged() {
        for (PlaneChanged listener : m_plane_listeners) {
            listener.onPlaneDipChanged();
        }
    }

    void notifyPlaneSizeChanged() {
        for (PlaneChanged listener : m_plane_listeners) {
            listener.onPlaneSizeChanged();
        }
    }


    //TODO volarlo y hacerlo bien
    public Plane clonePlane() {
        Geocentric3D location = m_geocentric;
        clear(false);
        m_geocentric = location;
        Plane plane = new Plane("dummy", "", "");
        EllipsoidTangentPlane tangent = new EllipsoidTangentPlane(EllipsoidTransformations.geodetic3DFromGeocentric3D(location));
        //m_plane.setStrikePlaneGraphic(new Entity(m_plane.getId(), new PlaneGraphics(tangent, true)));
        //m_plane.setDippingPlaneGraphic(new Entity(m_plane.getId().concat("dip"), new PlaneGraphics(new EllipsoidTangentPlane(EllipsoidTransformations.geodetic3DFromGeocentric3D(location)), false)));
        //m_plane.getDippingPlaneGraphic().setOrientation(Quaternion.fromAxisAngle(tangent.getXAxis(), m_dip / 180.0 * 3.14));
        plane.setStrikePlane(new PlaneGraphics(tangent, true));
        plane.setDippingPlane(new PlaneGraphics(new EllipsoidTangentPlane(EllipsoidTransformations.geodetic3DFromGeocentric3D(location)), false));
        plane.getStrikePlane().setWidth(m_size);
        plane.getDippingPlane().setWidth(m_size);
        plane.setStrikePlaneGraphic(new Entity(plane.getId(), plane.getStrikePlane()));
        plane.setDippingPlaneGraphic(new Entity(plane.getId().concat("dip"), plane.getDippingPlane()));
        //m_plane.getDippingPlaneGraphic().setOrientation(Quaternion.fromAxisAngle(tangent.getXAxis(), m_dip / 180.0 * 3.14));
        setOrientations(plane);
        if (m_show_strike_plane) {
            m_ArgeoFragment.getViewer().getEntities().add(plane.getStrikePlaneGraphic());
        }
        m_ArgeoFragment.getViewer().getEntities().add(plane.getDippingPlaneGraphic());
        notifyPlanePositionChanged();

        return plane;
    }
}