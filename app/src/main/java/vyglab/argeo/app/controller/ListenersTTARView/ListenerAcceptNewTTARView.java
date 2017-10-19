package vyglab.argeo.app.controller.ListenersTTARView;

import android.app.Activity;
import android.view.View;

import vyglab.argeo.jni.ArgeoFragment;
import vyglab.argeo.app.MainActivityState;
import vyglab.argeo.app.model.TTARView;
import vyglab.argeo.app.model.TTARViewRepository;
import vyglab.argeo.app.model.database.DBManager;
import vyglab.argeo.app.view.FragmentTTARView;

/**
 * Created by root on 16/02/17.
 */

public class ListenerAcceptNewTTARView implements View.OnClickListener {
    private Activity m_activity;
    private MainActivityState m_main_activity_state;
    private ArgeoFragment m_argeoFragment;
    private FragmentTTARView m_fragment_ttarview;
    private DBManager m_DBmanager;

    public ListenerAcceptNewTTARView(Activity activity, MainActivityState activity_state, ArgeoFragment fragment, FragmentTTARView fragment_ttarview, DBManager dbmanager){
        m_activity = activity;
        m_main_activity_state = activity_state;
        m_argeoFragment = fragment;
        m_fragment_ttarview = fragment_ttarview;
        m_DBmanager = dbmanager;
    }

    @Override
    public void onClick(View v){
        TTARView ttarview = m_fragment_ttarview.acceptCreation();
        TTARViewRepository.getInstance().addTTARView(ttarview);
        //TODO la siguiente es una solucion de compromiso, pensar bien como modelarlo (donde se hace)
        m_DBmanager.insertTTARView(ttarview);

        m_main_activity_state.getSecondaryFabContext().goNextState(this);
        m_main_activity_state.getSecondaryFabContext().getState().handle();
    }
}