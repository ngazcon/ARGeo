package vyglab.argeo.app.model;

/**
 * Created by root on 02/12/17.
 */

public class PlaneBuilder {
    private String m_id;
    private String m_name;
    private String m_description;
    private Geodetic3D m_position;
    private int m_dip;
    private int m_vorientation;
    private int m_strike;
    private int m_size;
    private int m_thickness;

    public PlaneBuilder() {

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

    public PlaneBuilder setPosition(Geodetic3D position) {
        m_position = position;
        return this;
    }

    public PlaneBuilder setDip(int dip) {
        m_dip = dip;
        return this;
    }

    public PlaneBuilder setVOrientation(int vorientation) {
        m_vorientation = vorientation;
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

    public Plane build() {
        Plane plane = new Plane(m_id, m_name, m_description);
        plane.setPosition(m_position);
        plane.setDip(m_dip);
        plane.setVirtualOrientation(m_vorientation);
        plane.setStrike(m_strike);
        plane.setSize(m_size);
        plane.setThickness(m_thickness);
        // ahora uso los valores de dip y vorientation para crear los planos con su orientacion
        // para crear los planos creo la entity y su respectivo igraphic plano
        // tambien uso los valores de size y thickness para el width y el height respectivamente

        return plane;
    }

}
