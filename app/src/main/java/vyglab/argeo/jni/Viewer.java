package vyglab.argeo.jni;

public class Viewer
        extends NativeObject
{
    private Scene mScene;
    private EntityCollection mEntityCollection;

    private Viewer()
    { }

    public  native void stopRenderLoop();
    public  native void startRenderLoop();

    public  native boolean isRunning();

    public  native boolean addStateListener(ViewerStateListener listener);
    public  native boolean removeStateListener(ViewerStateListener listener);

    public  native void executeOnRenderLoop(RenderLoopRunnable runnable);

    public  EntityCollection getEntities() {
        if (this.mEntityCollection == null) {
            this.mEntityCollection = nativeGetEntities();
        }
        return this.mEntityCollection;
    }

    public Scene getScene() {
        if (this.mScene == null) {
            this.mScene = nativeGetScene();
        }
        return this.mScene;
    }

    public  native Entity		pickEntity(int windowX, int windowY);
    public  native Geocentric3D pickPosition(int windowX, int windowY);

    public  native ScreenSpaceCameraController getScreenSpaceCameraController();
    public  native DevicePoseCameraController  getDevicePoseCameraController();

    protected native void initialize();
    protected native void dispose();

    private  native EntityCollection nativeGetEntities();
    private  native Scene nativeGetScene();
}
