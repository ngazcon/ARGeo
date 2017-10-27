package vyglab.argeo.app.controller.SecondaryFabStateMachine;

import android.view.View;

import vyglab.argeo.app.MainActivityState;

import java.util.HashMap;


/**
 * Created by ngazcon on 23/11/2016.
 */

public class SecondaryFabContext implements MainActivityState.AppModeStateChangedListener {

    private SecondaryFabState m_mainstate_terrain;
    private SecondaryFabState m_mainstate_poi;
    private SecondaryFabState m_mainstate_plane;
    private SecondaryFabState m_mainstate_ttarview;

    private SecondaryFabState m_current_state;

    private HashMap<String, SecondaryFabState> m_states_map;

    public SecondaryFabContext() {
        m_mainstate_terrain = null;
        m_mainstate_poi = null;
        m_mainstate_plane = null;
        m_mainstate_ttarview = null;

        m_current_state = null;

        m_states_map = new HashMap<String, SecondaryFabState>();
    }

    @Override
    public void onAppModeStateChanged(MainActivityState.AppMode mode){
        switch (mode){
            case TERRAIN :
                m_current_state = m_mainstate_terrain;
                this.setState(m_current_state);
                this.getState().handle();
                break;

            case POI :
                m_current_state = m_mainstate_poi;
                this.setState(m_current_state);
                this.getState().handle();
                break;

            case PLANE :
                m_current_state = m_mainstate_plane;
                this.setState(m_current_state);
                this.getState().handle();
                break;

            /*case TTARVIEW :
                m_current_state = m_mainstate_ttarview;
                this.setState(m_current_state);
                this.getState().handle();
                break;*/
        }
    }

    @Override
    public void onApplicationModeChanged(int mode){
    }

    public void setMainStates(SecondaryFabState state_terrain, SecondaryFabState state_poi, SecondaryFabState state_plane, SecondaryFabState state_ttarview){
        m_mainstate_terrain = state_terrain;
        m_mainstate_poi = state_poi;
        m_mainstate_plane = state_plane;
        m_mainstate_ttarview = state_ttarview;
    }

    public void setState(SecondaryFabState state){
        m_current_state = state;
    }

    public SecondaryFabState getState(){
        return m_current_state;
    }

    public void registerState(String key, SecondaryFabState state){
        m_states_map.put(key, state);
    }

    public SecondaryFabState getRegistredState(String key){
        return m_states_map.get(key);
    }

    public void goNextState(View.OnClickListener caller){
        m_current_state.nextState(this, caller);
    }

    public void goNextState(String transition_label){
        m_current_state.nextState(this, transition_label);
    }

    public void goNextStateAndHandle(View.OnClickListener caller){
        m_current_state.nextState(this, caller);
        m_current_state.handle();
    }

    public void request(){
        m_current_state.handle();
    }
}
