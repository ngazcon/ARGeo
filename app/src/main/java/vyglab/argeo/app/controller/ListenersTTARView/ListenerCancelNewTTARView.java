package vyglab.argeo.app.controller.ListenersTTARView;

import android.view.View;

import vyglab.argeo.app.MainActivityState;
import vyglab.argeo.app.controller.UserInterfaceState.UIContextManager;
import vyglab.argeo.app.controller.UserInterfaceState.UIState;

/**
 * Created by root on 16/02/17.
 */

public class ListenerCancelNewTTARView implements View.OnClickListener {

    public ListenerCancelNewTTARView(){
    }

    @Override
    public void onClick(View v){
        UIContextManager.getInstance().next(MainActivityState.ApplicationMode.TTARVIEW, UIState.Interactions.SECONDARY_FAB_2);
        UIContextManager.getInstance().request(MainActivityState.ApplicationMode.TTARVIEW);
    }
}