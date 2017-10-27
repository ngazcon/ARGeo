package vyglab.argeo.app.controller.UserInterfaceState;

import java.util.HashMap;

import vyglab.argeo.app.MainActivityState;

/**
 * Created by ngazcon on 25/10/2017.
 */

public class UIContextManager implements MainActivityState.AppModeStateChangedListener {
    private static UIContextManager m_instance;
    private HashMap<Integer, UIContext> m_contexts;

    protected UIContextManager() {
        m_contexts = new HashMap<Integer, UIContext>();
    }

    public static UIContextManager getInstance() {
        if (m_instance == null) {
            m_instance = new UIContextManager();
        }

        return  m_instance;
    }

    public void registerContext(int key, UIContext context) {
        m_contexts.put(key, context);
    }

    public void request(int context_id) {
        UIContext context = m_contexts.get(context_id);
        context.request();
    }

    public void next(int context_id, int interaction) {
        UIContext context = m_contexts.get(context_id);
        context.next(interaction);
    }

    @Override
    public void onAppModeStateChanged(MainActivityState.AppMode mode){
    }

    @Override
    public void onApplicationModeChanged(int mode){

        switch (mode){
            case MainActivityState.ApplicationMode.TTARVIEW :
                m_contexts.get(mode).getState().handle();
                break;
        }
    }
}
