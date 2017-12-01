package vyglab.argeo.app;

import android.Manifest;
import android.app.ActivityManager;
import android.content.Context;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.os.PowerManager;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.TabLayout;
import android.support.v4.app.ActivityCompat;
import android.support.v4.app.FragmentTransaction;
import android.support.v4.content.res.ResourcesCompat;
import android.support.v4.view.ViewPager;
import android.support.v7.widget.SwitchCompat;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.view.WindowManager;
import android.view.Window;
import android.widget.CompoundButton;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.Toast;

import java.util.ArrayList;

import vyglab.argeo.R;
import vyglab.argeo.app.controller.UserInterfaceState.ListenerForUITransition;
import vyglab.argeo.app.controller.ListenersTTARView.ListenerTTARViewSketch;
import vyglab.argeo.app.controller.ListenersTTARView.ListenerTTARViewTouchSketch;
import vyglab.argeo.app.controller.ListenersTTARView.ListenerTTARViewTouchNormal;
import vyglab.argeo.app.controller.UserInterfaceState.UIContext;
import vyglab.argeo.app.controller.UserInterfaceState.UIContextManager;
import vyglab.argeo.app.controller.UserInterfaceState.UIFacade;
import vyglab.argeo.app.controller.UserInterfaceState.UIState;
import vyglab.argeo.app.controller.UserInterfaceState.UIStateTTARViewBase;
import vyglab.argeo.app.controller.UserInterfaceState.UIStateTTARViewCreation;
import vyglab.argeo.app.controller.UserInterfaceState.UIStateTTARViewEdition;
import vyglab.argeo.app.controller.UserInterfaceState.UIStateTTARViewSelected;
import vyglab.argeo.app.model.POI;
import vyglab.argeo.app.model.POIRepository;
import vyglab.argeo.app.model.SketchFilter;
import vyglab.argeo.app.model.TTARViewRepository;
import vyglab.argeo.app.utils.Storage;
import vyglab.argeo.app.view.DialogChangeTTARVIEWResolution;
import vyglab.argeo.app.view.FragmentTerrain;
import vyglab.argeo.jni.ArgeoFragment;
import vyglab.argeo.jni.Camera;
import vyglab.argeo.jni.EllipsoidTransformations;
import vyglab.argeo.jni.Engine;
import vyglab.argeo.jni.Geocentric3D;
import vyglab.argeo.jni.RenderLoopRunnable;
import vyglab.argeo.jni.SnapshotListener;
import vyglab.argeo.app.controller.FabOnClickListener;
import vyglab.argeo.app.controller.InitializeEngine;
import vyglab.argeo.app.controller.ListenerClosePictureInPicture;
import vyglab.argeo.app.controller.ListenerScreenShot;
import vyglab.argeo.app.controller.ListenersPlane.ListenerAcceptNewPlane;
import vyglab.argeo.app.controller.ListenersPlane.ListenerCancelNewPlane;
import vyglab.argeo.app.controller.ListenersPlane.ListenerNewPlane;
import vyglab.argeo.app.controller.ListenersPoi.ListenerAcceptNewPoi;
import vyglab.argeo.app.controller.ListenersPoi.ListenerCancelNewPoi;
import vyglab.argeo.app.controller.ListenersPoi.ListenerNewPoi;
import vyglab.argeo.app.controller.ListenersTTARView.ListenerNewTTARView;
import vyglab.argeo.app.controller.ListenersTTARView.ListenerOpenPictureInPictureTTARView;
import vyglab.argeo.app.controller.ListenersTerrain.ListenerAcceptRotateCamera;
import vyglab.argeo.app.controller.ListenersTerrain.ListenerAcceptTranslateCamera;
import vyglab.argeo.app.controller.ListenersTerrain.ListenerCancelRotateCamera;
import vyglab.argeo.app.controller.ListenersTerrain.ListenerCancelTranslateCamera;
import vyglab.argeo.app.controller.ListenersTerrain.ListenerRotateCamera;
import vyglab.argeo.app.controller.ListenersTerrain.ListenerTranslateCamera;
import vyglab.argeo.app.controller.PositionPoiChanged;
import vyglab.argeo.app.controller.SecondaryFabStateMachine.SecondaryFabContext;
import vyglab.argeo.app.controller.SecondaryFabStateMachine.StateAcceptCancelNewPlane;
import vyglab.argeo.app.controller.SecondaryFabStateMachine.StateAcceptCancelNewPoi;
import vyglab.argeo.app.controller.SecondaryFabStateMachine.StateAcceptCancelRotateCamera;
import vyglab.argeo.app.controller.SecondaryFabStateMachine.StateAcceptCancelTranslateCamera;
import vyglab.argeo.app.controller.SecondaryFabStateMachine.StatePlane;
import vyglab.argeo.app.controller.SecondaryFabStateMachine.StatePoiEdit;
import vyglab.argeo.app.controller.SecondaryFabStateMachine.StatePoiNew;
import vyglab.argeo.app.controller.SecondaryFabStateMachine.StateTerrain;
import vyglab.argeo.app.controller.TabSectionsPagerAdapter;
import vyglab.argeo.app.model.TTARView;
import vyglab.argeo.app.model.database.DBManager;
import vyglab.argeo.app.utils.HandyPlane;
import vyglab.argeo.app.utils.StorageManager;
import vyglab.argeo.app.view.FragmentPlane;
import vyglab.argeo.app.view.FragmentTTARView;
import vyglab.argeo.app.view.TabFragmentPOI;
import vyglab.argeo.app.utils.HandyPOI;
import vyglab.argeo.app.controller.TouchModeStateChangedListener;
import vyglab.argeo.app.view.DialogChangeTTARVIEWResolution.DialogChangeTTARVIEWResolutionListener;

public class MainActivity extends AppCompatActivity
        implements NavigationView.OnNavigationItemSelectedListener,
        View.OnTouchListener,
        TouchModeStateChangedListener,
        FragmentTerrain.OnWireframeToggleChangedListener,
        SnapshotListener,
        PositionPoiChanged,
        HandyPlane.PlaneChanged,
        DialogChangeTTARVIEWResolutionListener {


    private ViewPager m_ViewPager;
    private DrawerLayout m_drawer;

    private MainActivityState m_activity_state;
    private FabOnClickListener m_fab_listener;

    private SecondaryFabContext m_secondary_fab_context;

    private TTARView m_current_ttarview;

    //private POIRepository m_poi_repo;
    private DBManager m_DBmanager;
    private StorageManager m_external_storage;

    //private SharedPreferences m_preferences;
    private PreferencesManager m_preferences;

    private ArgeoFragment mArgeoFragment;
    private FragmentPlane m_fragment_plane;
    private FragmentTTARView m_fragment_ttarview;
    private ListenerTTARViewTouchNormal m_ttarview_touch_listener_normal;
    private ListenerTTARViewTouchSketch m_ttarview_touch_listener_sketch;
    private SketchFilter m_sketch_filter;
    private float m_screen_density;
    private int m_screen_w;
    private int m_screen_h;
    private float m_screen_aspect_ratio;
    private int m_ttarview_snapshot_w;
    private int m_ttarview_snapshot_h;
    private int m_ttarview_picture_in_picture_w;
    private int m_ttarview_picture_in_picture_h;
    private int m_ttarview_picture_in_picture_w_MIN;
    private int m_ttarview_picture_in_picture_h_MIN;
    private int m_ttarview_picture_in_picture_w_MAX;
    private int m_ttarview_picture_in_picture_h_MAX;

    private PowerManager.WakeLock mWakeLock;

    private DialogChangeTTARVIEWResolution m_dialog_ttarview_resolution;

    //Volarlo cdo lo arregle
    private float mLastTouchX;
    private float mLastTouchY;
    private int mActivePointerId;
    private InitializeEngine m_engine_initializer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        m_external_storage = StorageManager.getInstance(this.getApplicationContext());

        Engine.init(this);

        setContentView(R.layout.activity_main);

        supportRequestWindowFeature(Window.FEATURE_ACTION_BAR); // Esta linea la agrego para luego llamar a getSupportActionBar().hide(); y esconder la actionBar
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        m_drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
                this, m_drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        m_drawer.setDrawerListener(toggle);
        m_drawer.setOnTouchListener(this);
        toggle.syncState();

        NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        navigationView.setNavigationItemSelectedListener(this);

        // Escondo la ActionBar y pongo en modo fullscreen
        getSupportActionBar().hide();
        hideStatusAndNavigationBars();

        m_activity_state = new MainActivityState();
        m_activity_state.addTouchModeListener(this);

        mArgeoFragment = (ArgeoFragment) getSupportFragmentManager().findFragmentById(R.id.argeo_fragment);
        HandyPOI.getInstance().setup(mArgeoFragment);
        HandyPlane.getInstance().setup(mArgeoFragment);

        m_DBmanager = new DBManager(getApplicationContext(), mArgeoFragment);
        Toast.makeText(getApplicationContext(), m_DBmanager.listPois().toString(), Toast.LENGTH_SHORT).show();
        Storage.getInstance().init(m_DBmanager);

        ActivityCompat.requestPermissions(
                this,
                new String[]{
                        Manifest.permission.INTERNET,
                        Manifest.permission.ACCESS_FINE_LOCATION,
                        Manifest.permission.ACCESS_COARSE_LOCATION,
                        Manifest.permission.CAMERA,
                        Manifest.permission.WAKE_LOCK,
                        Manifest.permission.WRITE_EXTERNAL_STORAGE,
                        Manifest.permission.READ_EXTERNAL_STORAGE,
                },
                0);

        // SKETCH STUFF
        // Lo tengo que hacer antes de que se cree el FragmentTTARView porque uso los valores de pantalla calculados
        m_sketch_filter = new SketchFilter();
        m_ttarview_touch_listener_normal = new ListenerTTARViewTouchNormal(this);
        m_ttarview_touch_listener_sketch = new ListenerTTARViewTouchSketch(this, m_fragment_ttarview, m_sketch_filter);
        ImageView picture = (ImageView) findViewById(R.id.imageview_ttarview_picture_in_picture);
        picture.setOnTouchListener(m_ttarview_touch_listener_normal);
        //picture.setOnTouchListener(m_ttarview_touch_listener_sketch);
        m_dialog_ttarview_resolution = null;

        DisplayMetrics displayMetrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
        // Watch out! landscap
        // When float is converted to int is truncated, so I add 0.5f for round error
        // (Android blog https://developer.android.com/guide/practices/screens_support.html)
        m_screen_density = displayMetrics.density;
        m_screen_w = displayMetrics.widthPixels;
        m_screen_h = displayMetrics.heightPixels;
        m_screen_aspect_ratio = (float) m_screen_w / (float) m_screen_h;
        m_ttarview_snapshot_w = 1024;
        m_ttarview_snapshot_h = (int) ((float) m_ttarview_snapshot_w / m_screen_aspect_ratio + 0.5f);
        m_ttarview_picture_in_picture_w = (int) ((float) m_screen_w * 0.3f + 0.5f); // 30% of screen
        m_ttarview_picture_in_picture_h = (int) ((float) m_ttarview_picture_in_picture_w / m_screen_aspect_ratio + 0.5f);
        m_ttarview_picture_in_picture_w_MIN = m_ttarview_picture_in_picture_w;
        m_ttarview_picture_in_picture_h_MIN = m_ttarview_picture_in_picture_h;
        m_ttarview_picture_in_picture_w_MAX = (int) ((float) m_screen_w * 0.8f + 0.5f);
        m_ttarview_picture_in_picture_h_MAX = (int) ((float) m_screen_h * 0.8f + 0.5f);
        MediatorTTARView.getInstance().init(this);

        // Set viewpager adapter
        m_ViewPager = (ViewPager) findViewById(R.id.pager);
        setupViewPager(m_ViewPager);
        setUpFabs();

        // Set tabs
        TabLayout tabs = (TabLayout) findViewById(R.id.tabs);
        tabs.setupWithViewPager(m_ViewPager);

        // Con esto le doy por c*** al navigation bar cdo. aparece en un EditText
        getWindow().getDecorView().setOnSystemUiVisibilityChangeListener
                (new View.OnSystemUiVisibilityChangeListener() {
                    @Override
                    public void onSystemUiVisibilityChange(int visibility) {
                        hideStatusAndNavigationBars();
                    }
                });

        // Loading preferences
        //m_preferences = getSharedPreferences("ArgeoPreferences", Context.MODE_PRIVATE);

        PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        mWakeLock       = pm.newWakeLock(PowerManager.SCREEN_BRIGHT_WAKE_LOCK, "WakeLock");

        //TODO Get rid of this
        m_engine_initializer = new InitializeEngine(m_DBmanager, mArgeoFragment, this);
        mArgeoFragment.getViewer().addStateListener(m_engine_initializer);

        FloatingActionButton fab_screenshot = (FloatingActionButton) findViewById(R.id.fab_screenshot);
        fab_screenshot.setOnClickListener(new ListenerScreenShot(this, mArgeoFragment));

        // OpenCV init
        /*if (!OpenCVLoader.initDebug()) {
            Log.e(this.getClass().getSimpleName(), "  OpenCVLoader.initDebug(), not working.");
        } else {
            Log.d(this.getClass().getSimpleName(), "  OpenCVLoader.initDebug(), working.");
        }*/
    }

    @Override
    protected void onStart() {
        super.onStart();
        //TODO Cdo esten los callbacks del fragment ubicar alli estas lineas comentadas
        //FragmentTerrain fragment_terrain = (FragmentTerrain) getSupportFragmentManager().findFragmentById(R.id.right_menu_fragment_terrain);
        //fragment_terrain.setOpacity(mArgeoFragment.getViewer().getScene().getTerrainOpacity());
        //fragment_terrain.setExaggeration(mArgeoFragment.getViewer().getScene().getTerrain().getHeightExaggeration());


        // Seteo por defecto mode "TERRAIN"
        NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        onNavigationItemSelected(navigationView.getMenu().getItem(0));
        navigationView.getMenu().getItem(0).setChecked(true);

        // Set Properties that it is needed a "living view"
        m_fragment_ttarview.setTTARViewWidthAndHeight(MediatorTTARView.getInstance().getTTARViewWidth(), MediatorTTARView.getInstance().getTTARViewHeight());

        // Load Preferences
        m_preferences = new PreferencesManager(this);
        // -- Preferences for Fragment Terrain
        FragmentTerrain fragment_terrain = (FragmentTerrain) getSupportFragmentManager().findFragmentById(R.id.right_menu_fragment_terrain);
        int int_value = m_preferences.getInt(R.string.pref_hud_width_key, R.integer.pref_hud_width_default);
        fragment_terrain.setHudWidth(int_value);

        int_value = m_preferences.getInt(R.string.pref_hud_opacity_key, R.integer.pref_hud_opacity_default);
        fragment_terrain.setHudOpacity(int_value);

        // -- Preferences TTARView Picture in Picture window
        FrameLayout frame_layout = (FrameLayout) findViewById(R.id.framelayout_ttarview_picture_in_picture);
        int_value = m_preferences.getIntProvidingDefault(R.string.pref_ttarview_pip_width_key, m_ttarview_picture_in_picture_w);
        frame_layout.getLayoutParams().width = int_value;
        int_value = m_preferences.getIntProvidingDefault(R.string.pref_ttarview_pip_height_key, m_ttarview_picture_in_picture_h);
        frame_layout.getLayoutParams().height = int_value;
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        //this.mArgeoFragment.onTouch(v, event);

        // TESTING STUFF
        //DBManager manager = new DBManager(this.getBaseContext());
        /*for (POI poi : POIRepository.getInstance().getPOIs() ) {
            manager.insertPoi(poi);
        }*/
        //ArrayList<String> list = manager.listPois();

        switch ( m_activity_state.getTouchMode() ){
            case DEFAULT :
                return super.onTouchEvent(event);

            case POI_CREATION :
                if ( event.getAction() != MotionEvent.ACTION_UP )
                    return true;

                final int windowX = (int) event.getX();
                final int windowY = (int) event.getY();

                this.mArgeoFragment.getViewer().executeOnRenderLoop(new RenderLoopRunnable() {
                    @Override
                    public void run()
                    {
                        Geocentric3D geocentric = mArgeoFragment.getViewer().pickPosition(
                                windowX,
                                windowY);
                        HandyPOI.getInstance().updateLocation(geocentric);
                    }
                });
                break;

            case PLANE_CREATION :
                if ( event.getAction() != MotionEvent.ACTION_UP )
                    return true;

                final int windowX_plane = (int) event.getX();
                final int windowY_plane = (int) event.getY();

                this.mArgeoFragment.getViewer().executeOnRenderLoop(new RenderLoopRunnable() {
                    @Override
                    public void run()
                    {
                        Geocentric3D geocentric = mArgeoFragment.getViewer().pickPosition(
                                windowX_plane,
                                windowY_plane);
                        //HandyPOI.getInstance().updateLocationPlane(geocentric);
                        HandyPlane.getInstance().updatePlaneLocation(geocentric);
                    }
                });
                break;

            case CAMERA_TRANSLATION :
                handlePanning(event);
                break;

            case CAMERA_ROTATION :
                handleRotation(event);
                break;
        }

        return true;
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus){
            hideStatusAndNavigationBars();
        }
    }

    @Override
    protected  void onPause() {
        if(mWakeLock.isHeld()) {
            mWakeLock.release();
        }
        super.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        if(!mWakeLock.isHeld()) {
            mWakeLock.acquire();
        }
        hideStatusAndNavigationBars();

        //TODO Ver si esto lo puedo poner mas adentro en el life cycle
        NavigationView n = (NavigationView) findViewById(R.id.nav_view);
        LinearLayout l = (LinearLayout) n.getMenu().findItem(R.id.nav_screenshot_button).getActionView();
        SwitchCompat screenshot_switch = (SwitchCompat) l.getChildAt(0);

        screenshot_switch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                FloatingActionButton fab_screenshot = (FloatingActionButton) findViewById(R.id.fab_screenshot);

                if (b){
                    fab_screenshot.setVisibility(View.VISIBLE);
                } else {
                    fab_screenshot.setVisibility(View.GONE);
                }
            }
        });
    }

    @Override
    public void onBackPressed() {
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        if (drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START);
        } else {
            super.onBackPressed();
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);

        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @SuppressWarnings("StatementWithEmptyBody")
    @Override
    public boolean onNavigationItemSelected(MenuItem item) {
        // Handle navigation view item clicks here.
        int id = item.getItemId();
        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);

        //TODO prueba... sacar las siguientes 3 lineas cdo se haga bien
        TabSectionsPagerAdapter a = (TabSectionsPagerAdapter) m_ViewPager.getAdapter();
        POIListFragment p = (POIListFragment) a.getItem(0);
        p.resetSelected();

        if (id == R.id.nav_terrain_view) {
            // First check if right menu must be closed because it was open in other mode
            //if (m_activity_state.getFabModeOpen() && (m_activity_state.getAppMode() != MainActivityState.AppMode.TERRAIN)){
            //    m_fab_listener.onClick(fab);
            //}
            m_activity_state.setAppMode(MainActivityState.AppMode.TERRAIN);
            //fab.setImageResource(R.drawable.ic_menu_terrain);
        } else if (id == R.id.nav_pois) {
            //if (m_activity_state.getFabModeOpen()){
            //    m_fab_listener.onClick(fab);
            //}
            m_activity_state.setAppMode(MainActivityState.AppMode.POI);
            //fab.setImageResource(R.drawable.ic_menu_pois);
        } else if (id == R.id.nav_plane) {
            //if (m_activity_state.getFabModeOpen()){
            //    m_fab_listener.onClick(fab);
            //}
            m_activity_state.setAppMode(MainActivityState.AppMode.PLANE);
            //fab.setImageResource(R.drawable.ic_menu_plane);
        } else if (id == R.id.nav_ttarview) {
            m_activity_state.setAppMode(MainActivityState.AppMode.TTARVIEW);// aca esto hace algo para aparecer el panel derecho que no lo encuentro
            m_activity_state.setApplicationMode(MainActivityState.ApplicationMode.TTARVIEW);
        } else if (id == R.id.nav_settings) {

        } else if (id == R.id.nav_screenshot_button) {
            SwitchCompat screenshot_switch = (SwitchCompat) findViewById(R.id.screenshot_switch);
            screenshot_switch.toggle();
        } else if (id == R.id.nav_ttarview_resolution) {
            if (m_dialog_ttarview_resolution == null) {
                m_dialog_ttarview_resolution = new DialogChangeTTARVIEWResolution();
            }

            m_dialog_ttarview_resolution.setWAndRatio(MediatorTTARView.getInstance().getTTARViewWidth(), MediatorTTARView.getInstance().getTTARViewAspectRatio());
            m_dialog_ttarview_resolution.show(getSupportFragmentManager(), "TTARVIEW_DIALOG_RESOLUTION");
        } else if (id == R.id.nav_db_load) {
            ArrayList<POI> poi_list = m_DBmanager.obtainBillboardList();
            for (POI poi : poi_list) {
                POIRepository.getInstance().addPOI(poi);
                mArgeoFragment.getViewer().getEntities().add(poi.getGraphic());
            }

            ArrayList<TTARView> ttarview_list = m_DBmanager.obtainTTARViewList();
            for (TTARView ttarview : ttarview_list) {
                TTARViewRepository.getInstance().addTTARView(ttarview);
            }

            Toast.makeText(getApplicationContext(), "Database loaded!", Toast.LENGTH_SHORT).show();
        } else if (id == R.id.nav_db_clear) {
            m_DBmanager.clearDB();
            Toast.makeText(getApplicationContext(), "Database cleared!", Toast.LENGTH_SHORT).show();
        }

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);
        return true;
    }

    private void hideStatusAndNavigationBars(){
        // Full screen window
        getWindow().setFlags(
                WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS,
                WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS);

        // Hide nav and status bars, only works on Android 4.4+
        getWindow().getDecorView().setSystemUiVisibility(
                        View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION // hide nav bar
                        | View.SYSTEM_UI_FLAG_FULLSCREEN // hide status bar
                        | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
    }

    private void setUpFabs(){
        // Settings for principal FAB
        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        int right_menu_width = (int) (getResources().getDimension(R.dimen.right_menu_default_width) / getResources().getDisplayMetrics().density);
        int fab_margin = (int) (getResources().getDimension(R.dimen.fab_margin) / getResources().getDisplayMetrics().density);
        int fab_width = (int) (getResources().getDimension(R.dimen.fab_width) / getResources().getDisplayMetrics().density);
        m_fab_listener = new FabOnClickListener(fab, this, right_menu_width, fab_margin, fab_width);
        fab.setOnClickListener(m_fab_listener);
        m_activity_state.addAppModeListener(m_fab_listener);

        // Create Secondary fab context
        m_secondary_fab_context = new SecondaryFabContext();
        FloatingActionButton fab_1 = (FloatingActionButton) findViewById(R.id.fab_1);
        FloatingActionButton fab_2 = (FloatingActionButton) findViewById(R.id.fab_2);

        // Create Listeners
        ListenerTranslateCamera listener_translate_camera = new ListenerTranslateCamera(this, m_activity_state, mArgeoFragment);
        ListenerAcceptTranslateCamera listener_accept_translate_camera = new ListenerAcceptTranslateCamera(this, m_activity_state, mArgeoFragment);
        ListenerCancelTranslateCamera listener_cancel_translate_camera = new ListenerCancelTranslateCamera(this, m_activity_state, mArgeoFragment);

        ListenerRotateCamera listener_rotate_camera = new ListenerRotateCamera(this, m_activity_state, mArgeoFragment);
        ListenerAcceptRotateCamera listener_accept_rotate_camera = new ListenerAcceptRotateCamera(this, m_activity_state, mArgeoFragment);
        ListenerCancelRotateCamera listener_cancel_rotate_camera = new ListenerCancelRotateCamera(this, m_activity_state, mArgeoFragment);

        ListenerNewPoi listener_newpoi = new ListenerNewPoi(this, m_activity_state, m_fab_listener);
        ListenerAcceptNewPoi listener_accept_newpoi = new ListenerAcceptNewPoi(this, m_activity_state, mArgeoFragment, m_DBmanager);
        ListenerCancelNewPoi listener_cancel_newpoi = new ListenerCancelNewPoi(this, m_activity_state);

        ListenerNewPlane listener_newplane = new ListenerNewPlane(this, m_activity_state, m_fragment_plane, m_fab_listener);
        ListenerAcceptNewPlane listener_accept_newplane = new ListenerAcceptNewPlane(this, m_activity_state, mArgeoFragment, m_DBmanager);
        ListenerCancelNewPlane listener_cancel_newplane = new ListenerCancelNewPlane(this, m_activity_state);

        ListenerNewTTARView listener_newttarview = new ListenerNewTTARView(m_fab_listener);
        //ListenerAcceptNewTTARView listener_accept_newttarview = new ListenerAcceptNewTTARView();
        //ListenerCancelNewTTARView listener_cancel_newttarview = new ListenerCancelNewTTARView();
        ListenerOpenPictureInPictureTTARView listener_open_main_ttarview = new ListenerOpenPictureInPictureTTARView(this, m_activity_state, m_fragment_ttarview, m_fab_listener);
        ListenerTTARViewSketch listener_ttarview_sketch = new ListenerTTARViewSketch(this, m_activity_state, m_fragment_ttarview, m_fab_listener);

        // Create States
        StateTerrain state_terrain = new StateTerrain(fab_1, fab_2, listener_translate_camera, listener_rotate_camera);
        StateAcceptCancelTranslateCamera state_acceptcancel_transalte = new StateAcceptCancelTranslateCamera(fab_1, fab_2, listener_accept_translate_camera, listener_cancel_translate_camera);
        StateAcceptCancelRotateCamera state_acceptcancel_rotate = new StateAcceptCancelRotateCamera(fab_1, fab_2, listener_accept_rotate_camera, listener_cancel_rotate_camera);
        StateAcceptCancelNewPoi state_acceptcancel_newpoi = new StateAcceptCancelNewPoi(fab_1, fab_2, listener_accept_newpoi, listener_cancel_newpoi);
        StateAcceptCancelNewPlane state_acceptcancel_newplane = new StateAcceptCancelNewPlane(fab_1, fab_2, listener_accept_newplane, listener_cancel_newplane);
        //StateAcceptCancelNewTTARView state_acceptcancel_newttarview = new StateAcceptCancelNewTTARView(fab_1, fab_2, listener_accept_newttarview, listener_cancel_newttarview);
        StatePoiNew state_poinew = new StatePoiNew(fab_1, fab_2, listener_newpoi, null);
        StatePoiEdit state_poiedit = new StatePoiEdit(fab_1, fab_2, null, null);
        StatePlane state_plane = new StatePlane(fab_1, fab_2, listener_newplane, null);
        //StateTTARViewBase state_ttarview = new StateTTARViewBase(fab_1, fab_2, listener_newttarview, null);
        //m_secondary_fab_context.registerState("TTARVIEW_BASE", state_ttarview);
        //StateTTARViewSelection state_ttarview_selection = new StateTTARViewSelection(fab_1, fab_2);
        //m_secondary_fab_context.registerState("TTARVIEW_SELECTION", state_ttarview_selection);
        //StateTTARViewPictureInPicture state_ttarview_picture_in_picture = new StateTTARViewPictureInPicture(fab_1, fab_2);
        //m_secondary_fab_context.registerState("TTARVIEW_PICTURE_IN_PICTURE", state_ttarview_picture_in_picture);
        //StateTTARViewSketch state_ttarview_sketch = new StateTTARViewSketch(fab_1, fab_2);
        //m_secondary_fab_context.registerState("TTARVIEW_SKETCH", state_ttarview_sketch);

        // Set State transitions
        // -- Transitions - Terrain
        state_terrain.setTransitionFrist(state_acceptcancel_transalte);
        state_terrain.setTransitionSecond(state_acceptcancel_rotate);

        state_acceptcancel_transalte.setTransitionFrist(state_terrain);
        state_acceptcancel_transalte.setTransitionSecond(state_terrain);

        state_acceptcancel_rotate.setTransitionFrist(state_terrain);
        state_acceptcancel_rotate.setTransitionSecond(state_terrain);

        // -- Transitions - POI
        state_poinew.setTransitionFrist(state_acceptcancel_newpoi);

        state_acceptcancel_newpoi.setTransitionFrist(state_poinew);
        state_acceptcancel_newpoi.setTransitionSecond(state_poinew);

        // -- Transitions - PLANE
        state_plane.setTransitionFrist(state_acceptcancel_newplane);

        state_acceptcancel_newplane.setTransitionFrist(state_plane);
        state_acceptcancel_newplane.setTransitionSecond(state_plane);

        // -- Transitions - TTARView
        //state_ttarview.setTransitionFrist(state_acceptcancel_newttarview);
        //state_ttarview.addTransition(SecondaryFabState.Transitions.EXTRA_INTERACTION_1, state_ttarview_selection, null);

        //state_acceptcancel_newttarview.setTransitionFrist(state_ttarview);
        //state_acceptcancel_newttarview.setTransitionSecond(state_ttarview);

        //state_ttarview_selection.addTransition(SecondaryFabState.Transitions.SECONDARY_FAB_1, state_ttarview_picture_in_picture, listener_open_main_ttarview);
        //state_ttarview_selection.addTransition(SecondaryFabState.Transitions.EXTRA_INTERACTION_1, state_ttarview, null);

        //state_ttarview_picture_in_picture.addTransition(SecondaryFabState.Transitions.SECONDARY_FAB_1, state_ttarview_sketch, listener_ttarview_sketch); // TODO agregar listener para el close

        // Final settings
        m_secondary_fab_context.setMainStates(state_terrain, state_poinew, state_plane, null);//state_ttarview);
        m_activity_state.setSecondaryFabContext(m_secondary_fab_context);
        m_activity_state.addAppModeListener(m_secondary_fab_context);


        // NEW WORLD ORDER //***************************************************************************
        UIFacade.getInstance().init(fab, fab_1, fab_2, getSupportFragmentManager());
        m_activity_state.addApplicationModeChanged(UIContextManager.getInstance());

        UIContext context = new UIContext();
        UIContextManager.getInstance().registerContext(MainActivityState.ApplicationMode.TTARVIEW, context);

        UIState state_ttarview_base = new UIStateTTARViewBase();
        UIState state_ttarview_creation = new UIStateTTARViewCreation();
        UIState state_ttarview_selected = new UIStateTTARViewSelected();
        UIState state_ttarview_edition = new UIStateTTARViewEdition();

        state_ttarview_base.addTransition(UIState.Interactions.SECONDARY_FAB_1, state_ttarview_creation);
        state_ttarview_base.addInteraction(UIState.Interactions.SECONDARY_FAB_1, new ListenerForUITransition(UIState.Interactions.SECONDARY_FAB_1));
        state_ttarview_base.addTransition(UIState.Interactions.EXTRA_INTERACTION_1, state_ttarview_selected);
        state_ttarview_base.addInteraction(UIState.Interactions.EXTRA_INTERACTION_1, null);//new ListenerForUITransition(UIState.Interactions.EXTRA_INTERACTION_1));

        state_ttarview_creation.addTransition(UIState.Interactions.SECONDARY_FAB_1, state_ttarview_base);
        state_ttarview_creation.addInteraction(UIState.Interactions.SECONDARY_FAB_1, new ListenerForUITransition(UIState.Interactions.SECONDARY_FAB_1));
        state_ttarview_creation.addTransition(UIState.Interactions.SECONDARY_FAB_2, state_ttarview_base);
        state_ttarview_creation.addInteraction(UIState.Interactions.SECONDARY_FAB_2, new ListenerForUITransition(UIState.Interactions.SECONDARY_FAB_2));

        state_ttarview_selected.addTransition(UIState.Interactions.SECONDARY_FAB_1, state_ttarview_selected);
        state_ttarview_selected.addInteraction(UIState.Interactions.SECONDARY_FAB_1, new ListenerForUITransition(UIState.Interactions.SECONDARY_FAB_1));
        state_ttarview_selected.addTransition(UIState.Interactions.SECONDARY_FAB_2, state_ttarview_edition); // Edition state
        state_ttarview_selected.addInteraction(UIState.Interactions.SECONDARY_FAB_2, new ListenerForUITransition(UIState.Interactions.SECONDARY_FAB_2)); // Edition listener
        state_ttarview_selected.addTransition(UIState.Interactions.EXTRA_INTERACTION_2, state_ttarview_base);
        state_ttarview_selected.addInteraction(UIState.Interactions.EXTRA_INTERACTION_2, null);//new ListenerForUITransition(UIState.Interactions.EXTRA_INTERACTION_2));
        state_ttarview_selected.addTransition(UIState.Interactions.EXTRA_INTERACTION_3, state_ttarview_base);
        state_ttarview_selected.addInteraction(UIState.Interactions.EXTRA_INTERACTION_3, null);//new ListenerForUITransition(UIState.Interactions.EXTRA_INTERACTION_3));

        state_ttarview_edition.addTransition(UIState.Interactions.SECONDARY_FAB_1, state_ttarview_selected);
        state_ttarview_edition.addInteraction(UIState.Interactions.SECONDARY_FAB_1, new ListenerForUITransition(UIState.Interactions.SECONDARY_FAB_1));
        state_ttarview_edition.addTransition(UIState.Interactions.SECONDARY_FAB_2, state_ttarview_selected);
        state_ttarview_edition.addInteraction(UIState.Interactions.SECONDARY_FAB_2, new ListenerForUITransition(UIState.Interactions.SECONDARY_FAB_2));
        state_ttarview_edition.addTransition(UIState.Interactions.EXTRA_INTERACTION_2, state_ttarview_base);
        state_ttarview_edition.addTransition(UIState.Interactions.EXTRA_INTERACTION_3, state_ttarview_base);

        context.setState(state_ttarview_base);
        //**********************************************************************************************
    }

    private void setupViewPager(ViewPager viewPager){
        TabSectionsPagerAdapter adapter = new TabSectionsPagerAdapter(getSupportFragmentManager());
        // 1-- Create, initialize and add the fragment for poi list
        POIListFragment frag_poi_list = POIListFragment.newInstance(1);
        frag_poi_list.setMainActivityState(m_activity_state);
        adapter.addFragment(frag_poi_list, "POI list");

        // 2-- Create, initialize and add the fragment for poi creation/edition
        TabFragmentPOI tab_frag_poi = TabFragmentPOI.newInstance(2);
        tab_frag_poi.setMainActivityState(m_activity_state);
        tab_frag_poi.setArgeoFragment(mArgeoFragment);
        adapter.addFragment(tab_frag_poi, "POI prop.");
        //adapter.addFragment(new Fragment(), "Tab 2");

        // 3-- Set the adapter to the viewpager
        viewPager.setAdapter(adapter);

        // Fragmentados
        FragmentTransaction fragment_transaction;
        // FRAGMENT Plane
        m_fragment_plane = new FragmentPlane();
        m_fragment_plane.setMainActivityState(m_activity_state);
        m_fragment_plane.setArgeoFragment(mArgeoFragment);
        fragment_transaction = getSupportFragmentManager().beginTransaction();
        // Replace the contents of the container with the new fragment
        fragment_transaction.add(R.id.right_menu, m_fragment_plane, "TAG_PLANE");
        fragment_transaction.commit();
        // FRAGMENT TTARView
        m_fragment_ttarview = new FragmentTTARView();
        m_fragment_ttarview.setMainActivityState(m_activity_state);
        m_fragment_ttarview.setArgeoFragment(mArgeoFragment);
        //m_fragment_ttarview.setTTARViewWidthAndHeight(m_ttarview_snapshot_w, m_ttarview_snapshot_h);
        fragment_transaction = getSupportFragmentManager().beginTransaction();
        // Replace the contents of the container with the new fragment
        fragment_transaction.add(R.id.right_menu, m_fragment_ttarview, "TAG_TTARVIEW");
        fragment_transaction.commit();

        // Bonus track
        ImageButton imagebutton = (ImageButton) findViewById(R.id.imagebutton_ttarview_picture_in_picture_close);
        imagebutton.setOnClickListener(new ListenerClosePictureInPicture(this, m_fragment_ttarview));
    }

    private void handlePanning(MotionEvent event){
        final int action = event.getActionMasked();
        switch (action) {
            case MotionEvent.ACTION_DOWN: {
                final int pointerIndex = event.getActionIndex();

                final float x = event.getX(pointerIndex);
                final float y = event.getY(pointerIndex);

                // Remember where we started (for dragging)
                mLastTouchX = x;
                mLastTouchY = y;

                // Save the ID of this pointer (for dragging)
                mActivePointerId = event.getPointerId(0);
                break;
            }

            case MotionEvent.ACTION_MOVE: {
                // Find the index of the active pointer and fetch its position
                final int pointerIndex = event.findPointerIndex(mActivePointerId);

                final float x = event.getX(pointerIndex);
                final float y = event.getY(pointerIndex);

                final double step = 1.0; // Esto deberia ser en metros

                final double delta   = 10;
                final double delta_y = mLastTouchY - y;
                final double delta_x = x - mLastTouchX;

                mArgeoFragment.getViewer().executeOnRenderLoop(new RenderLoopRunnable() {
                    @Override
                    public void run() {
                        if(Math.abs(delta_y) > Math.abs(delta_x)) {
                            if (Math.abs(delta_y) > delta) {
                                mArgeoFragment.getViewer().getScene().getCamera().moveUp(Math.signum(delta_y) * step);
                            }
                        }
                        else {
                            if (Math.abs(delta_x) > delta) {
                                mArgeoFragment.getViewer().getScene().getCamera().moveLeft(Math.signum(delta_x) * step);
                            }
                        }
                    }
                });

                // Remember this touch position for the next move event
                mLastTouchX = x;
                mLastTouchY = y;
                break;
            }

            case MotionEvent.ACTION_UP:
                mActivePointerId = MotionEvent.INVALID_POINTER_ID;
                break;

            case MotionEvent.ACTION_CANCEL:
                mActivePointerId = MotionEvent.INVALID_POINTER_ID;
                break;

            case MotionEvent.ACTION_POINTER_UP: {
                final int pointerIndex = event.getActionIndex();
                final int pointerId    = event.getPointerId(pointerIndex);

                if (pointerId == mActivePointerId) {
                    // This was our active pointer going up. Choose a new
                    // active pointer and adjust accordingly.
                    final int newPointerIndex = pointerIndex == 0 ? 1 : 0;
                    mLastTouchX = event.getX(newPointerIndex);
                    mLastTouchY = event.getY(newPointerIndex);

                    mActivePointerId = event.getPointerId(newPointerIndex);
                }
                break;
            }
        }
    }

    private void handleRotation(MotionEvent event){
        final int action = event.getActionMasked();
        switch (action) {
            case MotionEvent.ACTION_DOWN: {
                final int pointerIndex = event.getActionIndex();

                final float x = event.getX(pointerIndex);
                final float y = event.getY(pointerIndex);

                // Remember where we started (for dragging)
                mLastTouchX = x;
                mLastTouchY = y;

                // Save the ID of this pointer (for dragging)
                mActivePointerId = event.getPointerId(0);
                break;
            }

            case MotionEvent.ACTION_MOVE: {
                // Find the index of the active pointer and fetch its position
                final int pointerIndex = event.findPointerIndex(mActivePointerId);

                final float x = event.getX(pointerIndex);
                final float y = event.getY(pointerIndex);

                final double delta = 25;
                final double angle = 0.25;

                if ( Math.abs(y - mLastTouchY ) > delta ) {
                    if ( y > mLastTouchY ){
                        mArgeoFragment.getViewer().getScene().getCamera().lookUp(angle / 180.0 * Math.PI);
                    }else{
                        mArgeoFragment.getViewer().getScene().getCamera().lookDown(angle / 180.0 * Math.PI);
                    }
                    mLastTouchY = y;
                }

                if ( Math.abs(x - mLastTouchX ) > delta ) {
                    if ( x > mLastTouchX){
                        mArgeoFragment.getViewer().getScene().getCamera().lookRight(angle / 180.0 * Math.PI);
                    }else{
                        mArgeoFragment.getViewer().getScene().getCamera().lookLeft(angle / 180.0 * Math.PI);
                    }
                    mLastTouchX = x;
                }

                FragmentTerrain frag_terrain = (FragmentTerrain) getSupportFragmentManager().findFragmentById(R.id.right_menu_fragment_terrain);
                frag_terrain.showCameraOrientationParameters(mArgeoFragment.getViewer().getScene().getCamera());

                break;
            }

            case MotionEvent.ACTION_UP:
                mActivePointerId = MotionEvent.INVALID_POINTER_ID;
                break;

            case MotionEvent.ACTION_CANCEL:
                mActivePointerId = MotionEvent.INVALID_POINTER_ID;
                break;

            case MotionEvent.ACTION_POINTER_UP: {
                final int pointerIndex = event.getActionIndex();
                final int pointerId    = event.getPointerId(pointerIndex);

                if (pointerId == mActivePointerId) {
                    // This was our active pointer going up. Choose a new
                    // active pointer and adjust accordingly.
                    final int newPointerIndex = pointerIndex == 0 ? 1 : 0;
                    mLastTouchX = event.getX(newPointerIndex);
                    mLastTouchY = event.getY(newPointerIndex);
                    mActivePointerId = event.getPointerId(newPointerIndex);

                    // ver si aca hago algo
                }
                break;
            }
        }
    }

    @Override
    public void onTouchModeStateChanged(MainActivityState.TouchMode new_state){
        switch ( m_activity_state.getTouchMode() ){
            case DEFAULT :
                m_drawer.setDrawerLockMode(DrawerLayout.LOCK_MODE_UNLOCKED);
                break;

            case POI_CREATION :
                m_drawer.setDrawerLockMode(DrawerLayout.LOCK_MODE_LOCKED_CLOSED);
                break;

            case PLANE_CREATION :
                m_drawer.setDrawerLockMode(DrawerLayout.LOCK_MODE_LOCKED_CLOSED);
                break;

            case CAMERA_TRANSLATION :
                m_drawer.setDrawerLockMode(DrawerLayout.LOCK_MODE_LOCKED_CLOSED);
                break;

            case CAMERA_ROTATION :
                m_drawer.setDrawerLockMode(DrawerLayout.LOCK_MODE_LOCKED_CLOSED);
                break;
        }
    }

    @Override
    public void onHideTerrainToggled(boolean value){
        // TODO: Watch!!
        //mArgeoFragment.getViewer().getScene().getTerrain().setShow(!value); //Notar la negacion, por la semantica del switch

        final boolean val = value;
        mArgeoFragment.getViewer().executeOnRenderLoop(new RenderLoopRunnable() {
            @Override
            public void run() {
                Camera camera        = mArgeoFragment.getViewer().getScene().getCamera();
                vyglab.argeo.jni.Geodetic3D position  = camera.getPositionGeodetic();

                mArgeoFragment.getViewer().getScene().setMode(val ? 0 : 1);


                camera.setView(
                        EllipsoidTransformations.geocentric3DFromGeodetic3D(new vyglab.argeo.jni.Geodetic3D(
                                position.getLatitude(),
                                position.getLongitude(),
                                1.75e6)),
                        0.0,
                        -Math.PI / 2.0,
                        0.0);
            }
        });
    }

    @Override
    public void onWireframeToggled(boolean value){
        mArgeoFragment.getViewer().getScene().getTerrain().setWireframe(value);
    }

    @Override
    public void onIlluminationChanged(boolean value){
        mArgeoFragment.getViewer().getScene().getTerrain().setLighting(value);
    }

    @Override
    public void onOpacityChanged(float value){
        mArgeoFragment.getViewer().getScene().setTerrainOpacity(value);
    }

    @Override
    public void onExaggerationChanged(double value){
        mArgeoFragment.getViewer().getScene().getTerrain().setHeightExaggeration(value);
    }

    double zoom;
    double maxZoom;

    @Override
    public double onCameraHeightChanged(double value){
        // TODO: Cuando la camara cambia la ubicacion se reinicia el zoom!!
        final Camera camera  = mArgeoFragment.getViewer().getScene().getCamera();

        maxZoom = maxZoom == 0.0 ?
                camera.getMagnitude() * 0.9905 :
                maxZoom;

        final double newZoom = Math.log(value + 1.0) / Math.log(100);
        final double oldZoom = zoom;

        mArgeoFragment.getViewer().executeOnRenderLoop(new RenderLoopRunnable() {
            @Override
            public void run() {
                camera.zoomOut(oldZoom * maxZoom);
                camera.zoomIn(newZoom * maxZoom);
            }
        });

        zoom = newZoom;
        return camera.getMagnitude();
    }

    @Override
    public void onKalmanToggled(boolean value){
        //mArgeoFragment.getViewer().getDevicePoseCameraController().setKalmanFilterEnabled(value);
    }

    @Override
    public void onHudOpacityChanged(int value){
        LinearLayout right_menu = (LinearLayout) findViewById(R.id.right_menu);
        int id = R.color.backgroundColorO60;

        // Ya se que es una negrada, cuando tenga tiempo hago la cuenta en hexa
        switch ( value ){
            case 0 :
                id = R.color.backgroundColorO00;
                break;

            case 10 :
                id = R.color.backgroundColorO10;
                break;

            case 20 :
                id = R.color.backgroundColorO20;
                break;

            case 30 :
                id = R.color.backgroundColorO30;
                break;

            case 40 :
                id = R.color.backgroundColorO40;
                break;

            case 50 :
                id = R.color.backgroundColorO50;
                break;

            case 60 :
                id = R.color.backgroundColorO60;
                break;

            case 70 :
                id = R.color.backgroundColorO70;
                break;

            case 80 :
                id = R.color.backgroundColorO80;
                break;

            case 90 :
                id = R.color.backgroundColorO90;
                break;

            case 100 :
                id = R.color.backgroundColor100;
                break;
        }

        right_menu.setBackgroundColor(ResourcesCompat.getColor(getResources(), id, null));

        m_preferences.putInt(R.string.pref_hud_opacity_key, value);
    }

    @Override
    public void onHudWidthChanged(int value){
        LinearLayout right_menu = (LinearLayout) findViewById(R.id.right_menu);
        RelativeLayout.LayoutParams params = (RelativeLayout.LayoutParams) right_menu.getLayoutParams();
        params.width = (int) (value * getResources().getDisplayMetrics().density);
        right_menu.setLayoutParams(params);
        m_fab_listener.updatePositioning(value);

        m_preferences.putInt(R.string.pref_hud_width_key, value);
    }

    @Override
    public void onDefaultButtonClicked(){
        FragmentTerrain fragment_terrain = (FragmentTerrain) getSupportFragmentManager().findFragmentById(R.id.right_menu_fragment_terrain);

        fragment_terrain.resetToDefault(false, false, true, 0.7f, 1.0, 1.5f, false,
                getResources().getInteger(R.integer.pref_hud_opacity_default), // 60
                getResources().getInteger(R.integer.pref_hud_width_default));  // 200
    }

    public void openPictureInPicture(TTARView current_ttarview) {
        m_current_ttarview = current_ttarview;
        mArgeoFragment.getViewer().getScene().addSnapshotListener(this);
        HandyPOI.getInstance().addListener(this);
        HandyPlane.getInstance().addListener(this);

        ImageView picture = (ImageView) findViewById(R.id.imageview_ttarview_picture_in_picture);
        //picture.setImageBitmap(current_ttarview.getUpdatedView());
        //todo corregir ttarview por opacity
        picture.setImageBitmap(current_ttarview.getInitialView());

        FrameLayout frame_layout = (FrameLayout) findViewById(R.id.framelayout_ttarview_picture_in_picture);
        frame_layout.setVisibility(View.VISIBLE);

        //m_sketch_filter.setNewBitmap(current_ttarview.getUpdatedView());
        //todo corregir ttarview por opacity
        //m_sketch_filter.setNewBitmap(current_ttarview.getInitialView());
    }

    public TTARView getCurrentPicturInPictureTTARView() {
        return  m_current_ttarview;
    }

    public void updatePictureInPicture(Bitmap image) {
        ImageView image_view = (ImageView) findViewById(R.id.imageview_ttarview_picture_in_picture);
        image_view.setImageBitmap(image);
    }

    public void closePictureInPicture() {
        mArgeoFragment.getViewer().getScene().removeSnapshotListener(this);
        HandyPOI.getInstance().removeListener(this);
        HandyPlane.getInstance().removeListener(this);
        m_current_ttarview = null;

        FrameLayout frame_layout = (FrameLayout) findViewById(R.id.framelayout_ttarview_picture_in_picture);
        frame_layout.setVisibility(View.GONE);

        ImageView image_view = (ImageView) findViewById(R.id.imageview_ttarview_picture_in_picture);
        image_view.setImageResource(R.drawable.ic_menu_camera);
    }

    private void requestTTARSnapshot() {
        // Before doing something that requires a lot of memory,
        // check to see whether the device is in a low memory state.
        ActivityManager.MemoryInfo memoryInfo = getAvailableMemory();


        if (!memoryInfo.lowMemory && memoryInfo.threshold * 1.5f <= memoryInfo.availMem) {
        // Do memory intensive work ...
            mArgeoFragment.getViewer().executeOnRenderLoop(
                    new RenderLoopRunnable() {
                        @Override
                        public void run() {
                            mArgeoFragment.getViewer().getScene().takeTTARSnapshot(
                                    m_current_ttarview.getInitialCameraFrame(),
                                    m_current_ttarview.getUpdatedView(),
                                    m_current_ttarview.getCamera(),
                                    MediatorTTARView.getInstance().getTTARViewWidth(),
                                    MediatorTTARView.getInstance().getTTARViewHeight());
                        }
                    });
        }
        else {
            Log.e("memoria total", memoryInfo.totalMem + "mB");
            Log.e("memoria limite", memoryInfo.threshold + "mB");
            Log.e("memoria disponible", memoryInfo.availMem + "mB");
            System.gc();
        }


    }

    // Get a MemoryInfo object for the device's current memory status.
    private ActivityManager.MemoryInfo getAvailableMemory() {
        ActivityManager activityManager = (ActivityManager) this.getSystemService(ACTIVITY_SERVICE);
        ActivityManager.MemoryInfo memoryInfo = new ActivityManager.MemoryInfo();
        activityManager.getMemoryInfo(memoryInfo);
        return memoryInfo;
    }

    @Override
    public void onPositionPoiChanged() {
        requestTTARSnapshot();
    }

    @Override
    public void onPlanePositionChanged() {
        requestTTARSnapshot();
    }

    @Override
    public void onPlaneStrikeChanged() {
        requestTTARSnapshot();
    }

    @Override
    public void onPlaneDipChanged() {
        requestTTARSnapshot();
    }

    @Override
    public void onPlaneSizeChanged() {
        requestTTARSnapshot();
    }

    @Override
    public void onSnapshotAvailable(Bitmap snapshot) {
        // Nothing to do here
    }

    @Override
    public void onARBufferSnapshotAvailable( Bitmap reality_snapshot, Bitmap virtuality_snapshot, Bitmap mixed_reality_snapshot) {
        // Nothing to do here
    }

    @Override
    public void onTTARSnapshotAvailable(Bitmap reality_snapshot, Bitmap ttar_snapshot, Camera camera) {


        m_current_ttarview.setUpdatedView(ttar_snapshot);
        final Bitmap image = ttar_snapshot;

        final ImageView imageview_updated = (ImageView) findViewById(R.id.imageview_ttarview_picture_in_picture);
        imageview_updated.post(new Runnable() {
            @Override
            public void run() {
                imageview_updated.setImageBitmap(image);
            }
        });
    }

    //region TTARView related class members
    public float getScreenDensity() {
        return m_screen_density;
    }

    public int getScreenWidth() {
        return m_screen_w;
    }

    public int getScreenHeight() {
        return  m_screen_h;
    }

    public float getScreenAspectRatio() {
        return m_screen_aspect_ratio;
    }

    public int getTTARViewSnapshotWidth() {
        return m_ttarview_snapshot_w;
    }

    public void setTTARViewSnapshotWidth(int width) {
        m_ttarview_snapshot_w = width;
    }

    public int getTTARViewSnapshotHeight() {
        return m_ttarview_snapshot_h;
    }

    public void settTTARViewSnapshotHeight(int height) {
        m_ttarview_snapshot_h = height;
    }

    public int getTTARViewPictureInPuctureWidth() {
        return m_ttarview_picture_in_picture_w;
    }

    public void setTTARViewPictureInPuctureWidth(int width) {
        m_ttarview_picture_in_picture_w = width;
    }

    public int getTTARViewPictureInPuctureHeight() {
        return m_ttarview_picture_in_picture_h;
    }

    public void setTTARViewPictureInPuctureHeight(int height) {
        m_ttarview_picture_in_picture_h = height;
    }

    public void updateTTARViewPictureInPictrueSize(int width, int height) {
        int w = Math.max(m_ttarview_picture_in_picture_w_MIN, Math.min(width, m_ttarview_picture_in_picture_w_MAX));
        int h = Math.max(m_ttarview_picture_in_picture_h_MIN, Math.min(height, m_ttarview_picture_in_picture_h_MAX));

        FrameLayout frame_layout = (FrameLayout) findViewById(R.id.framelayout_ttarview_picture_in_picture);
        frame_layout.getLayoutParams().height = h;
        frame_layout.getLayoutParams().width = w;
        frame_layout.requestLayout();

        m_ttarview_picture_in_picture_w = w;
        m_ttarview_picture_in_picture_h = h;

        m_preferences.putInt(R.string.pref_ttarview_pip_width_key, m_ttarview_picture_in_picture_w);
        m_preferences.putInt(R.string.pref_ttarview_pip_height_key, m_ttarview_picture_in_picture_h);
    }

    public void resetTTARViewPictureInPictrueSize() {
        FrameLayout frame_layout = (FrameLayout) findViewById(R.id.framelayout_ttarview_picture_in_picture);
        frame_layout.getLayoutParams().width = m_ttarview_picture_in_picture_w_MIN;
        frame_layout.getLayoutParams().height = m_ttarview_picture_in_picture_h_MIN;
        frame_layout.requestLayout();

        m_ttarview_picture_in_picture_w = m_ttarview_picture_in_picture_w_MIN;
        m_ttarview_picture_in_picture_h = m_ttarview_picture_in_picture_h_MIN;

        m_preferences.putInt(R.string.pref_ttarview_pip_width_key, m_ttarview_picture_in_picture_w);
        m_preferences.putInt(R.string.pref_ttarview_pip_height_key, m_ttarview_picture_in_picture_h);
    }
    //endregion

    //region DialogChangeTTARVIEWResolution
    @Override
    public void onDialogPositiveClick(DialogChangeTTARVIEWResolution dialog){
        m_ttarview_snapshot_w = dialog.getW();
        m_ttarview_snapshot_h = (int) ((float) m_ttarview_snapshot_w / m_screen_aspect_ratio);
    }

    @Override
    public void onDialogNegativeClick(DialogChangeTTARVIEWResolution dialog){
        //Nothing to do yet
    }
    //endregion

}
