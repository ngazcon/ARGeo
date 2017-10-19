package vyglab.argeo.app;

import android.widget.ArrayAdapter;
import android.view.LayoutInflater;
import android.view.ViewGroup;
import android.view.View;
import android.content.Context;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.List;
import java.util.Random;

import vyglab.argeo.R;
import vyglab.argeo.app.model.POI;


/**
 * Created by ngazcon on 05/09/2016.
 */
public class POIListAdapter extends ArrayAdapter<POI> {
    Random m_rand;

    public POIListAdapter(Context context, List<POI> objects){
        // It passes 0 on the 2nd parameter because it will use its own layout
        super(context, 0, objects);

        m_rand = new Random();
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent){
        LayoutInflater inflater = (LayoutInflater) getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);

        // Ask if the view already exists or it is not inflated yet
        if (null == convertView){
            convertView = inflater.inflate(R.layout.poi_list_item, parent, false);
        }

        // Inflate the view
        // Obtain references
        ImageView image = (ImageView) convertView.findViewById(R.id.poi_image);
        TextView name = (TextView) convertView.findViewById(R.id.poi_name);
        TextView type = (TextView) convertView.findViewById(R.id.poi_type);
        TextView distance = (TextView) convertView.findViewById(R.id.poi_distance);

        // Current POI
        POI poi = getItem(position);

        // Setup UI components
        image.setImageResource(R.drawable.ic_list_poi);
        name.setText(poi.getId());
        type.setText(poi.getDescription());
        int randomNum = m_rand.nextInt(100 + 1); // For now... it is a random number
        distance.setText(String.valueOf(randomNum).concat("m"));

        return convertView;
    }
}
