package vyglab.argeo.app.model;

/**
 * Created by ngazcon on 04/07/2016.
 *
 */
public class Geodetic3D {
    private double m_long;
    private double m_lat;
    private double m_height;

    public Geodetic3D(){
        m_long = 0.0;
        m_lat = 0.0;
        m_height = 0.0;
    }

    public Geodetic3D( double lat, double lon, double height ){
        m_long = lon;
        m_lat = lat;
        m_height = height;
    }

    public void setLong( double lon ) { m_long = lon; }
    public double getLong() { return m_long; }

    public void setLat( double lat ){ m_lat = lat; }
    public double getLat() { return m_lat; }

    public void setHeight( double height){ m_height = height; }
    public double getHeight() { return m_height; }

    public static String coordinateToPrintableText(double coordinate){
        double degrees;
        double minutes;
        double seconds;
        Double d;
        int degrees_int;
        int minutes_int;

        if (Double.isNaN(coordinate))
            return new String("---");

        degrees = Math.floor(coordinate);
        minutes = Math.floor((coordinate - degrees) * 60);
        seconds = 3600 * (coordinate - degrees) - (60 * minutes);

        d = degrees;
        degrees_int = d.intValue();
        d = minutes;
        minutes_int = d.intValue();

        String text = String.valueOf(degrees_int).concat("º ").concat(String.valueOf(minutes_int)).concat("' ").concat(String.format("%.2f", seconds)).concat("''");

        return text;
    }

    public static String heightToPrintableText(double height){
        String text = String.format("%.2f", height).concat("m");

        return text;
    }


}
