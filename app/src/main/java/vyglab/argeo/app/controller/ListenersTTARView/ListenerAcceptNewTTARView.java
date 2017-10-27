package vyglab.argeo.app.controller.ListenersTTARView;

import android.view.View;

import vyglab.argeo.app.controller.UserInterfaceState.UIContextManager;
import vyglab.argeo.app.controller.UserInterfaceState.UIState;
import vyglab.argeo.app.MainActivityState;

/**
 * Created by root on 16/02/17.
 */

public class ListenerAcceptNewTTARView implements View.OnClickListener {

    public ListenerAcceptNewTTARView(){
    }

    @Override
    public void onClick(View v){
        UIContextManager.getInstance().next(MainActivityState.ApplicationMode.TTARVIEW, UIState.Interactions.SECONDARY_FAB_1);
        UIContextManager.getInstance().request(MainActivityState.ApplicationMode.TTARVIEW);
    }
}