package vyglab.argeo.app.model;

import java.util.HashMap;

/**
 * Created by ngazcon on 05/08/2016.
 */
public class ConfigState {
    private static ConfigState m_instance = null;

    private HashMap<String,String> m_paths;

    protected ConfigState() {
        // Exists only to defeat instantiation.
        m_paths = new HashMap<String, String>();
        m_paths.put("PATH_HEIGHTMAPS","/heightmaps/");
        m_paths.put("PATH_SRTM","/srtm");
        m_paths.put("PATH_MODELS","/models");
    }

    public static ConfigState getInstance() {
        if (m_instance == null) {
            m_instance = new ConfigState();
        }
        return m_instance;
    }

    public String getPathHeightmaps(){ return m_paths.get("PATH_HEIGHTMAPS");}
}
