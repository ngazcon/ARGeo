package vyglab.argeo.app.utils;

import android.content.Context;
import android.os.Environment;

import java.io.File;
import java.util.ArrayList;

/**
 * Created by ngazcon on 03/08/2016.
 */
public class StorageManager {

    private static StorageManager m_instance = null;
    private static Context m_context;

    protected StorageManager(){
        // Exists only to defeat instantiation.
    }

    public static StorageManager getInstance(Context context) {
        if(m_instance == null) {
            m_instance = new StorageManager();
            //TODO ver por que rebienta aca
            //m_instance.init();
        }
        m_context = context;

        return m_instance;
    }

    private void init(){
        File file = new File(getAppPathOnExternalStorage().concat("/Raster"));
        if( !file.exists()) {
            file.mkdirs();
        }

        file = new File(getAppPathOnExternalStorage().concat("/Raster/SRTM"));
        if( !file.exists()) {
            file.mkdirs();
        }
    }

    public boolean isExternalStorageWritable(){
        boolean available = false;

        String state = Environment.getExternalStorageState();
        if (Environment.MEDIA_MOUNTED.equals(state)){
            available = true;
        }

        return available;
    }

    public boolean isExternalStorageReadable(){
        boolean available = false;

        String state = Environment.getExternalStorageState();
        if (Environment.MEDIA_MOUNTED.equals(state) || Environment.MEDIA_MOUNTED_READ_ONLY.equals(state)){
            available = true;
        }

        return available;
    }

    public long getExternalFreeSpace(){
        File f = m_context.getExternalFilesDir(null);

        return f.getFreeSpace();
    }

    public long getExternalTotalSpace(){
        File f = m_context.getExternalFilesDir(null);

        return f.getTotalSpace();
    }

    public String getAppPathOnExternalStorage(){
        return m_context.getExternalFilesDir(null).getAbsolutePath();
    }

    public String getAppPathOnInternalStorage(){
        return m_context.getFilesDir().getAbsolutePath();
    }
    /**
     * Method to list files of a directory.
     * @param path Path to list files. If it is null it searches on the App's private external storage folder.
     * @return Returns the list of files of the given path on the external storage.
     */
    public String[] listDirectory(String path){
        File f = m_context.getExternalFilesDir(null);
        String initial_path;
        String[] list = f.list();

        // Check if ir is App's path or a custom path
        if (path == null){
            initial_path = f.getAbsolutePath();
        }
        else{
            initial_path = path;
        }

        for(int i = 0; i < list.length; i++){
            list[i] = initial_path.concat("/").concat(list[i]);
        }

        return list;
    }

    public ArrayList<String> listFilesOrDirectories(String path, boolean listFiles){
        File f;
        String initial_path;
        String element;
        ArrayList<String> list = new ArrayList<String>();
        File[] f_list;

        // Check if ir is App's path or a custom path
        if (path == null){
            f = m_context.getExternalFilesDir(null);
            initial_path = f.getAbsolutePath();
        }
        else{
            initial_path = path;
            f = new File(path);
        }
        f_list = f.listFiles();

        if (listFiles){
            for( File file : f_list){
                if (file.isFile()){
                    element = initial_path.concat("/").concat(file.getName());
                    list.add(element);
                }
            }
        }
        else{
            for( File file : f_list){
                if (file.isDirectory()){
                    element = initial_path.concat("/").concat(file.getName());
                    list.add(element);
                }
            }
        }

        return list;
    }

    //Todo Method: Ver que onda con esto de savefile
    public boolean saveFile(String fileName, boolean publicFile){
        boolean success = false;
        File f;

        if (!publicFile){
            f = new File(m_context.getExternalFilesDir(null), fileName);
        }
        else{
            f = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DCIM), fileName);
        }
        success = f.mkdirs();

        return success;
    }

    //Todo Method: Ver que onda con esto de loadfile
    public void loadFile(){}

    public boolean deleteFile(String fileName){
        File f = new File(fileName);

        return  f.delete();
    }
}
