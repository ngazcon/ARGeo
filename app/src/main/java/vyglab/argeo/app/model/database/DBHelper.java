package vyglab.argeo.app.model.database;

import android.content.Context;
import android.database.sqlite.SQLiteOpenHelper;
import android.database.sqlite.SQLiteDatabase;

/**
 * Created by root on 03/12/16.
 */

public class DBHelper extends SQLiteOpenHelper {
    // If you change the database schema, you must increment the database version.
    public static final int DATABASE_VERSION = 2;
    public static final String DATABASE_NAME = "Database.db";

    // Poi table creation statement
    private static final String TEXT_TYPE = " TEXT";
    private static final String INTEGER_TYPE = " INTEGER";
    private static final String DOUBLE_TYPE = " REAL";
    private static  final String BLOB_TYPE = " BLOB";
    private static final String COMMA_SEP = ",";
    private static final String SQL_CREATE_POI_ENTRIES =
            "CREATE TABLE " + DatabaseContract.PoiTable.TABLE_NAME + " (" +
                    DatabaseContract.PoiTable._ID + " INTEGER PRIMARY KEY," +
                    DatabaseContract.PoiTable.COLUMN_NAME_POI_ID + TEXT_TYPE + COMMA_SEP +
                    DatabaseContract.PoiTable.COLUMN_NAME_DESCRIPTION + TEXT_TYPE + COMMA_SEP +
                    DatabaseContract.PoiTable.COLUMN_NAME_LONG + DOUBLE_TYPE + COMMA_SEP +
                    DatabaseContract.PoiTable.COLUMN_NAME_LAT + DOUBLE_TYPE + COMMA_SEP +
                    DatabaseContract.PoiTable.COLUMN_NAME_HEIGHT + DOUBLE_TYPE + COMMA_SEP +
                    DatabaseContract.PoiTable.COLUMN_NAME_ENTITY + INTEGER_TYPE + " )";

    // Poi table deletion statement
    private static final String SQL_DELETE_POI_ENTRIES =
            "DROP TABLE IF EXISTS " + DatabaseContract.PoiTable.TABLE_NAME;

    // IGraphic table creation statement
    private static final String SQL_CREATE_IGRAPHIC_ENTRIES =
            "CREATE TABLE " + DatabaseContract.IGraphicTable.TABLE_NAME + " (" +
                    DatabaseContract.IGraphicTable._ID + " INTEGER PRIMARY KEY," +
                    DatabaseContract.IGraphicTable.COLUMN_NAME_TYPE + TEXT_TYPE + COMMA_SEP +
                    DatabaseContract.IGraphicTable.COLUMN_NAME_PATH + TEXT_TYPE + COMMA_SEP +
                    DatabaseContract.IGraphicTable.COLUMN_NAME_VALUE_1 + INTEGER_TYPE + COMMA_SEP +
                    DatabaseContract.IGraphicTable.COLUMN_NAME_VALUE_2 + INTEGER_TYPE + " )";

    // IGraphic table deletion statement
    private static final String SQL_DELETE_IGRAPHIC_ENTRIES =
            "DROP TABLE IF EXISTS " + DatabaseContract.IGraphicTable.TABLE_NAME;

    // TTARView table creation statement
    private static final String SQL_CREATE_TTARVIEW_ENTRIES =
            "CREATE TABLE " + DatabaseContract.TTARViewTable.TABLE_NAME + " (" +
                    DatabaseContract.TTARViewTable._ID + " INTEGER PRIMARY KEY," +
                    DatabaseContract.TTARViewTable.COLUMN_NAME_ID + TEXT_TYPE + COMMA_SEP +
                    DatabaseContract.TTARViewTable.COLUMN_NAME_NAME + TEXT_TYPE + COMMA_SEP +
                    DatabaseContract.TTARViewTable.COLUMN_NAME_DESCRIPTION + TEXT_TYPE + COMMA_SEP +
                    DatabaseContract.TTARViewTable.COLUMN_NAME_LONG + DOUBLE_TYPE + COMMA_SEP +
                    DatabaseContract.TTARViewTable.COLUMN_NAME_LAT + DOUBLE_TYPE + COMMA_SEP +
                    DatabaseContract.TTARViewTable.COLUMN_NAME_HEIGHT + DOUBLE_TYPE + COMMA_SEP +
                    DatabaseContract.TTARViewTable.COLUMN_NAME_CAMERA_YAW + DOUBLE_TYPE + COMMA_SEP +
                    DatabaseContract.TTARViewTable.COLUMN_NAME_CAMERA_PITCH + DOUBLE_TYPE + COMMA_SEP +
                    DatabaseContract.TTARViewTable.COLUMN_NAME_CAMERA_ROLL + DOUBLE_TYPE + COMMA_SEP +
                    DatabaseContract.TTARViewTable.COLUMN_NAME_BITMAP_FRAME + BLOB_TYPE + COMMA_SEP +
                    DatabaseContract.TTARViewTable.COLUMN_NAME_BITMAP_VIEW + BLOB_TYPE + " )";

    // TTARView table deletion statement
    private static final String SQL_DELETE_TTARVIEW_ENTRIES =
            "DROP TABLE IF EXISTS " + DatabaseContract.TTARViewTable.TABLE_NAME;

    public DBHelper(Context context) {
        super(context, DATABASE_NAME, null, DATABASE_VERSION);
    }

    public void onCreate(SQLiteDatabase db) {
        db.execSQL(SQL_CREATE_IGRAPHIC_ENTRIES);
        db.execSQL(SQL_CREATE_POI_ENTRIES);
        db.execSQL(SQL_CREATE_TTARVIEW_ENTRIES);
    }

    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        // This database is only a cache for online data, so its upgrade policy is
        // to simply to discard the data and start over
        db.execSQL(SQL_DELETE_POI_ENTRIES);
        db.execSQL(SQL_DELETE_IGRAPHIC_ENTRIES);
        db.execSQL(SQL_DELETE_TTARVIEW_ENTRIES);
        onCreate(db);
    }

    public void onDowngrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        onUpgrade(db, oldVersion, newVersion);
    }
}