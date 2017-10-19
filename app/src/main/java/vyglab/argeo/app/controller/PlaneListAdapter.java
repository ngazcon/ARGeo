package vyglab.argeo.app.controller;

import android.content.Context;
import android.graphics.Color;
import android.support.v4.content.ContextCompat;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import java.util.List;

import vyglab.argeo.R;
import vyglab.argeo.app.model.Plane;
import vyglab.argeo.app.view.FragmentPlaneList;

/**
 * Created by ngazcon on 09/03/2017.
 */

public class PlaneListAdapter extends ArrayAdapter<Plane> {
    // Pequeño workaround porq no pude desactivar el "setchecked" al hacer click.
    private FragmentPlaneList m_fragment;

    public PlaneListAdapter(Context context, List<Plane> objects, FragmentPlaneList fragment){
        // It passes 0 on the 2nd parameter because it will use its own layout
        super(context, 0, objects);

        m_fragment = fragment;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent){
        LayoutInflater inflater = (LayoutInflater) getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);

        // Ask if the view already exists or it is not inflated yet
        if (null == convertView){
            convertView = inflater.inflate(R.layout.plane_list_item, parent, false);
        }

        // Inflate the view
        // Obtain references
        TextView name = (TextView) convertView.findViewById(R.id.textview_plane_item_name);
        TextView description = (TextView) convertView.findViewById(R.id.textview_plane_item_desc);

        // Current TTARView
        Plane plane = getItem(position);

        // Setup UI components
        //image.setImageResource(R.drawable.ic_menu_argeo_logo);
        name.setText(plane.getName());
        description.setText(plane.getDescription());

        if (m_fragment.getSelected() == position) {
            convertView.setBackgroundColor(ContextCompat.getColor(getContext(), R.color.colorAccentTransparent080));
        } else {
            convertView.setBackgroundColor(Color.TRANSPARENT);
        }

        return convertView;
    }
}