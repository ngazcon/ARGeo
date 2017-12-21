package vyglab.argeo.app.model;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;

/**
 * Created by ngazcon on 09/03/2017.
 */

public class PlaneRepository {
    private static PlaneRepository m_instance = null;
    private HashMap<String, Plane> m_repo;
    private List<PlaneRepositoryChanged> m_repo_listeners = new ArrayList<PlaneRepositoryChanged>();

    public interface PlaneRepositoryChanged {
        void onPlaneAdded(Plane plane);
        void onPlaneRemoved(Plane plane);
    }

    protected PlaneRepository() {
        m_repo = new HashMap<String, Plane>();
    }

    public static PlaneRepository getInstance() {
        if (m_instance == null) {
            m_instance = new PlaneRepository();
        }

        return m_instance;
    }

    public void addPlane(Plane plane) {
        //TODO repo provisorio, para evitar duplicate entries (el id del poi no se controla q sea unico). Cuidado que esto va tarer problemas con el update en la BD
        boolean inserted = false;
        int count = 0;
        String key = plane.getId();

        while (!inserted) {
            if (m_repo.containsKey(key)) {
                count++;
                key = new String(plane.getId()).concat(" - Duplicate entry ").concat(String.valueOf(count));
            } else {
                // Update de poi id and insert
                plane.setId(key);
                m_repo.put(key, plane);
                inserted = true;
                notifyPlaneAdded(plane);
            }
        }
    }

    public void removePlane(Plane plane) {
        String key = plane.getId();

        m_repo.remove(key);
        notifyPlaneRemoved(plane);
    }

    public String listRepoDescription() {
        String output = "List of Planes\n";
        Collection<Plane> collection = m_repo.values();

        for (Plane plane : collection) {
            output = output.concat(plane.getId()).concat(" - ").concat(plane.getDescription()).concat("\n");
        }

        return output;
    }

    public List<Plane> getPlanes() {
        return new ArrayList<>(m_repo.values());
    }
    //TODO method - Listar los POIs respecto a un threshold (distancia a un punto por ejemplo)

    // Methods for update location listeners
    public void addListener(PlaneRepositoryChanged listener) {
        m_repo_listeners.add(listener);
    }

    void notifyPlaneAdded(Plane plane) {
        for (PlaneRepositoryChanged listener : m_repo_listeners) {
            listener.onPlaneAdded(plane);
        }
    }

    void notifyPlaneRemoved(Plane plane) {
        for (PlaneRepositoryChanged listener : m_repo_listeners) {
            listener.onPlaneRemoved(plane);
        }
    }
}