package vyglab.argeo.app.controller;

import android.view.View;

import vyglab.argeo.app.MainActivity;
import vyglab.argeo.app.view.FragmentTTARView;

/**
 * Created by ngazcon on 08/03/2017.
 */

public class ListenerClosePictureInPicture implements View.OnClickListener {

    private MainActivity m_activity;
    private FragmentTTARView m_fragment_ttarview;

    public ListenerClosePictureInPicture(MainActivity activity, FragmentTTARView fragment_ttarview) {
        m_activity = activity;
        m_fragment_ttarview = fragment_ttarview;
    }

    @Override
    public void onClick(View view) {
        m_activity.closePictureInPicture();
    }
}
