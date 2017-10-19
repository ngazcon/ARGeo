package vyglab.argeo.jni;

public class PolylineGraphics extends IGraphics
{
    private PolylineGraphics()
    { }

    public PolylineGraphics(Geocentric3D[] points) {
        create(points);
    }

    public PolylineGraphics(
            Geocentric3D[] points,
            boolean        loop) {
        create(
                points,
                loop);
    }

    public PolylineGraphics(
            Geocentric3D[] points,
            boolean        loop,
            double         width) {
        create(
                points,
                loop,
                width);
    }

    private native void create(
            Geocentric3D[] points);

    private native void create(
            Geocentric3D[] points,
            boolean        loop);

    private native void create(
            Geocentric3D[] points,
            boolean        loop,
            double         width);

    public native void addPoint(Geocentric3D point);
}
