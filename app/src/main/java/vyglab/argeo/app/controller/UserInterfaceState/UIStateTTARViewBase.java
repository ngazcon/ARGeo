package vyglab.argeo.app.controller.UserInterfaceState;

import android.view.View;
import vyglab.argeo.R;
import vyglab.argeo.app.view.FragmentTTARView;

/**
 * Created by ngazcon on 25/10/2017.
 */

public class UIStateTTARViewBase extends UIState {

    public UIStateTTARViewBase() {
        super();
    }

    @Override
    public void handle() {
        // 1-- Update Fab
        //TODO Refactoring pending: FAB updates should be done here.

        // 2-- Update Secondary Fabs
        UIFacade.getInstance().setSecondaryFabListeners(m_listeners.get(Interactions.SECONDARY_FAB_1), null);
        UIFacade.getInstance().setSecondaryFabImages(R.drawable.ic_menu_add_photo, 0);
        UIFacade.getInstance().setSecondaryFabVisibilities(View.VISIBLE, View.GONE);

        // 3-- Update Fragment
        // Something to do?
        //FragmentTTARView fragment = (FragmentTTARView) UIFacade.getInstance().getCurrentFragmente("TAG_TTARVIEW");
        //fragment.setForCreation(1024, 768);
    }

    @Override
    public void nextState(UIContext context, int interaction) {
        context.setState(m_transitions.get(interaction));
    }

    @Override
    public void enterAction(int interaction) {

    }

    @Override
    public void exitAction(int interaction) {

    }
}
