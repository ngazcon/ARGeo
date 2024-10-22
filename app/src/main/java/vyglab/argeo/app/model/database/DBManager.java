package vyglab.argeo.app.model.database;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;

import vyglab.argeo.app.model.Plane;
import vyglab.argeo.app.model.PlaneBuilder;
import vyglab.argeo.app.model.TTARView;
import vyglab.argeo.app.model.TTARViewBuilder;
import vyglab.argeo.jni.ArgeoFragment;
import vyglab.argeo.jni.BillboardGraphics;
import vyglab.argeo.app.model.POI;
import vyglab.argeo.app.model.PoiBillboardBuilder;
import vyglab.argeo.jni.EllipsoidTransformations;
import vyglab.argeo.jni.Geocentric3D;
import vyglab.argeo.jni.Geodetic3D;

import java.util.ArrayList;

/**
 * Created by root on 03/12/16.
 */

public class DBManager {
    private DBHelper m_dbhelper;
    // Only for the camera creation (TTARViewBuilder)
    private ArgeoFragment m_argeofragment;

    public DBManager(Context context, ArgeoFragment fragment){
        m_dbhelper = new DBHelper(context);
        m_argeofragment = fragment;
    }

    public boolean insertPoi(POI poi){
        // Gets the data repository in write mode
        SQLiteDatabase db = m_dbhelper.getWritableDatabase();

        // Create a new map of values, where column names are the keys
        ContentValues igraphic_values = new ContentValues();
        BillboardGraphics billboard = (BillboardGraphics) poi.getGraphic().getGraphics();
        if (billboard != null){
            igraphic_values.put(DatabaseContract.IGraphicTable.COLUMN_NAME_TYPE, "billboard");
            igraphic_values.put(DatabaseContract.IGraphicTable.COLUMN_NAME_PATH, billboard.getImagePath());
            igraphic_values.put(DatabaseContract.IGraphicTable.COLUMN_NAME_VALUE_1, billboard.getWidth());
            igraphic_values.put(DatabaseContract.IGraphicTable.COLUMN_NAME_VALUE_2, billboard.getHeight());
        }

        // Insert the new row, returning the primary key value of the new row
        long newRowId = db.insert(DatabaseContract.IGraphicTable.TABLE_NAME, null, igraphic_values);

        if (newRowId != -1){
            ContentValues poi_values = new ContentValues();
            poi_values.put(DatabaseContract.PoiTable.COLUMN_NAME_POI_ID, poi.getId());
            poi_values.put(DatabaseContract.PoiTable.COLUMN_NAME_DESCRIPTION, poi.getDescription());
            poi_values.put(DatabaseContract.PoiTable.COLUMN_NAME_LONG, poi.getPosition().getLong());
            poi_values.put(DatabaseContract.PoiTable.COLUMN_NAME_LAT, poi.getPosition().getLat());
            poi_values.put(DatabaseContract.PoiTable.COLUMN_NAME_HEIGHT, poi.getPosition().getHeight());
            poi_values.put(DatabaseContract.PoiTable.COLUMN_NAME_ENTITY, newRowId);

            newRowId = db.insert(DatabaseContract.PoiTable.TABLE_NAME, null, poi_values);
        }

        db.close();

        return ( newRowId != -1 );
    }

    public boolean insertPlane(Plane plane){
        // Gets the data repository in write mode
        SQLiteDatabase db = m_dbhelper.getWritableDatabase();

        // Create a new map of values, where column names are the keys
        ContentValues plane_values = new ContentValues();

        plane_values.put(DatabaseContract.PlaneTable.COLUMN_NAME_PLANE_ID, plane.getId());
        plane_values.put(DatabaseContract.PlaneTable.COLUMN_NAME_NAME, plane.getName());
        plane_values.put(DatabaseContract.PlaneTable.COLUMN_NAME_DESCRIPTION, plane.getDescription());
        plane_values.put(DatabaseContract.PlaneTable.COLUMN_NAME_LONG, plane.getPosition().getLong());
        plane_values.put(DatabaseContract.PlaneTable.COLUMN_NAME_LAT, plane.getPosition().getLat());
        plane_values.put(DatabaseContract.PlaneTable.COLUMN_NAME_HEIGHT, plane.getPosition().getHeight());
        plane_values.put(DatabaseContract.PlaneTable.COLUMN_NAME_VIRTUAL_ORIENTATION, plane.getVirtualOrientation());
        plane_values.put(DatabaseContract.PlaneTable.COLUMN_NAME_DIP, plane.getDip());
        plane_values.put(DatabaseContract.PlaneTable.COLUMN_NAME_STRIKE, plane.getStrike());
        plane_values.put(DatabaseContract.PlaneTable.COLUMN_NAME_SIZE, plane.getSize());
        plane_values.put(DatabaseContract.PlaneTable.COLUMN_NAME_THICKNESS, plane.getThickness());
        plane_values.put(DatabaseContract.PlaneTable.COLUMN_NAME_SHOW_VIRTUAL_ORIENTATION, plane.getShowVirtualOrientationPlane());

        // Insert the new row, returning the primary key value of the new row
        long newRowId = db.insert(DatabaseContract.PlaneTable.TABLE_NAME, null, plane_values);

        db.close();

        return ( newRowId != -1 );
    }

    public boolean deletePlane(Plane plane){
        // Gets the data repository in write mode
        SQLiteDatabase db = m_dbhelper.getWritableDatabase();

        long newRowId = db.delete(DatabaseContract.PlaneTable.TABLE_NAME,
                DatabaseContract.PlaneTable.COLUMN_NAME_PLANE_ID.concat("= ?"),
                new String[]{ plane.getId()});
        db.close();

        return ( newRowId != 0 );
    }

    public boolean updatePlane(Plane plane) {
        // Gets the data repository in write mode
        SQLiteDatabase db = m_dbhelper.getWritableDatabase();

        // Create a new map of values, where column names are the keys
        ContentValues plane_values = new ContentValues();
        plane_values.put(DatabaseContract.PlaneTable.COLUMN_NAME_PLANE_ID, plane.getId());
        plane_values.put(DatabaseContract.PlaneTable.COLUMN_NAME_NAME, plane.getName());
        plane_values.put(DatabaseContract.PlaneTable.COLUMN_NAME_DESCRIPTION, plane.getDescription());
        plane_values.put(DatabaseContract.PlaneTable.COLUMN_NAME_LONG, plane.getPosition().getLong());
        plane_values.put(DatabaseContract.PlaneTable.COLUMN_NAME_LAT, plane.getPosition().getLat());
        plane_values.put(DatabaseContract.PlaneTable.COLUMN_NAME_HEIGHT, plane.getPosition().getHeight());
        plane_values.put(DatabaseContract.PlaneTable.COLUMN_NAME_VIRTUAL_ORIENTATION, plane.getVirtualOrientation());
        plane_values.put(DatabaseContract.PlaneTable.COLUMN_NAME_DIP, plane.getDip());
        plane_values.put(DatabaseContract.PlaneTable.COLUMN_NAME_STRIKE, plane.getStrike());
        plane_values.put(DatabaseContract.PlaneTable.COLUMN_NAME_SIZE, plane.getSize());
        plane_values.put(DatabaseContract.PlaneTable.COLUMN_NAME_THICKNESS, plane.getThickness());
        plane_values.put(DatabaseContract.PlaneTable.COLUMN_NAME_SHOW_VIRTUAL_ORIENTATION, plane.getShowVirtualOrientationPlane());

        long newRowId = db.update(DatabaseContract.PlaneTable.TABLE_NAME,
                plane_values,
                DatabaseContract.PlaneTable.COLUMN_NAME_PLANE_ID.concat("= ?"),
                new String[]{ plane.getId()});
        db.close();

        return ( newRowId != 0 );
    }

    public boolean insertTTARView(TTARView ttarview){
        // TODO: Esto hacerlo asincronicamente.
        // Gets the data repository in write mode
        SQLiteDatabase db = m_dbhelper.getWritableDatabase();

        // Create a new map of values, where column names are the keys
        ContentValues ttarview_values = new ContentValues();
        ttarview_values.put(DatabaseContract.TTARViewTable.COLUMN_NAME_ID, ttarview.getId());
        ttarview_values.put(DatabaseContract.TTARViewTable.COLUMN_NAME_NAME, ttarview.getName());
        ttarview_values.put(DatabaseContract.TTARViewTable.COLUMN_NAME_DESCRIPTION, ttarview.getDescription());
        ttarview_values.put(DatabaseContract.TTARViewTable.COLUMN_NAME_LONG, ttarview.getPosition().getLong());
        ttarview_values.put(DatabaseContract.TTARViewTable.COLUMN_NAME_LAT, ttarview.getPosition().getLat());
        ttarview_values.put(DatabaseContract.TTARViewTable.COLUMN_NAME_HEIGHT, ttarview.getPosition().getHeight());
        ttarview_values.put(DatabaseContract.TTARViewTable.COLUMN_NAME_CAMERA_YAW, ttarview.getCamera().getYaw());
        ttarview_values.put(DatabaseContract.TTARViewTable.COLUMN_NAME_CAMERA_ROLL, ttarview.getCamera().getRoll());
        ttarview_values.put(DatabaseContract.TTARViewTable.COLUMN_NAME_BITMAP_FRAME, ttarview.getStoreableBitmap(ttarview.getInitialCameraFrame()));
        ttarview_values.put(DatabaseContract.TTARViewTable.COLUMN_NAME_BITMAP_VIEW, ttarview.getStoreableBitmap(ttarview.getInitialView()));

        // Insert the new row, returning the primary key value of the new row
        long newRowId = db.insert(DatabaseContract.TTARViewTable.TABLE_NAME, null, ttarview_values);

        db.close();

        return ( newRowId != -1 );
        //return true;
    }

    public boolean deleteTTARView(TTARView ttarview){
        // TODO: Esto hacerlo asincronicamente.
        // Gets the data repository in write mode
        SQLiteDatabase db = m_dbhelper.getWritableDatabase();

        long newRowId = db.delete(DatabaseContract.TTARViewTable.TABLE_NAME, DatabaseContract.TTARViewTable.COLUMN_NAME_ID.concat("= ?"), new String[]{ ttarview.getId()});
        db.close();

        return ( newRowId != 0 );
    }

    public boolean updateTTARView(TTARView ttarview) {
        // Gets the data repository in write mode
        SQLiteDatabase db = m_dbhelper.getWritableDatabase();

        // Create a new map of values, where column names are the keys
        ContentValues ttarview_values = new ContentValues();
        ttarview_values.put(DatabaseContract.TTARViewTable.COLUMN_NAME_ID, ttarview.getId());
        ttarview_values.put(DatabaseContract.TTARViewTable.COLUMN_NAME_NAME, ttarview.getName());
        ttarview_values.put(DatabaseContract.TTARViewTable.COLUMN_NAME_DESCRIPTION, ttarview.getDescription());
        //ttarview_values.put(DatabaseContract.TTARViewTable.COLUMN_NAME_LONG, ttarview.getPosition().getLong());
        //ttarview_values.put(DatabaseContract.TTARViewTable.COLUMN_NAME_LAT, ttarview.getPosition().getLat());
        //ttarview_values.put(DatabaseContract.TTARViewTable.COLUMN_NAME_HEIGHT, ttarview.getPosition().getHeight());
        //ttarview_values.put(DatabaseContract.TTARViewTable.COLUMN_NAME_CAMERA_YAW, ttarview.getCamera().getYaw());
        //ttarview_values.put(DatabaseContract.TTARViewTable.COLUMN_NAME_CAMERA_ROLL, ttarview.getCamera().getRoll());
        //ttarview_values.put(DatabaseContract.TTARViewTable.COLUMN_NAME_BITMAP_FRAME, ttarview.getStoreableBitmap(ttarview.getInitialCameraFrame()));
        //ttarview_values.put(DatabaseContract.TTARViewTable.COLUMN_NAME_BITMAP_VIEW, ttarview.getStoreableBitmap(ttarview.getInitialView()));

        long newRowId = db.update(DatabaseContract.TTARViewTable.TABLE_NAME, ttarview_values, DatabaseContract.TTARViewTable.COLUMN_NAME_ID.concat("= ?"), new String[]{ ttarview.getId()});
        db.close();

        return ( newRowId != 0 );
    }

    public boolean deletePoi(POI poi){
        return true;
    }

    public boolean updatePoi(POI poi){
        return true;
    }

    public ArrayList<String> listPois(){
        SQLiteDatabase db = m_dbhelper.getReadableDatabase();

        // Define a projection that specifies which columns from the database
        // you will actually use after this query.
        String[] projection = {
                DatabaseContract.PoiTable._ID,
                DatabaseContract.PoiTable.COLUMN_NAME_POI_ID,
                DatabaseContract.PoiTable.COLUMN_NAME_DESCRIPTION,
                DatabaseContract.PoiTable.COLUMN_NAME_ENTITY
        };

        // Filter results WHERE "title" = 'My Title'
        String selection = "?";
        String[] selectionArgs = { "*" };

        // How you want the results sorted in the resulting Cursor
        String sortOrder = DatabaseContract.PoiTable._ID + " DESC";

        /*Cursor cursor = db.query(
                DatabaseContract.PoiTable.TABLE_NAME,     // The table to query
                projection,                               // The columns to return
                selection,                                // The columns for the WHERE clause
                selectionArgs,                            // The values for the WHERE clause
                null,                                     // don't group the rows
                null,                                     // don't filter by row groups
                sortOrder                                 // The sort order
        );*/

        //TODO Esto es para borrar la mente
        //db.execSQL("delete from poi");
        //db.execSQL("delete from igraphic");
        Cursor cursor = db.rawQuery("select * from poi, igraphic where entity = igraphic._ID and igraphic.type = 'billboard'", null);

        ArrayList<String> array_list = new ArrayList<String>();
        cursor.moveToFirst();

        while(cursor.isAfterLast() == false) {
            String poi_and_graphic = cursor.getString(cursor.getColumnIndex(DatabaseContract.PoiTable.COLUMN_NAME_POI_ID));
            poi_and_graphic = poi_and_graphic.concat(" - ");
            poi_and_graphic = poi_and_graphic.concat(cursor.getString(cursor.getColumnIndex(DatabaseContract.IGraphicTable.COLUMN_NAME_TYPE)));
            poi_and_graphic = poi_and_graphic.concat("(");
            poi_and_graphic = poi_and_graphic.concat(cursor.getString(cursor.getColumnIndex(DatabaseContract.IGraphicTable._ID)));
            poi_and_graphic = poi_and_graphic.concat(")");
            array_list.add(poi_and_graphic);
            cursor.moveToNext();
        }

        return array_list;
    }

    public ArrayList<POI> obtainBillboardList() {
        SQLiteDatabase db = m_dbhelper.getReadableDatabase();
        Cursor cursor = db.rawQuery("select * from poi, igraphic where entity = igraphic._ID", null);
        ArrayList<POI> array_list = new ArrayList<POI>();

        cursor.moveToFirst();

        while(cursor.isAfterLast() == false){
            PoiBillboardBuilder builder = new PoiBillboardBuilder();
            builder.configurePoiBuilder(
                    cursor.getString(cursor.getColumnIndex(DatabaseContract.PoiTable.COLUMN_NAME_POI_ID)),
                    cursor.getString(cursor.getColumnIndex(DatabaseContract.PoiTable.COLUMN_NAME_DESCRIPTION)),
                    new vyglab.argeo.app.model.Geodetic3D(
                            cursor.getDouble(cursor.getColumnIndex(DatabaseContract.PoiTable.COLUMN_NAME_LAT)),
                            cursor.getDouble(cursor.getColumnIndex(DatabaseContract.PoiTable.COLUMN_NAME_LONG)),
                            cursor.getDouble(cursor.getColumnIndex(DatabaseContract.PoiTable.COLUMN_NAME_HEIGHT))
                            )
            );

            //TODO ver este bizarren peperoni
            String peperoni = cursor.getString(cursor.getColumnIndex(DatabaseContract.IGraphicTable.COLUMN_NAME_PATH));
            PoiBillboardBuilder.BillboardIcon icon = PoiBillboardBuilder.getIconFromPath(peperoni);
            builder.configureBillboardBuilder(
                    icon,
                    cursor.getInt(cursor.getColumnIndex(DatabaseContract.IGraphicTable.COLUMN_NAME_VALUE_1)),
                    cursor.getInt(cursor.getColumnIndex(DatabaseContract.IGraphicTable.COLUMN_NAME_VALUE_2))
            );
            builder.createNewPoi();
            POI p = builder.getPoi();
            array_list.add(builder.getPoi());

            cursor.moveToNext();
        }

        return array_list;
    }

    public ArrayList<Plane> obtainPlaneList() {
        SQLiteDatabase db = m_dbhelper.getReadableDatabase();
        Cursor cursor = db.rawQuery("select * from plane", null);
        ArrayList<Plane> array_list = new ArrayList<Plane>();

        cursor.moveToFirst();

        while(cursor.isAfterLast() == false){
            PlaneBuilder builder = new PlaneBuilder(m_argeofragment);

            // Obtain geocentric from geodetic values
            Double latitude = cursor.getDouble(cursor.getColumnIndex(DatabaseContract.PlaneTable.COLUMN_NAME_LAT));
            Double longitude = cursor.getDouble(cursor.getColumnIndex(DatabaseContract.PlaneTable.COLUMN_NAME_LONG));
            Double height = cursor.getDouble(cursor.getColumnIndex(DatabaseContract.PlaneTable.COLUMN_NAME_HEIGHT));
            Geodetic3D geodetic = new Geodetic3D(latitude, longitude, height);
            Geocentric3D geocentric = EllipsoidTransformations.geocentric3DFromGeodetic3D(geodetic);

            // Obtain boolean value corresponding to whether to show virtual orientation plane
            boolean show_virtual_orientation = (cursor.getInt(cursor.getColumnIndex(DatabaseContract.PlaneTable.COLUMN_NAME_SHOW_VIRTUAL_ORIENTATION)) == 1);

            // Build plane
            Plane plane = builder.setId(cursor.getString(cursor.getColumnIndex(DatabaseContract.PlaneTable.COLUMN_NAME_PLANE_ID)))
                    .setName(cursor.getString(cursor.getColumnIndex(DatabaseContract.PlaneTable.COLUMN_NAME_NAME)))
                    .setDescription(cursor.getString(cursor.getColumnIndex(DatabaseContract.PlaneTable.COLUMN_NAME_DESCRIPTION)))
                    .setPosition(geocentric)
                    .setVirtualOrientation(cursor.getInt(cursor.getColumnIndex(DatabaseContract.PlaneTable.COLUMN_NAME_VIRTUAL_ORIENTATION)))
                    .setDip(cursor.getInt(cursor.getColumnIndex(DatabaseContract.PlaneTable.COLUMN_NAME_DIP)))
                    .setStrike(cursor.getInt(cursor.getColumnIndex(DatabaseContract.PlaneTable.COLUMN_NAME_STRIKE)))
                    .setSize(cursor.getInt(cursor.getColumnIndex(DatabaseContract.PlaneTable.COLUMN_NAME_SIZE)))
                    .setThickness(cursor.getInt(cursor.getColumnIndex(DatabaseContract.PlaneTable.COLUMN_NAME_THICKNESS)))
                    .setShowVirtualOrientationPlane(show_virtual_orientation)
                    .build();

            // Add plane to the list
            array_list.add(plane);

            // Finally move to the next cursor value
            cursor.moveToNext();
        }

        return array_list;
    }

    public ArrayList<TTARView> obtainTTARViewList() {
        SQLiteDatabase db = m_dbhelper.getReadableDatabase();
        Cursor cursor = db.rawQuery("select * from ttarview", null);
        ArrayList<TTARView> array_list = new ArrayList<TTARView>();

        cursor.moveToFirst();

        while(cursor.isAfterLast() == false){
            TTARViewBuilder builder = new TTARViewBuilder();
            builder.configureTTARViewBuilder(
                    cursor.getString(cursor.getColumnIndex(DatabaseContract.TTARViewTable.COLUMN_NAME_ID)),
                    cursor.getString(cursor.getColumnIndex(DatabaseContract.TTARViewTable.COLUMN_NAME_NAME)),
                    cursor.getString(cursor.getColumnIndex(DatabaseContract.TTARViewTable.COLUMN_NAME_DESCRIPTION)),
                    cursor.getDouble(cursor.getColumnIndex(DatabaseContract.TTARViewTable.COLUMN_NAME_LONG)),
                    cursor.getDouble(cursor.getColumnIndex(DatabaseContract.TTARViewTable.COLUMN_NAME_LAT)),
                    cursor.getDouble(cursor.getColumnIndex(DatabaseContract.TTARViewTable.COLUMN_NAME_HEIGHT)),
                    cursor.getDouble(cursor.getColumnIndex(DatabaseContract.TTARViewTable.COLUMN_NAME_CAMERA_YAW)),
                    cursor.getDouble(cursor.getColumnIndex(DatabaseContract.TTARViewTable.COLUMN_NAME_CAMERA_PITCH)),
                    cursor.getDouble(cursor.getColumnIndex(DatabaseContract.TTARViewTable.COLUMN_NAME_CAMERA_ROLL)),
                    cursor.getBlob(cursor.getColumnIndex(DatabaseContract.TTARViewTable.COLUMN_NAME_BITMAP_FRAME)),
                    cursor.getBlob(cursor.getColumnIndex(DatabaseContract.TTARViewTable.COLUMN_NAME_BITMAP_VIEW)),
                    m_argeofragment.getViewer().getScene()
            );

            builder.createTTARView();
            array_list.add(builder.getTTARView());

            cursor.moveToNext();
        }

        return array_list;
    }

    public void clearDB() {
        SQLiteDatabase db = m_dbhelper.getWritableDatabase();
        db.execSQL("delete from poi");
        db.execSQL("delete from igraphic");
        db.execSQL("delete from plane");
        db.execSQL("delete from ttarview");
    }
}
