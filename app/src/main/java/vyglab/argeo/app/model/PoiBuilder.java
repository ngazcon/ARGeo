package vyglab.argeo.app.model;

/**
 * Created by root on 29/10/16.
 */

public abstract class PoiBuilder {
    protected POI m_poi;

    protected String m_poi_id;
    protected String m_poi_desc;
    protected Geodetic3D m_poi_position;

    public POI getPoi() {
        return m_poi;
    }

    public void configurePoiBuilder(String id, String description, Geodetic3D position){
        m_poi_id = id;
        m_poi_desc = description;
        m_poi_position = position;
    }

    public void buildPoi() {
        m_poi = new POI(m_poi_id, m_poi_desc);
        m_poi.setVisible(true);
        m_poi.setClamped(false);
        m_poi.setOrientation(null);
        m_poi.setPosition(m_poi_position);
    }

    public abstract void createNewPoi();
}
