package vyglab.argeo.app.model;

/**
 * Created by ngazcon on 04/07/2016.
 */
abstract public class Object {
    private String m_id;

    public Object(){
        m_id = "Unknown";
    }

    /**
     *
     * @param id Must be a unique identifier.
     */
    public Object( String id ) { m_id = id; }

    /**
     *
     * @param id Must be a unique identifier.
     */
    public void setId(String id){
        m_id = id;
    }
    public String getId() { return m_id; }
}
