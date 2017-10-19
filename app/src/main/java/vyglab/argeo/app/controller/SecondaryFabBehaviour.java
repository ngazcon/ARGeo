package vyglab.argeo.app.controller;

import android.content.Context;
import android.support.design.widget.CoordinatorLayout;
import android.support.design.widget.FloatingActionButton;
import android.util.AttributeSet;
import android.view.View;

public class SecondaryFabBehaviour extends CoordinatorLayout.Behavior<FloatingActionButton> {

    public SecondaryFabBehaviour(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    public boolean layoutDependsOn(CoordinatorLayout parent, FloatingActionButton child, View dependency) {
        return dependency instanceof FloatingActionButton;
    }

    @Override
    public boolean onDependentViewChanged(CoordinatorLayout parent, FloatingActionButton child, View dependency) {
        float translationX = (float) dependency.getWidth();
        child.setTranslationX(-translationX);
        return false;
    }
}
