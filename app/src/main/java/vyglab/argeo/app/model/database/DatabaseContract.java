package vyglab.argeo.app.model.database;

import android.provider.BaseColumns;

/**
 * Created by root on 03/12/16.
 */

public final class DatabaseContract {
    // To prevent someone from accidentally instantiating the contract class,
    // make the constructor private.
    private DatabaseContract() {}

    /* Inner classes that defines the table contents */
    public static class PoiTable implements BaseColumns {
        public static final String TABLE_NAME = "poi";
        public static final String COLUMN_NAME_POI_ID = "poi_id";
        public static final String COLUMN_NAME_DESCRIPTION = "description";
        public static final String COLUMN_NAME_LONG = "long";
        public static final String COLUMN_NAME_LAT = "lat";
        public static final String COLUMN_NAME_HEIGHT = "height";
        public static final String COLUMN_NAME_ENTITY = "entity";
    }

    public static class IGraphicTable implements BaseColumns {
        public static final String TABLE_NAME = "igraphic";
        public static final String COLUMN_NAME_TYPE = "type";
        public static final String COLUMN_NAME_PATH = "path";
        public static final String COLUMN_NAME_VALUE_1 = "value_1";
        public static final String COLUMN_NAME_VALUE_2 = "value_2";
    }

    public static class PlaneTable implements BaseColumns {
        public static final String TABLE_NAME = "plane";
        public static final String COLUMN_NAME_PLANE_ID = "plane_id";
        public static final String COLUMN_NAME_NAME = "name";
        public static final String COLUMN_NAME_DESCRIPTION = "description";
        public static final String COLUMN_NAME_LONG = "long";
        public static final String COLUMN_NAME_LAT = "lat";
        public static final String COLUMN_NAME_HEIGHT = "height";
        public static final String COLUMN_NAME_VIRTUAL_ORIENTATION = "virtual_orientation";
        public static final String COLUMN_NAME_DIP = "dip";
        public static final String COLUMN_NAME_STRIKE = "strike";
        public static final String COLUMN_NAME_SIZE = "size";
        public static final String COLUMN_NAME_THICKNESS = "thickness";
        public static final String COLUMN_NAME_SHOW_VIRTUAL_ORIENTATION = "show_virtual_orientation";
    }

    public static class TTARViewTable implements BaseColumns {
        public static final String TABLE_NAME = "ttarview";
        public static final String COLUMN_NAME_ID = "id";
        public static final String COLUMN_NAME_NAME = "name";
        //public static final String COLUMN_NAME_TTARVIEW_ID = "id";
        public static final String COLUMN_NAME_DESCRIPTION = "description";
        public static final String COLUMN_NAME_LONG = "long";
        public static final String COLUMN_NAME_LAT = "lat";
        public static final String COLUMN_NAME_HEIGHT = "height";
        public static final String COLUMN_NAME_CAMERA_YAW = "yaw";
        public static final String COLUMN_NAME_CAMERA_PITCH = "pitch";
        public static final String COLUMN_NAME_CAMERA_ROLL = "roll";
        public static final String COLUMN_NAME_BITMAP_FRAME = "bitmap_frame";
        public static final String COLUMN_NAME_BITMAP_VIEW = "bitmap_view";
    }
}