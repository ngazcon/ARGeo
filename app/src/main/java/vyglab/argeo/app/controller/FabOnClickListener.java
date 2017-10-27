package vyglab.argeo.app.controller;

import android.support.design.widget.FloatingActionButton;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.view.animation.AnimationUtils;
import android.view.animation.Interpolator;
import android.widget.LinearLayout;

import vyglab.argeo.app.MainActivityState;
import vyglab.argeo.R;
import vyglab.argeo.app.view.FragmentPlane;
import vyglab.argeo.app.view.FragmentTTARView;

/**
 * Created by ngazcon on 14/09/2016.
 */
public class FabOnClickListener
        implements View.OnClickListener, MainActivityState.AppModeStateChangedListener {

    private FloatingActionButton m_fab;
    private AppCompatActivity m_activity;
    private LinearLayout m_current_right_menu;
    private MainActivityState.AppMode m_current_mode;
    private FragmentTTARView m_fragment_ttarview;
    private FragmentPlane m_fragment_plane;
    private int  m_current_icon;
    private Interpolator m_interpolator;
    private float m_density;
    private int m_right_menu_width;
    private int m_fab_with;
    private int m_fab_margin;

    private boolean m_fab_open;

    public FabOnClickListener(FloatingActionButton fab, AppCompatActivity activity,
                              int right_menu_width,
                              int fab_margin,
                              int fab_with) {
        m_fab = fab;
        m_activity = activity;
        m_density = fab.getContext().getResources().getDisplayMetrics().density;
        m_right_menu_width = right_menu_width;
        m_fab_margin = fab_margin;
        m_fab_with = fab_with;

        m_fab_open = false;
        m_current_right_menu = (LinearLayout) m_activity.findViewById(R.id.terrain_linearLayout);
        m_current_icon = R.drawable.ic_menu_terrain;


        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.LOLLIPOP) {
            m_interpolator = AnimationUtils.loadInterpolator(m_activity.getBaseContext(), android.R.interpolator.fast_out_slow_in);
        }

        setMenusVisibilitiesFalse();
    }

    public void onClick(View view){
        LinearLayout right_menu;

        if (!m_fab_open) {
            m_fab_open = true;
            if (m_current_right_menu == null){
                if (m_current_mode == MainActivityState.AppMode.PLANE) {
                    m_fragment_plane.showFragment();
                } else if (m_current_mode == MainActivityState.AppMode.TTARVIEW){
                    m_fragment_ttarview.showFragment();
                }
            } else {
                m_current_right_menu.setVisibility(m_current_right_menu.VISIBLE);
            }
            right_menu = (LinearLayout) m_activity.findViewById(R.id.right_menu);
            right_menu.setVisibility(right_menu.VISIBLE);

            // Animation
            if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.LOLLIPOP) {
                float translation = -m_right_menu_width + m_fab_with/2.0f + m_fab_margin;
                animateOpen(view, translation, 0.5f, 360.0f);
            }
            m_fab.setImageResource(R.drawable.ic_menu_chevron_double_right);
        } else{// close fab
            m_fab_open = false;
            setMenusVisibilitiesFalse();
            //TODO ARREGLAR DISENIO PARA EL OGT
            if (m_fragment_plane != null) {
                m_fragment_plane.hideFragment();
            }
            if (m_fragment_ttarview != null) {
                m_fragment_ttarview.hideFragment();
            }

            // Animation
            if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.LOLLIPOP) {
                animateClose(view, 0.0f, 1.0f, 0.0f);
            }

            // Set the proper icon to the fab
            m_fab.setImageResource(m_current_icon);
        }
    }

    @Override
    public void onAppModeStateChanged(MainActivityState.AppMode mode) {
        if (mode == MainActivityState.AppMode.TERRAIN) {
            m_current_right_menu = (LinearLayout) m_activity.findViewById(R.id.terrain_linearLayout);
        } else if (mode == MainActivityState.AppMode.POI) {
            m_current_right_menu = (LinearLayout) m_activity.findViewById(R.id.poi_linearLayout);
        } else if (mode == MainActivityState.AppMode.PLANE) {
            //m_current_right_menu = (LinearLayout) m_activity.findViewById(R.id.plane_linearLayout);
            m_current_right_menu = null; // para el caso fragmentacion maxima
            m_fragment_plane = (FragmentPlane) m_activity.getSupportFragmentManager().findFragmentByTag("TAG_PLANE");
            m_current_mode = mode;
        } else if (mode == MainActivityState.AppMode.TTARVIEW) {
            //TODO poner lo que corresponde de TTARVIEW layouts
            m_current_right_menu = null; // para el caso fragmentacion maxima
            m_fragment_ttarview = (FragmentTTARView) m_activity.getSupportFragmentManager().findFragmentByTag("TAG_TTARVIEW");
            m_current_mode = mode;
        }

        // It is safe to assume that the application mode changes to a new one
        setIcon(mode);
        if (m_fab_open){
            // Close the menu and update
            this.onClick(m_fab);
        }
        // Solución de compromiso para que no rebiente cdo. de una intento crear un nuevo POI sin haber abierto el tab por primera vez
        requestOpen();
    }

    @Override
    public void onApplicationModeChanged(int mode) {
        m_current_right_menu = null; // para el caso fragmentacion maxima
        m_fragment_ttarview = (FragmentTTARView) m_activity.getSupportFragmentManager().findFragmentByTag("TAG_TTARVIEW");
        m_current_mode = MainActivityState.AppMode.TTARVIEW;
        setIcon(m_current_mode);
        if (m_fab_open){
            // Close the menu and update
            this.onClick(m_fab);
        }
        // Solución de compromiso para que no rebiente cdo. de una intento crear un nuevo POI sin haber abierto el tab por primera vez
        requestOpen();
    }

    private void setIcon(MainActivityState.AppMode mode){
        switch (mode){
            case TERRAIN:
                m_current_icon = R.drawable.ic_menu_terrain;
                break;

            case POI:
                m_current_icon = R.drawable.ic_menu_pois;
                break;

            case PLANE:
                m_current_icon = R.drawable.ic_menu_plane;
                break;

            case TTARVIEW:
                m_current_icon = R.drawable.ic_menu_burstmode;
        }

        m_fab.setImageResource(m_current_icon);
    }

    private void animateOpen(View view, float translation, float scale, float rotation){
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.LOLLIPOP) {
            view.animate()
                    .rotation(rotation)
                    .translationX(translation * m_density)
                    .scaleY(scale)
                    .scaleX(scale)
                    .setInterpolator(m_interpolator)
                    .start();
        }
        //Animation fab_open = AnimationUtils.loadAnimation(m_activity.getApplicationContext(), R.anim.fab_open);
        //view.startAnimation(fab_open);
    }

    private void animateClose(View view, float translation, float scale, float rotation){
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.LOLLIPOP) {
            view.animate()
                    .translationX(translation)
                    .scaleY(scale)
                    .scaleX(scale)
                    .rotation(rotation)
                    .setInterpolator(m_interpolator)
                    .start();
        }
    }

    private void setMenusVisibilitiesFalse(){
        LinearLayout layout = (LinearLayout) m_activity.findViewById(R.id.right_menu);
        layout.setVisibility(layout.GONE);
        layout = (LinearLayout) m_activity.findViewById(R.id.terrain_linearLayout);
        layout.setVisibility(layout.GONE);
        layout = (LinearLayout) m_activity.findViewById(R.id.poi_linearLayout);
        layout.setVisibility(layout.GONE);
        //layout = (LinearLayout) m_activity.findViewById(R.id.plane_linearLayout);
        //layout.setVisibility(layout.GONE);
    }

    public void updatePositioning(int right_menu_width){
        m_right_menu_width = right_menu_width;

        if (m_fab_open) {
            if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.LOLLIPOP) {
                float translation = -m_right_menu_width + m_fab_with / 2.0f + m_fab_margin;
                animateOpen(m_fab, translation, 0.5f, 360.0f);
            }
        }
    }

    public void requestOpen(){
        if (!m_fab_open) {
            this.onClick(m_fab);
        }
    }
}

