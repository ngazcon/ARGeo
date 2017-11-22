package vyglab.argeo.app.controller.UserInterfaceState;

import android.view.View;

import vyglab.argeo.R;
import vyglab.argeo.app.MediatorTTARView;
import vyglab.argeo.app.model.TTARView;
import vyglab.argeo.app.utils.Storage;
import vyglab.argeo.app.view.FragmentTTARView;

/**
 * Created by ngazcon on 27/10/2017.
 */

public class UIStateTTARViewSelected extends UIState {

    public UIStateTTARViewSelected() {
        super();
    }

    @Override
    public void handle() {
        // 1-- Update Fab
        //TODO Refactoring pending: FAB updates should be done here.

        // 2-- Update Secondary Fabs
        UIFacade.getInstance().setSecondaryFabListeners(m_listeners.get(Interactions.SECONDARY_FAB_1), m_listeners.get(Interactions.SECONDARY_FAB_2));
        UIFacade.getInstance().setSecondaryFabImages(R.drawable.ic_menu_photo_size_select_large, R.drawable.ic_menu_pencil);
        UIFacade.getInstance().setSecondaryFabVisibilities(View.VISIBLE, View.VISIBLE);

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
        FragmentTTARView fragment;

        switch(interaction) {
            case Interactions.SECONDARY_FAB_1 :
                // Creation was accepted
                fragment = (FragmentTTARView) UIFacade.getInstance().getCurrentFragment("TAG_TTARVIEW");
                fragment.prepareForPictureInPictureARView();
                break;

            case Interactions.EXTRA_INTERACTION_3 :
                // Delete TTARView
                fragment = (FragmentTTARView) UIFacade.getInstance().getCurrentFragment("TAG_TTARVIEW");
                TTARView ttarview = fragment.deleteCurrentTTARView();
                MediatorTTARView.getInstance().closeTTARViewOnDeletion(ttarview);
                Storage.getInstance().delete(ttarview);
                //todo DELETE TTARVIEW
                break;
        }
    }
}
