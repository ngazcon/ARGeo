package vyglab.argeo.app.controller.UserInterfaceState;

import android.view.View;

import vyglab.argeo.R;
import vyglab.argeo.app.MediatorArgeoFragment;
import vyglab.argeo.app.model.Plane;
import vyglab.argeo.app.utils.Storage;
import vyglab.argeo.app.view.FragmentPlane;

/**
 * Created by ngazcon on 21/12/2017.
 */

public class UIStatePlaneSelected extends UIState {

    public UIStatePlaneSelected() {
        super();
    }

    @Override
    public void handle() {
        // 1-- Update Fab
        //TODO Refactoring pending: FAB updates should be done here.

        // 2-- Update Secondary Fabs
        UIFacade.getInstance().setSecondaryFabListeners(null, m_listeners.get(Interactions.SECONDARY_FAB_2));
        UIFacade.getInstance().setSecondaryFabImages(0, R.drawable.ic_menu_pencil);
        UIFacade.getInstance().setSecondaryFabVisibilities(View.GONE, View.VISIBLE);

        // 3-- Update Fragment
    }

    @Override
    public void nextState(UIContext context, int interaction) {
        exitAction(interaction);
        context.setState(m_transitions.get(interaction));
    }

    @Override
    public void enterAction(int interaction) {

    }

    @Override
    public void exitAction(int interaction) {
        FragmentPlane fragment;

        switch(interaction) {
            case Interactions.EXTRA_INTERACTION_3 :
                // Delete Plane
                fragment = (FragmentPlane) UIFacade.getInstance().getCurrentFragment("TAG_PLANE");
                Plane plane = fragment.deleteCurrentPlane();
                Storage.getInstance().delete(plane);
                MediatorArgeoFragment.getInstance().cleanGraphics(plane);
                plane = null;
                break;
        }
    }
}