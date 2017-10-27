package vyglab.argeo.app.controller.UserInterfaceState;

import android.support.design.widget.FloatingActionButton;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.view.View;

/**
 * Created by ngazcon on 25/10/2017.
 */

public class UIFacade {
    private static UIFacade m_instance = null;

    FloatingActionButton m_fab;
    FloatingActionButton m_secondary_fab_1;
    FloatingActionButton m_secondary_fab_2;
    FragmentManager m_fragment_manager;

    protected UIFacade(){
    }

    public static UIFacade getInstance() {
        if(m_instance == null) {
            m_instance = new UIFacade();
        }

        return m_instance;
    }

    public void init(FloatingActionButton fab, FloatingActionButton sec_fab_1, FloatingActionButton sec_fab_2, FragmentManager manager) {
        m_fab = fab;
        m_secondary_fab_1 = sec_fab_1;
        m_secondary_fab_2 = sec_fab_2;
        m_fragment_manager = manager;
    }

    public void setSecondaryFabListeners(View.OnClickListener listener_1, View.OnClickListener listener_2) {
        m_secondary_fab_1.setOnClickListener(listener_1);
        m_secondary_fab_2.setOnClickListener(listener_2);
    }

    public void setSecondaryFabVisibilities(int value_sf_1, int value_sf_2) {
        m_secondary_fab_1.setVisibility(value_sf_1);
        m_secondary_fab_2.setVisibility(value_sf_2);
    }

    public void setSecondaryFabImages(int res_sf_1, int res_sf_2) {
        m_secondary_fab_1.setImageResource(res_sf_1);
        m_secondary_fab_2.setImageResource(res_sf_2);
    }

    public Fragment getCurrentFragmente(String tag) {
        return m_fragment_manager.findFragmentByTag(tag);
    }

}
