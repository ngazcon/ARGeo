package vyglab.argeo.app.controller;

import android.content.Context;
import android.support.design.widget.CoordinatorLayout;
import android.support.design.widget.FloatingActionButton;
import android.util.AttributeSet;
import android.view.View;
import android.widget.LinearLayout;

/**
 * Created by root on 22/11/16.
 */

public class SecondaryFabBehaviour2 extends CoordinatorLayout.Behavior<LinearLayout> {

    public SecondaryFabBehaviour2(Context context, AttributeSet attrs) {}

    @Override
    public boolean layoutDependsOn(CoordinatorLayout parent, LinearLayout child, View dependency) {
        return dependency instanceof FloatingActionButton;
    }

    @Override
    public boolean onDependentViewChanged(CoordinatorLayout parent, LinearLayout child, View dependency) {
        float translationX = (float) dependency.getWidth();
        child.setTranslationX(-translationX);
        return true;
    }
}
