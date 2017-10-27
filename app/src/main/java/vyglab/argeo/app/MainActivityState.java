package vyglab.argeo.app;

import vyglab.argeo.app.controller.SecondaryFabStateMachine.SecondaryFabContext;
import vyglab.argeo.app.controller.TouchModeStateChangedListener;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by ngazcon on 14/09/2016.
 */
public class MainActivityState {
    public enum AppMode{
        NONE, TERRAIN, POI, PLANE, TTARVIEW
    }

    public static class ApplicationMode {
        public static final int TTARVIEW = 1;
    }

    public enum TouchMode{
        DEFAULT, POI_CREATION, PLANE_CREATION, CAMERA_TRANSLATION, CAMERA_ROTATION
    }

    public interface AppModeStateChangedListener{
        void onAppModeStateChanged(AppMode app_mode);
        void onApplicationModeChanged(int mode);
    }

    private AppMode m_app_mode;
    private TouchMode m_touch_mode;
    private List<AppModeStateChangedListener> m_app_mode_listeners = new ArrayList<AppModeStateChangedListener>();
    private List<AppModeStateChangedListener> m_application_mode_listeners = new ArrayList<AppModeStateChangedListener>();
    private List<TouchModeStateChangedListener> m_touch_listeners = new ArrayList<TouchModeStateChangedListener>();

    private boolean m_right_menu_terrain_visible;
    private boolean m_right_menu_poi_visible;
    private boolean m_right_menu_plane_visible;

    private SecondaryFabContext m_secondary_fab_context;



    public MainActivityState( ){
        m_app_mode = AppMode.NONE;
        m_touch_mode = TouchMode.DEFAULT;

        m_right_menu_terrain_visible = false;
        m_right_menu_poi_visible = false;
        m_right_menu_plane_visible = false;

        m_secondary_fab_context = null;
    }

    public AppMode getAppMode(){
        return m_app_mode;
    }

    public TouchMode getTouchMode(){
        return m_touch_mode;
    }

    public boolean getRightMenuTerrainVisible(){
        return m_right_menu_terrain_visible;
    }

    public boolean getRightMenuPoiVisible(){
        return m_right_menu_poi_visible;
    }

    public boolean getRightMenuPlaneVisible(){
        return m_right_menu_plane_visible;
    }

    public void setAppMode(AppMode mode){
        // If the state is different then notify the change. Otherwise do nothing.
        if ( mode != m_app_mode) {
            m_app_mode = mode;
            notifyAppModeStateChanged(m_app_mode);
        }
    }

    public void setApplicationMode(int mode) {
        notifyApplicationModeChanged(mode);
    }

    public void setTouchMode(TouchMode mode){
        m_touch_mode = mode;
        notifyTouchModeStateChanged(m_touch_mode);
    }

    public void setRightMenuTerrainVisible(boolean value){
        m_right_menu_terrain_visible = value;
    }

    public void setRightMenuPoiVisible(boolean value){
        m_right_menu_poi_visible = value;
    }

    public void setRightMenuPlaneVisible(boolean value){
        m_right_menu_plane_visible = value;
    }

    public void setSecondaryFabContext(SecondaryFabContext context){
        m_secondary_fab_context = context;
    }

    public SecondaryFabContext getSecondaryFabContext(){
        return m_secondary_fab_context;
    }

    // Methods for application mode change listeners
    public void addAppModeListener(AppModeStateChangedListener listener) {
        m_app_mode_listeners.add(listener);
    }

    public void removeAppModeListener(AppModeStateChangedListener listener) {
        m_app_mode_listeners.remove(listener);
    }

    void notifyAppModeStateChanged(AppMode new_mode){
        for(AppModeStateChangedListener listener : m_app_mode_listeners){
            listener.onAppModeStateChanged(new_mode);
        }
    }

    public void addApplicationModeChanged(AppModeStateChangedListener listener) {
        m_application_mode_listeners.add(listener);
    }

    void notifyApplicationModeChanged(int new_mode){
        for(AppModeStateChangedListener listener : m_application_mode_listeners){
            listener.onApplicationModeChanged(new_mode);
        }
    }

    // Methods for touch mode change listeners
    public void addTouchModeListener(TouchModeStateChangedListener listener) {
        m_touch_listeners.add(listener);
    }

    void notifyTouchModeStateChanged(TouchMode new_mode){
        for(TouchModeStateChangedListener listener : m_touch_listeners){
            listener.onTouchModeStateChanged(new_mode);
        }
    }
}
