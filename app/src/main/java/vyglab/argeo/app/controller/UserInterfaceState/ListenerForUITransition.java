package vyglab.argeo.app.controller.UserInterfaceState;

import android.view.View;

import vyglab.argeo.app.MainActivityState;

/**
 * Created by ngazcon on 27/10/2017.
 */

public class ListenerForUITransition implements View.OnClickListener {
    private int m_interaction;

    public ListenerForUITransition(int interaction){
        m_interaction = interaction;
    }

    @Override
    public void onClick(View v){
        UIContextManager.getInstance().next(MainActivityState.ApplicationMode.TTARVIEW, m_interaction);
        UIContextManager.getInstance().request(MainActivityState.ApplicationMode.TTARVIEW);
    }
}