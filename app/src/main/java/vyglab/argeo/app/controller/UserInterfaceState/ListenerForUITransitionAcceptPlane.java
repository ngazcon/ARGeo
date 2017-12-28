package vyglab.argeo.app.controller.UserInterfaceState;

import android.app.Notification;
import android.view.View;

import vyglab.argeo.R;
import vyglab.argeo.app.utils.MessagesManager;
import vyglab.argeo.app.view.FragmentPlane;

/**
 * Created by ngazcon on 27/12/2017.
 */

public class ListenerForUITransitionAcceptPlane extends ListenerForUITransition {

    public ListenerForUITransitionAcceptPlane(int context, int interaction) {
        super(context, interaction);
    }

    @Override
    public void onClick(View v){
        FragmentPlane fragment = (FragmentPlane) UIFacade.getInstance().getCurrentFragment("TAG_PLANE");

        if (fragment.screenTapped()) {
            UIContextManager.getInstance().next(m_context, m_interaction);
            UIContextManager.getInstance().request(m_context);
        } else {
            //MessagesManager.getInstance().showShortMessage("You need to tap the terrain to add a new Virtual Plane before finishing.");
            MessagesManager.getInstance().showLongMessage(R.string.ui_message_accept_plane_creation);
        }
    }
}
