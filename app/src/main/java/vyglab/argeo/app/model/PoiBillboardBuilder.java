package vyglab.argeo.app.model;

import vyglab.argeo.jni.*;
import vyglab.argeo.jni.Geodetic3D;
import vyglab.argeo.jni.enums.HeightClamping;

/**
 * Created by root on 29/10/16.
 */

public class PoiBillboardBuilder extends PoiBuilder {
    public enum BillboardIcon{
        DEFAULT, STAR, FLAG, CUSTOM
    }

    protected BillboardIcon m_icon;
    protected int m_width = 50;
    protected int m_height = 50;

    public static String getIconPath(BillboardIcon icon){
        String path = "icons/maps-marker.png";

        switch (icon){
            case DEFAULT :
                path = new String("icons/map-marker.png");
                break;

            case STAR :
                path = new String("icons/032.png");
                break;

            case FLAG :
                path = new String("icons/flag-variant.png");
                break;
        }

        return path;
    }

    public static BillboardIcon getIconFromPath(String path){
        BillboardIcon icon = BillboardIcon.CUSTOM;

        if (path.equals("icons/map-marker.png")){
            icon = BillboardIcon.DEFAULT;
        } else if (path.equals("icons/032.png")) {
            icon = BillboardIcon.STAR;
        } else if (path.equals("icons/flag-variant.png")){
            icon = BillboardIcon.FLAG;
        }

        return icon;
    }

    public static String getIconAtlasId(BillboardIcon icon){
        String id = "marker";

        switch (icon){
            case DEFAULT :
                id = new String("marker");
                break;

            case STAR :
                id = new String("star");
                break;

            case FLAG :
                id = new String("flag");
                break;
        }

        return id;
    }

    public void configureBillboardBuilder(BillboardIcon icon){
        m_icon = icon;
    }

    public void configureBillboardBuilder(BillboardIcon icon, int width, int height){
        m_icon = icon;
        m_width = width;
        m_height = height;
    }

    public void buildBillboard(){
        BillboardGraphics graphic = new BillboardGraphics(getIconPath(m_icon), getIconAtlasId(m_icon), m_width, m_height, HeightClamping.NONE);
        Entity billboard = new Entity(m_poi_id, graphic);
        billboard.setPosition(new Geodetic3D(m_poi_position.getLat(), m_poi_position.getLong(), m_poi_position.getHeight()));
        m_poi.setGraphic(billboard);
    }

    @Override
    public void createNewPoi(){
        buildPoi();
        buildBillboard();
    }
}
