package vyglab.argeo.app.model;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;

/**
 * Created by root on 08/03/17.
 */

public class TTARViewRepository {
    private static TTARViewRepository m_instance = null;
    private HashMap<String, TTARView> m_repo;
    private List<TTARViewRepositoryChanged> m_repo_listeners = new ArrayList<TTARViewRepositoryChanged>();

    public interface TTARViewRepositoryChanged {
        void onTTARViewAdded(TTARView ttarview);
    }

    protected TTARViewRepository(){
        m_repo = new HashMap<String, TTARView>();
    }

    public static TTARViewRepository getInstance() {
        if(m_instance == null) {
            m_instance = new TTARViewRepository();
        }

        return m_instance;
    }

    public void addTTARView( TTARView ttarview ){
        //TODO repo provisorio, para evitar duplicate entries (el id del poi no se controla q sea unico). Cuidado que esto va tarer problemas con el update en la BD
        boolean inserted = false;
        int count = 0;
        String key = ttarview.getId();

        while (!inserted){
            if (m_repo.containsKey(key)){
                count++;
                key = new String(ttarview.getId()).concat(" - Duplicate entry ").concat(String.valueOf(count));
            } else {
                // Update de poi id and insert
                ttarview.setId(key);
                m_repo.put(key, ttarview);
                inserted = true;
                notifyPoiAdded(ttarview);
            }
        }
    }

    public String listRepoDescription(){
        String output = "List of TTARView's\n";
        Collection<TTARView> collection = m_repo.values();

        for( TTARView ttarview : collection ){
            output = output.concat(ttarview.getId()).concat(" - ").concat(ttarview.getName()).concat(" - ").concat(ttarview.getDescription()).concat("\n");
        }

        return output;
    }

    //TODO method - Listar los POIs respecto a un threshold (distancia a un punto por ejemplo)
    public List<TTARView> getTTARViews(){
        return new ArrayList<>(m_repo.values());
    }

    public int getSize() {
        return m_repo.size();
    }


    // Methods for update location listeners
    public void addListener(TTARViewRepositoryChanged listener) {
        m_repo_listeners.add(listener);
    }

    void notifyPoiAdded(TTARView ttarview){
        for(TTARViewRepositoryChanged listener : m_repo_listeners){
            listener.onTTARViewAdded(ttarview);
        }
    }
}
