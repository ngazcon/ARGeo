package vyglab.argeo.jni;

import vyglab.argeo.jni.annotations.UsedByNativeCode;

public class Camera extends NativeObject
{
    /**
     * Create a camera for the given scene.
     * @param scene The scene in which the camera is.
     */
    public Camera(Scene scene) {
        initialize(scene);
    }

    /***
     * Constructor used internally by the native code.
     */
    @SuppressWarnings("unused")
    private Camera()
    { }

    /***
     * Add a listener that receives an event when this camera moves.
     * @param listener The listener to add.
     * @return True if the listener was not already registered and has been added. False otherwise.
     */
    @UsedByNativeCode
    public native boolean addMoveListener(CameraMoveListener listener);

    /***
     * Remove a registered move listener from this camera.
     * @param listener The listener to remove.
     * @return True if camera had the given listener and it has been removed. False otherwise.
     */
    @UsedByNativeCode
    public native boolean removeMoveListener(CameraMoveListener listener);


    /***
     * Add a listener that receives an event when this camera state changed.
     * @param listener The listener to add.
     * @return True if the listener was not already registered and has been added. False otherwise.
     */
    @UsedByNativeCode
    public native boolean addChangedListener(CameraChangedListener listener);

    /***
     * Remove a registered change listener from this camera.
     * @param listener The listener to remove.
     * @return True if camera had the given listener and it has been removed. False otherwise.
     */
    @UsedByNativeCode
    public native boolean removeChangedListener(CameraChangedListener listener);

    /***
     * Get the minimum percentage of change needed to trigger a change event.
     * @return The minimum percentage.
     */
    @UsedByNativeCode
    public native double getPercentageChanged();

    /***
     * Set the minimum percentage of change needed to trigger a change event.
     * @param percentage The minimum percentage.
     */
    @UsedByNativeCode
    public native void setPercentageChanged(double percentage);


    /***
     * Get the yaw angle of this camera.
     * @return The angle in radians.
     */
    @UsedByNativeCode
    public native double getYaw();

    /***
     * Get the pitch angle of this camera.
     * @return The angle in radians.
     */
    @UsedByNativeCode
    public native double getPitch();

    /***
     * Get the roll angle of this camera.
     * @return The angle in radians.
     */
    @UsedByNativeCode
    public native double getRoll();


    @UsedByNativeCode
    public native void zoomIn(double amount);

    @UsedByNativeCode
    public native void zoomOut(double amount);

    @UsedByNativeCode
    public native void twistLeft(double angle);

    @UsedByNativeCode
    public native void twistRight(double angle);

    @UsedByNativeCode
    public native void lookLeft(double angle);

    @UsedByNativeCode
    public native void lookRight(double angle);

    @UsedByNativeCode
    public native void lookUp(double angle);

    @UsedByNativeCode
    public native void lookDown(double angle);

    @UsedByNativeCode
    public native void rotateLeft(double angle);

    @UsedByNativeCode
    public native void rotateRight(double angle);

    @UsedByNativeCode
    public native void rotateUp(double angle);

    @UsedByNativeCode
    public native void rotateDown(double angle);

    @UsedByNativeCode
    public native void moveForward(double amount);

    @UsedByNativeCode
    public native void moveBackward(double amount);

    @UsedByNativeCode
    public native void moveLeft(double amount);

    @UsedByNativeCode
    public native void moveRight(double amount);

    @UsedByNativeCode
    public native void moveUp(double amount);

    @UsedByNativeCode
    public native void moveDown(double amount);

    @UsedByNativeCode
    public native double getMagnitude();

    @UsedByNativeCode
    public native Geodetic3D getPositionGeodetic();

    @UsedByNativeCode
    public native Geocentric3D getPosition();

    @UsedByNativeCode
    public native Geocentric3D getDirection();

    @UsedByNativeCode
    public native Geocentric3D getUp();

    @UsedByNativeCode
    public native void setPosition(Geocentric3D position);

    @UsedByNativeCode
    public native void setDirection(Geocentric3D direction);

    @UsedByNativeCode
    public native void setUp(Geocentric3D up);

    @UsedByNativeCode
    public native void setView(
            Geocentric3D position,
            double yaw,
            double pitch,
            double roll);

    @UsedByNativeCode
    @Override
    public native void dispose();

    @UsedByNativeCode
    private native void initialize(Scene scene);

    // Memento stuff
    public static class Memento {
        private final Geocentric3D m_position;
        private final Geocentric3D m_direction;
        private final Geocentric3D m_up;

        public Memento(Geocentric3D position, Geocentric3D direction, Geocentric3D up) {
            m_position = position;
            m_direction = direction;
            m_up = up;
        }

        public Geocentric3D getPosition() {
            return m_position;
        }
        public Geocentric3D getDirection() {
            return m_direction;
        }
        public Geocentric3D getUp() {
            return m_up;
        }
    }

    public Memento createMemento() {
        return new Memento(this.getPosition(), this.getDirection(), this.getUp() );
    }

    public void restoreFromMemento(Memento memento){
        this.setPosition(memento.getPosition());
        this.setDirection(memento.getDirection());
        this.setUp(memento.getUp());
    }

}
