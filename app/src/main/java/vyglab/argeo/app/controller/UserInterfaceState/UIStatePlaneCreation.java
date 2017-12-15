package vyglab.argeo.app.controller.UserInterfaceState;

import android.view.View;

import vyglab.argeo.R;
import vyglab.argeo.app.MainActivityState;
import vyglab.argeo.app.MediatorTouch;
import vyglab.argeo.app.view.FragmentPlane;

/**
 * Created by ngazcon on 13/12/2017.
 */

public class UIStatePlaneCreation extends UIState {

    public UIStatePlaneCreation() {
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
        fragment.setForCreation();

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
        //FragmentTTARView fragment;

        switch(interaction) {
            case Interactions.SECONDARY_FAB_1 :
                // Creation was accepted
                //fragment = (FragmentTTARView) UIFacade.getInstance().getCurrentFragment("TAG_TTARVIEW");
                //TARView ttarview = fragment.acceptCreation();
                //TTARViewRepository.getInstance().addTTARView(ttarview);
                //Storage.getInstance().insert(ttarview);
                MediatorTouch.getInstance().changeTouchMode(MainActivityState.TouchMode.DEFAULT);
                break;

            case Interactions.SECONDARY_FAB_2 :
                // Creation was canceled
                //fragment = (FragmentTTARView) UIFacade.getInstance().getCurrentFragment("TAG_TTARVIEW");
                //fragment.cancelCreation();
                MediatorTouch.getInstance().changeTouchMode(MainActivityState.TouchMode.DEFAULT);
                break;
        }
    }
}