package vyglab.argeo.app.view;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.support.v4.app.DialogFragment;
import android.view.WindowManager;

import java.util.ArrayList;
import java.util.Arrays;

/**
 * Created by ngazcon on 23/10/2017.
 */

public class DialogChangeTTARVIEWResolution extends DialogFragment{

    public interface DialogChangeTTARVIEWResolutionListener {
        public void onDialogPositiveClick(DialogChangeTTARVIEWResolution dialog);
        public void onDialogNegativeClick(DialogChangeTTARVIEWResolution dialog);
    }

    // Use this instance of the interface to deliver action events
    private DialogChangeTTARVIEWResolutionListener m_listener;
    private int m_initial_w;
    private float m_ratio;
    private int m_selected_index;
    private ArrayList m_w_resolutions = new ArrayList<Integer>(Arrays.asList(256,640,1024,1408,2048,2560));

    public void setWAndRatio(int w, float ratio) {
        m_initial_w = w;
        m_ratio = ratio;
        m_selected_index = m_w_resolutions.indexOf(w);
    }

    public int getW() {
        return (int) m_w_resolutions.get(m_selected_index);
    }

    private AlertDialog createSingleListDialog() {
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());

        final CharSequence[] items = new CharSequence[6];

        for (int i = 0; i< m_w_resolutions.size(); i++){
            int w = (int) m_w_resolutions.get(i);
            int h = (int) ((float) w / m_ratio);
            items[i] = String.valueOf(w).concat("x").concat(String.valueOf(h));
        }

        builder.setTitle("Field Snapshot resolution")
                .setSingleChoiceItems(items, m_selected_index, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        m_selected_index = which;
                    }
                })
                .setPositiveButton("Ok", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int id) {
                        m_listener.onDialogPositiveClick(DialogChangeTTARVIEWResolution.this);
                    }
                })
                .setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int id) {
                    }
                });

        AlertDialog dialog = builder.create();

        // Hack to avoid showing the navigation buttons when the dialog is shown
        dialog.getWindow().addFlags(WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE);

        return dialog;
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        // Use the Builder class for convenient dialog construction

        return createSingleListDialog();
    }

    // Override the Fragment.onAttach() method to instantiate the NoticeDialogListener
    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
        // Verify that the host activity implements the callback interface
        try {
            // Instantiate the NoticeDialogListener so we can send events to the host
            m_listener = (DialogChangeTTARVIEWResolutionListener) activity;
        } catch (ClassCastException e) {
            // The activity doesn't implement the interface, throw exception
            throw new ClassCastException(activity.toString()
                    + " must implement NoticeDialogListener");
        }
    }
}
