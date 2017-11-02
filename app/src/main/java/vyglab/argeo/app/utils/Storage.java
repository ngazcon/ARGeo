package vyglab.argeo.app.utils;

import vyglab.argeo.app.model.TTARView;
import vyglab.argeo.app.model.database.DBManager;

/**
 * Created by ngazcon on 27/10/2017.
 */

public class Storage {
    private static Storage m_instance = null;
    private DBManager m_database;

    protected Storage(){
        // Nothing to do here:
    }

    public static Storage getInstance() {
        if (m_instance == null) {
            m_instance = new Storage();
        }

        return  m_instance;
    }

    public void init(DBManager db_manager) {
        m_database = db_manager;
    }

    //region TTARView
    public void insert(TTARView ttarview) {
        m_database.insertTTARView(ttarview);
    }

    public void delete(TTARView ttarview) {
        m_database.deleteTTARView(ttarview);
    }

    public void update(TTARView ttarview) {
        m_database.updateTTARView(ttarview);
    }
    //endregion
}
