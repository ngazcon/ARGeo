package vyglab.argeo.app.controller.UserInterfaceState;

import android.view.View;

import vyglab.argeo.app.MainActivityState;

/**
 * Created by ngazcon on 27/10/2017.
 */

public class ListenerForUITransition implements View.OnClickListener {
    protected int m_context;
    protected int m_interaction;

    public ListenerForUITransition(int context, int interaction){
        m_context = context;
        m_interaction = interaction;
    }

    @Override
    public void onClick(View v){
        UIContextManager.getInstance().next(m_context, m_interaction);
        UIContextManager.getInstance().request(m_context);
    }
}