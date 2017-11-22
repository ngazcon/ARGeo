package vyglab.argeo.app.controller.UserInterfaceState;

import android.view.View;

import vyglab.argeo.R;
import vyglab.argeo.app.MediatorTTARView;
import vyglab.argeo.app.model.TTARView;
import vyglab.argeo.app.utils.Storage;
import vyglab.argeo.app.view.FragmentTTARView;

/**
 * Created by ngazcon on 02/11/2017.
 */

public class UIStateTTARViewEdition extends UIState {

    public UIStateTTARViewEdition() {
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
        FragmentTTARView fragment = (FragmentTTARView) UIFacade.getInstance().getCurrentFragment("TAG_TTARVIEW");
        fragment.setForEdition();
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
        FragmentTTARView fragment;
        TTARView ttarview;

        switch(interaction) {
            case Interactions.SECONDARY_FAB_1 :
                // Edition was accepted
                fragment = (FragmentTTARView) UIFacade.getInstance().getCurrentFragment("TAG_TTARVIEW");
                ttarview = fragment.acceptEdition();
                Storage.getInstance().update(ttarview);
                break;

            case Interactions.SECONDARY_FAB_2 :
                // Edition was canceled
                fragment = (FragmentTTARView) UIFacade.getInstance().getCurrentFragment("TAG_TTARVIEW");
                fragment.cancelEdition();
                break;

            case Interactions.EXTRA_INTERACTION_3 :
                // Delete TTARView
                fragment = (FragmentTTARView) UIFacade.getInstance().getCurrentFragment("TAG_TTARVIEW");
                ttarview = fragment.deleteCurrentTTARView();
                MediatorTTARView.getInstance().closeTTARViewOnDeletion(ttarview);
                Storage.getInstance().delete(ttarview);
                //todo DELETE TTARVIEW
                break;
        }
    }
}
