package vyglab.argeo.app.model;

import vyglab.argeo.jni.*;
import vyglab.argeo.jni.Geodetic3D;
import vyglab.argeo.app.controller.PoiRepositoryChanged;
import vyglab.argeo.jni.enums.HeightClamping;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;

/**
 * Created by ngazcon on 05/07/2016.
 */
public class POIRepository {
    private static POIRepository m_instance = null;
    private HashMap<String, POI> m_repo;
    private List<PoiRepositoryChanged> m_repo_listeners = new ArrayList<PoiRepositoryChanged>();

    protected POIRepository(){
        m_repo = new HashMap<String, POI>();

        // Generate dummy data for POI's
        POI dummy = new POI("Tres Picos", "Ernesto escalando este cerro.");
        dummy.setGraphic(new Entity(dummy.getId(), new BillboardGraphics("icons/ernesto.jpg", "ernesto", 100, 100, HeightClamping.GROUND)));
        dummy.getGraphic().setPosition(new Geodetic3D(-38.118629, -61.959423));
        dummy.setPosition(new vyglab.argeo.app.model.Geodetic3D(-38.118629, -61.959423, 0.0));
        //addPOI(dummy);
    }

    public static POIRepository getInstance() {
        if(m_instance == null) {
            m_instance = new POIRepository();
        }

        return m_instance;
    }

    public void addPOI( POI position ){
        //TODO repo provisorio, para evitar duplicate entries (el id del poi no se controla q sea unico). Cuidado que esto va tarer problemas con el update en la BD
        boolean inserted = false;
        int count = 0;
        String key = position.getId();

        while (!inserted){
            if (m_repo.containsKey(key)){
                count++;
                key = new String(position.getId()).concat(" - Duplicate entry ").concat(String.valueOf(count));
            } else {
                // Update de poi id and insert
                position.setId(key);
                m_repo.put(key, position);
                inserted = true;
                notifyPoiAdded(position);
            }
        }
    }

    public String listRepoDescription(){
        String output = "List of POI's\n";
        Collection<POI> collection = m_repo.values();

        for( POI pos : collection ){
            output = output.concat(pos.getId()).concat(" - ").concat(pos.getDescription()).concat("\n");
        }

        return output;
    }

    public List<POI> getPOIs(){
        return new ArrayList<>(m_repo.values());
    }
    //TODO method - Listar los POIs respecto a un threshold (distancia a un punto por ejemplo)

    // Methods for update location listeners
    public void addListener(PoiRepositoryChanged listener) {
        m_repo_listeners.add(listener);
    }

    void notifyPoiAdded(POI poi){
        for(PoiRepositoryChanged listener : m_repo_listeners){
            listener.onPoiAdded(poi);
        }
    }
}
