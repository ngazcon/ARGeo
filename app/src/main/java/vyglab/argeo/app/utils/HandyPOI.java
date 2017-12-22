package vyglab.argeo.app.utils;

import android.util.Log;

import vyglab.argeo.jni.ArgeoFragment;
import vyglab.argeo.jni.BillboardGraphics;
import vyglab.argeo.jni.EllipsoidTangentPlane;
import vyglab.argeo.jni.EllipsoidTransformations;
import vyglab.argeo.jni.Entity;
import vyglab.argeo.jni.Geocentric3D;
import vyglab.argeo.jni.Geodetic3D;

import vyglab.argeo.jni.PlaneGraphics;
import vyglab.argeo.jni.Quaternion;
import vyglab.argeo.app.controller.PositionPoiChanged;
import vyglab.argeo.app.model.POI;
import vyglab.argeo.app.model.PoiBillboardBuilder;
import vyglab.argeo.jni.enums.HeightClamping;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by ngazcon on 26/10/2016.
 */

public class HandyPOI {
            private static HandyPOI m_instance = null;
            private POI m_poi;
            private ArgeoFragment m_ArgeoFragment;
            private PoiBillboardBuilder.BillboardIcon m_billboard_icon = PoiBillboardBuilder.BillboardIcon.DEFAULT;
            private int m_billboard_size = 50;
            private List<PositionPoiChanged> m_poi_listeners = new ArrayList<PositionPoiChanged>();

    protected HandyPOI(){
                m_poi = null;
    }

    public static HandyPOI getInstance() {
        if(m_instance == null) {
            m_instance = new HandyPOI();
        }

        return m_instance;
    }

    public void setup( ArgeoFragment fragment ){
        m_ArgeoFragment = fragment;
    }

    public void updateLocation(Geodetic3D location){
        if (m_poi == null){
            m_poi = new POI("dummy", "");
            m_poi.setGraphic(new Entity(m_poi.getId(), new BillboardGraphics(PoiBillboardBuilder.getIconPath(m_billboard_icon), PoiBillboardBuilder.getIconAtlasId(m_billboard_icon), m_billboard_size, m_billboard_size, HeightClamping.NONE)));
            m_ArgeoFragment.getViewer().getEntities().add(m_poi.getGraphic());
        }

        m_poi.getGraphic().setPosition(location);
        notifyPositionPoiChanged();
    }

    public void updateLocation(Geocentric3D location){
        if (m_poi == null){
            m_poi = new POI("dummy", "");
            m_poi.setGraphic(new Entity(m_poi.getId(), new BillboardGraphics(PoiBillboardBuilder.getIconPath(m_billboard_icon), PoiBillboardBuilder.getIconAtlasId(m_billboard_icon), m_billboard_size, m_billboard_size, HeightClamping.NONE)));
            m_ArgeoFragment.getViewer().getEntities().add(m_poi.getGraphic());
        }

        m_poi.getGraphic().setPosition(location);
        notifyPositionPoiChanged();
    }

    public void updateLocationPlane(Geocentric3D location){
        clear();
            m_poi = new POI("dummy", "");
            POI m_poi2 = new POI("dummy", "");

            //Geocentric3D geocentric = HandyPOI.getInstance().getPOI().getGraphic().getPosition();
            vyglab.argeo.jni.Geodetic3D geodetic = EllipsoidTransformations.geodetic3DFromGeocentric3D(location);

        EllipsoidTangentPlane plane = new EllipsoidTangentPlane(geodetic);

        m_poi.setGraphic(new Entity(m_poi.getId(),
                new PlaneGraphics(plane, true)));

        m_poi2.setGraphic(new Entity(m_poi2.getId() + "pp",
            new PlaneGraphics(plane, false)));

        m_poi2.getGraphic().setOrientation(Quaternion.fromAxisAngle(plane.getXAxis(), 3.14 * 90.0 / 180.0));

         //m_poi.getGraphic().setPosition(location);
            m_ArgeoFragment.getViewer().getEntities().add(m_poi.getGraphic());
        m_ArgeoFragment.getViewer().getEntities().add(m_poi2.getGraphic());

        notifyPositionPoiChanged();
    }

    public void updateBillboardIcon(PoiBillboardBuilder.BillboardIcon icon){
        m_billboard_icon = icon;
        if (m_poi != null){
            double x = m_poi.getGraphic().getPosition().getX();
            double y = m_poi.getGraphic().getPosition().getY();
            double z = m_poi.getGraphic().getPosition().getZ();
            m_ArgeoFragment.getViewer().getEntities().remove(m_poi.getGraphic());
            Log.d("POI-REMOVE", m_poi.getGraphic().getId());
            //m_poi.setGraphic(new Entity(String.valueOf(++int_trippel_nagel), m_poi.getId(), new BillboardGraphics(PoiBillboardBuilder.getIconPath(m_billboard_icon), PoiBillboardBuilder.getIconAtlasId(m_billboard_icon), m_billboard_size, m_billboard_size)));
            m_poi.setGraphic(new Entity(m_poi.getId(), new BillboardGraphics(PoiBillboardBuilder.getIconPath(m_billboard_icon), PoiBillboardBuilder.getIconAtlasId(m_billboard_icon), m_billboard_size, m_billboard_size, HeightClamping.NONE)));
            m_poi.getGraphic().setPosition(new Geocentric3D(x, y, z));
            m_ArgeoFragment.getViewer().getEntities().add(m_poi.getGraphic());
            Log.d("POI-ADD", m_poi.getGraphic().getId());
        }
    }

    public void updateBillboardSize(int size){
        m_billboard_size = size;
        if (m_poi != null){
            double x = m_poi.getGraphic().getPosition().getX();
            double y = m_poi.getGraphic().getPosition().getY();
            double z = m_poi.getGraphic().getPosition().getZ();
            m_ArgeoFragment.getViewer().getEntities().remove(m_poi.getGraphic());
            m_poi.setGraphic(new Entity(m_poi.getId(), new BillboardGraphics(PoiBillboardBuilder.getIconPath(m_billboard_icon), PoiBillboardBuilder.getIconAtlasId(m_billboard_icon), m_billboard_size, m_billboard_size, HeightClamping.NONE)));
            m_poi.getGraphic().setPosition(new Geocentric3D(x, y, z));
            m_ArgeoFragment.getViewer().getEntities().add(m_poi.getGraphic());
        }
    }

    public void clear(){
        if (m_poi != null){
            m_ArgeoFragment.getViewer().getEntities().remove(m_poi.getGraphic());
            m_poi.setGraphic(null);
            m_poi = null;

            m_billboard_icon = PoiBillboardBuilder.BillboardIcon.DEFAULT;
            m_billboard_size = 50;
        }
    }

    public void clear2(){
        if (m_poi != null){
            //m_ArgeoFragment.getViewer().getEntities().remove(m_poi.getGraphic());
            //m_poi.setGraphic(null);
            m_poi = null;

            m_billboard_icon = PoiBillboardBuilder.BillboardIcon.DEFAULT;
            m_billboard_size = 50;
        }
    }

    public POI getPOI(){
        return m_poi;
    }

    // Methods for update location listeners
    public void addListener(PositionPoiChanged listener) {
        m_poi_listeners.add(listener);
    }

    public void removeListener(PositionPoiChanged listener) {
        m_poi_listeners.remove(listener);
    }

    void notifyPositionPoiChanged(){
        for(PositionPoiChanged listener : m_poi_listeners){
            listener.onPositionPoiChanged();
        }
    }
}
