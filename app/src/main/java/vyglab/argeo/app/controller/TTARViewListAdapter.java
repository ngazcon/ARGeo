package vyglab.argeo.app.controller;

import android.graphics.Color;
import android.support.v4.content.ContextCompat;
import android.widget.ArrayAdapter;
import android.view.LayoutInflater;
import android.view.ViewGroup;
import android.view.View;
import android.content.Context;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.List;

import vyglab.argeo.R;
import vyglab.argeo.app.model.TTARView;
import vyglab.argeo.app.view.FragmentTTARViewList;

/**
 * Created by root on 28/02/17.
 */

public class TTARViewListAdapter extends ArrayAdapter<TTARView> {
    // Pequeño workaround porq no pude desactivar el "setchecked" al hacer click.
    private FragmentTTARViewList m_fragment;

        public TTARViewListAdapter(Context context, List<TTARView> objects, FragmentTTARViewList fragment){
            // It passes 0 on the 2nd parameter because it will use its own layout
            super(context, 0, objects);

            m_fragment = fragment;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent){
            LayoutInflater inflater = (LayoutInflater) getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);

            // Ask if the view already exists or it is not inflated yet
            if (null == convertView){
                convertView = inflater.inflate(R.layout.ttarview_list_item, parent, false);
            }

            // Inflate the view
            // Obtain references
            ImageView image = (ImageView) convertView.findViewById(R.id.image_view_ttarview_list_item_image);
            TextView name = (TextView) convertView.findViewById(R.id.textview_ttarview_list_item_name);

            // Current TTARView
            TTARView t = getItem(position);

            // Setup UI components
            //image.setImageResource(R.drawable.ic_menu_argeo_logo);
            image.setImageBitmap(t.getInitialView());
            name.setText(t.getName());

            if (m_fragment.getSelected() == position) {
                convertView.setBackgroundColor(ContextCompat.getColor(getContext(), R.color.colorAccentTransparent080));
            } else {
                convertView.setBackgroundColor(Color.TRANSPARENT);
            }

            return convertView;
        }
    }
