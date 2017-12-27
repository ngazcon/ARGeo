package vyglab.argeo.app.controller.UserInterfaceState;

import android.view.View;

import vyglab.argeo.R;
import vyglab.argeo.app.MainActivityState;
import vyglab.argeo.app.MediatorArgeoFragment;
import vyglab.argeo.app.MediatorTouch;
import vyglab.argeo.app.model.Plane;
import vyglab.argeo.app.utils.Storage;
import vyglab.argeo.app.view.FragmentPlane;

/**
 * Created by ngazcon on 22/12/2017.
 */

public class UIStatePlaneEdition extends UIState {

    public UIStatePlaneEdition() {
        super();
    }

    @Override
    public void handle() {
        // 1-- Update Fab
        //TODO Refactoring pending: FAB updates should be done here.

        // 2-- Update Secondary Fabs
        UIFacade.getInstance().setSecondaryFabListeners(m_listeners.get(Interactions.SECONDARY_FAB_1), m_listeners.get(Interactions.SECONDARY_FAB_2));
        UIFacade.getInstance().setSecondaryFabImages(R.drawable.ic_menu_accept, R.drawable.ic_menu_cancel);
        UIFacade.getInstance().setSecondaryFabVisibilities(View.VISIBLE, View.VISIBLE);

        // 3-- Update Fragment
        FragmentPlane fragment = (FragmentPlane) UIFacade.getInstance().getCurrentFragment("TAG_PLANE");
        fragment.setForEdition();

        // 4-- Change Touch Mode
        MediatorTouch.getInstance().changeTouchMode(MainActivityState.TouchMode.PLANE_CREATION);
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
        Plane plane;

        switch(interaction) {
            case Interactions.SECONDARY_FAB_1 :
                // Edition was accepted
                fragment = (FragmentPlane) UIFacade.getInstance().getCurrentFragment("TAG_PLANE");
                plane = fragment.acceptEdition();
                //PlaneRepository.getInstance().addPlane(plane);
                Storage.getInstance().update(plane);
                //HandyPlane.getInstance().clear();
                MediatorTouch.getInstance().changeTouchMode(MainActivityState.TouchMode.DEFAULT);
                break;

            case Interactions.SECONDARY_FAB_2 :
                // Edition was canceled
                fragment = (FragmentPlane) UIFacade.getInstance().getCurrentFragment("TAG_PLANE");
                fragment.cancelEdition();
                MediatorTouch.getInstance().changeTouchMode(MainActivityState.TouchMode.DEFAULT);
                break;

            case Interactions.EXTRA_INTERACTION_3 :
                // Delete Plane
                fragment = (FragmentPlane) UIFacade.getInstance().getCurrentFragment("TAG_PLANE");
                plane = fragment.deleteCurrentPlane();
                Storage.getInstance().delete(plane);
                MediatorArgeoFragment.getInstance().cleanGraphics(plane);
                plane = null;
                break;
        }
    }
}
