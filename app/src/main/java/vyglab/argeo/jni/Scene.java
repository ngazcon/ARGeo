package vyglab.argeo.jni;

import android.graphics.Bitmap;

public class Scene extends NativeObject
{
    private ITerrain mTerrain;
    private Camera   mCamera;
    private Camera.Memento m_camera_memento;

    public static class Memento {
        private final Camera m_camera;
        // Poner los 3 vectores que dijo don trippel nagel

        public Memento(Camera camera) {
            m_camera = camera;
        }

        public Camera getdtate() {
            return m_camera;
        }
    }

    private Scene()
    { }

    public ITerrain getTerrain() {
        if (this.mTerrain == null) {
            this.mTerrain = nativeGetTerrain();
        }
        return this.mTerrain;
    }

    public Camera getCamera() {
        if (this.mCamera == null) {
            this.mCamera = nativeGetCamera();
        }
        return this.mCamera;
    }

    public void saveCameraState() {
        m_camera_memento = mCamera.createMemento();
    }

    public  void restoreCameraState() {
        mCamera.restoreFromMemento(m_camera_memento);
    }

    public native boolean getUseVR();
    public native void  setUseVR(boolean value);

    public native float getTerrainOpacity();
    public native void  setTerrainOpacity(float value);

    public native void   setMode(int value);
    public native void   takeSnapshot();
    public native void   takeARBufferSnapshot();

    public native void    takeTTARSnapshot(
            Bitmap reality_snapshot,
            Bitmap ttar_snapshot,
            Camera camera,
            int width,
            int height);

    public native boolean addSnapshotListener(SnapshotListener listener);
    public native boolean removeSnapshotListener(SnapshotListener listener);


    protected native void dispose();

    public native ITerrain nativeGetTerrain();
    public native Camera   nativeGetCamera();
}
