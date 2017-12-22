package vyglab.argeo.app;

import vyglab.argeo.app.model.Plane;
import vyglab.argeo.app.model.PlaneBuilder;
import vyglab.argeo.jni.ArgeoFragment;
import vyglab.argeo.jni.EllipsoidTransformations;
import vyglab.argeo.jni.Geocentric3D;
import vyglab.argeo.jni.Geodetic3D;

/**
 * Created by ngazcon on 22/12/2017.
 */

public class MediatorArgeoFragment {
    private static MediatorArgeoFragment m_instance = null;

    private ArgeoFragment m_argeo_fragment;

    protected MediatorArgeoFragment() {
        // Nothing to do here
    }

    public static MediatorArgeoFragment getInstance() {
        if (m_instance == null) {
            m_instance = new MediatorArgeoFragment();
        }

        return m_instance;
    }

    public void init(ArgeoFragment fragment) {
        m_argeo_fragment = fragment;
    }

    public void cleanGraphics(Plane plane) {
        // Check if there is a plane
        if (plane != null) {
            // Check if the plane has plane graphics
            //if ((plane.getVirtualOrientationPlaneGraphic() != null) && (plane.getDippingPlaneGraphic() != null)) {
                //if (m_show_virtual_orientation_plane) {
                   // m_argeo_fragment.getViewer().getEntities().remove(plane.getVirtualOrientationPlaneEntity());
                //}
                m_argeo_fragment.getViewer().getEntities().remove(plane.getVirtualOrientationPlaneEntity());
                m_argeo_fragment.getViewer().getEntities().remove(plane.getDippingPlaneEntity());

                plane.setVirtualOrientationPlaneEntity(null);
                plane.setDippingPlaneEntity(null);
                plane.setVirtualOrientationPlaneGraphic(null);
                plane.setDippingPlaneGraphic(null);
            //}
        }
    }

    public void updateGraphics(Plane plane) {
        PlaneBuilder builder = new PlaneBuilder(m_argeo_fragment);

        // Build the plane
        Plane temp_plane = builder.setId(plane.getId())
                .setName(plane.getName())
                .setDescription(plane.getDescription())
                .setPosition(EllipsoidTransformations.geocentric3DFromGeodetic3D(new Geodetic3D(plane.getPosition().getLat(), plane.getPosition().getLong(), plane.getPosition().getHeight())))
                .setVirtualOrientation(plane.getVirtualOrientation())
                .setDip(plane.getDip())
                .setStrike(plane.getStrike())
                .setSize(plane.getSize())
                .setThickness(plane.getThickness())
                .setShowVirtualOrientationPlane(plane.getShowVirtualOrientationPlane())
                .build();

        plane.setDippingPlaneEntity(temp_plane.getDippingPlaneEntity());
        plane.setVirtualOrientationPlaneEntity(temp_plane.getDippingPlaneEntity());
        plane.setVirtualOrientationPlaneGraphic(temp_plane.getVirtualOrientationPlaneGraphic());
        plane.setDippingPlaneGraphic(temp_plane.getDippingPlaneGraphic());
    }
}
