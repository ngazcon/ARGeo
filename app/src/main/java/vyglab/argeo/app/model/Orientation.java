package vyglab.argeo.app.model;

/**
 * Created by ngazcon on 05/07/2016.
 */
public class Orientation {
    private double m_yaw;
    private double m_pitch;
    private double m_roll;

    public Orientation() {
        m_yaw = 0.0;
        m_pitch = 0.0;
        m_roll = 0.0;
    }

    /**
     *
     * @param yaw Value in radian.
     * @param pitch Value in radian.
     * @param roll Value in radian.
     */
    public Orientation( double yaw, double pitch, double roll) {
        m_yaw = yaw;
        m_pitch = pitch;
        m_roll = roll;
    }

    /**
     *
     * @param yaw Value in radian.
     */
    public void setYaw( double yaw ) { m_yaw = yaw; }
    public double getYaw() { return m_yaw; }

    /**
     *
     * @param yaw Value in radian.
     */
    public void setPitch( double yaw ) { m_yaw = yaw; }
    public double getPitch() { return m_pitch; }

    /**
     *
     * @param yaw Value in radian.
     */
    public void setRoll( double yaw ) { m_yaw = yaw; }
    public double getRoll() { return m_roll; }
}
