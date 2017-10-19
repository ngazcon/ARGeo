package vyglab.argeo.app.controller;

import android.app.Activity;
import android.content.Context;
import android.location.Location;
import android.location.LocationManager;
import android.support.v7.app.AppCompatActivity;

import vyglab.argeo.R;
import vyglab.argeo.app.model.TTARView;
import vyglab.argeo.app.model.TTARViewRepository;
import vyglab.argeo.app.view.FragmentTerrain;
import vyglab.argeo.jni.ArgeoFragment;
import vyglab.argeo.jni.Camera;
import vyglab.argeo.jni.CameraChangedListener;
import vyglab.argeo.jni.CameraMoveListener;
import vyglab.argeo.jni.EllipsoidTransformations;
import vyglab.argeo.jni.Geocentric3D;
import vyglab.argeo.jni.Geodetic3D;
import vyglab.argeo.jni.RenderLoopRunnable;
import vyglab.argeo.jni.ViewerStateListener;
import vyglab.argeo.app.model.POI;
import vyglab.argeo.app.model.POIRepository;
import vyglab.argeo.app.model.database.DBManager;

import java.util.ArrayList;

/**
 * Created by ngazcon on 14/02/2017.
 */

public class InitializeEngine implements ViewerStateListener {
    private DBManager m_DBManager;
    private ArgeoFragment mArgeoFragment;
    private AppCompatActivity mActivity;

    public InitializeEngine(
            DBManager manager,
            ArgeoFragment fragment,
            AppCompatActivity activity){
        m_DBManager    = manager;
        mArgeoFragment = fragment;
        mActivity      = activity;
    }

    public void onStartRenderLoop(){
        //TODO por bizarren party en la creacion seguida de TTARViews saque esto porq se corrompe la DB y rebienta al cargar
        /*
        ArrayList<POI> poi_list = m_DBManager.obtainBillboardList();

        for (POI poi : poi_list) {
            POIRepository.getInstance().addPOI(poi);
            mArgeoFragment.getViewer().getEntities().add(poi.getGraphic());
        }

        ArrayList<TTARView> ttarview_list = m_DBManager.obtainTTARViewList();

        for (TTARView ttarview : ttarview_list) {
            TTARViewRepository.getInstance().addTTARView(ttarview);
        }
        */

        mArgeoFragment.getViewer().getScene().getCamera().addChangedListener(new CameraChangedListener() {
            @Override
            public void onChanged(double percentage, Camera camera) {
                mActivity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        FragmentTerrain frag_terrain = (FragmentTerrain) mActivity.getSupportFragmentManager().findFragmentById(R.id.right_menu_fragment_terrain);
                        frag_terrain.showCameraPositionParameters(mArgeoFragment.getViewer().getScene().getCamera());
                    }
                });
            }
        });

        mArgeoFragment.getViewer().getScene().getCamera().addMoveListener(new CameraMoveListener() {
            @Override
            public void onMoveStart(Camera camera) {
            }

            @Override
            public void onMoveEnd(Camera camera) {
                mActivity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        FragmentTerrain frag_terrain = (FragmentTerrain) mActivity.getSupportFragmentManager().findFragmentById(R.id.right_menu_fragment_terrain);
                        frag_terrain.showCameraPositionParameters(mArgeoFragment.getViewer().getScene().getCamera());
                    }
                });
            }
        });

        /*
        mArgeoFragment.getViewer().executeOnRenderLoop(new RenderLoopRunnable() {
            @Override
            public void run() {
                mArgeoFragment.getViewer().getScene().setMode(0);

                LocationManager lm = (LocationManager) mActivity.getSystemService(Context.LOCATION_SERVICE);
                Location location  =  lm.getLastKnownLocation(LocationManager.GPS_PROVIDER);

                Geocentric3D position2D = EllipsoidTransformations.geocentric3DFromGeodetic3D(new Geodetic3D(
                        location.getLongitude(),
                        location.getLatitude(),
                        1.75e6));

                Camera camera = mArgeoFragment.getViewer().getScene().getCamera();
                camera.setUp(new Geocentric3D(0.0, 1.0, 0.0));
                camera.setDirection(new Geocentric3D(-1.0, 0.0, 0.0));
                camera.setPosition(position2D);
            }
        });
        */
    }

    public void onStopRenderLoop() {
    }
}
