package vyglab.argeo.app.model;

import android.graphics.Bitmap;

import org.opencv.android.Utils;
import org.opencv.core.Core;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.Point;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;

/**
 * Created by ngazcon on 12/07/2017.
 */

public class SketchFilter {

    private Bitmap m_bitmap_original;
    private Bitmap m_bitmap_original_rectangle;
    private Bitmap m_bitmap_input;
    private Bitmap m_bitmap_output;

    private Mat m_mat_original;
    private Mat m_mat_output;

    private boolean m_draw_rectangle_on_output = true;

    // Upper left rectangle corner
    private int m_rectangle_col = 200;
    private int m_rectangle_row = 200;
    // Rectangle width and height, they must be lesser than image dimensions
    private int m_rectangle_w = 100;
    private int m_rectangle_h = 100;
    // Variables used for rectangle resize
    private int m_stored_rectangle_w;
    private int m_stored_rectangle_h;
    private int m_initial_dw;
    private int m_initial_dh;

    // By default, white rectangle using the following thickness
    private int m_rectangle_thickness = 5;
    private int m_rectangle_color_r = 255;
    private int m_rectangle_color_g = 255;
    private int m_rectangle_color_b = 255;

    // Cartoon parameters
    private int m_pyr_steps = 2;
    private int m_median_filter_steps = 7;

    // Skeleton parameters
    private boolean m_do_skeleton = true;
    private int m_skeleton_kernel = 3;
    private int m_skeleton_final_kernel = 3;

    public SketchFilter() {
    }

    //region Filter Parameters Methods
    public void setDrawRectangleOnOutput(boolean draw) {
        m_draw_rectangle_on_output = draw;
    }
    //endregion

    public void setNewBitmap(Bitmap image) {
        m_bitmap_original = image.copy(image.getConfig(), true);
        m_bitmap_original_rectangle = image.copy(image.getConfig(), true);

        if ( m_mat_original != null ) {
            m_mat_original.release();
        }
        m_mat_original = new Mat();
        Utils.bitmapToMat(image, m_mat_original);
        Imgproc.cvtColor(m_mat_original, m_mat_original, Imgproc.COLOR_RGBA2RGB); // Get rid of the alpha channel
    }

    public void setRectangleW(int w) {
        int max = m_bitmap_original.getWidth();

        if (w < max) {
            m_rectangle_w = w;
        } else {
            m_rectangle_w = max;
        }
    }

    public void setRectangleH(int h) {
        int max = m_bitmap_original.getHeight();

        if (h < max) {
            m_rectangle_h = h;
        } else {
            m_rectangle_h = max;
        }
    }

    public void startRectangleResize(int dw, int dh){
        m_stored_rectangle_w = m_rectangle_w;
        m_stored_rectangle_h = m_rectangle_h;

        m_initial_dw = dw;
        m_initial_dh = dh;
    }

    public void updateRectangleSize(int dw, int dh) {
        int w = (dw - m_initial_dw) + m_stored_rectangle_w;
        int h = (dh - m_initial_dh) + m_stored_rectangle_h;

        if ( w > 50) {
            setRectangleW(w);
        }

        if (h > 50) {
            setRectangleH(h);
        }
    }

    public Bitmap getRectangleBitmap() {
        return m_bitmap_original_rectangle;
    }

    public Bitmap getOutputBitmap() {
        return  m_bitmap_output;
    }

    private void setRectangleDimension(int center_row, int center_col) {
        // Se asume que el ancho y alto del rectangulo son menores a las dimensiones de la imagen
        int cols = m_mat_original.cols();
        int rows = m_mat_original.rows();

        m_rectangle_col = center_col - m_rectangle_w / 2;
        m_rectangle_row = center_row - m_rectangle_h / 2;

        if (m_rectangle_col < 0 ) {
            m_rectangle_col = 0;
        } else if (m_rectangle_col + m_rectangle_w > cols) {
            m_rectangle_col = cols - m_rectangle_w;
        }

        if (m_rectangle_row < 0) {
            m_rectangle_row = 0;
        } else if (m_rectangle_row + m_rectangle_h > rows) {
            m_rectangle_row = rows - m_rectangle_h;
        }
    }

    public void updateRectangle(int center_row, int center_col) {
        //TODO arreglar este harcodeo agresivo
        center_row = center_row * 616 / 180; // center_row = center_row * alto_ttarview / alto_imageview
        center_col = center_col * 1024 / 300;
        //********

        Mat image = new Mat();
        setRectangleDimension(center_row, center_col);

        m_mat_original.copyTo(image);
        Imgproc.rectangle(image,
                new Point(m_rectangle_col , m_rectangle_row),
                new Point(m_rectangle_col + m_rectangle_w, m_rectangle_row + m_rectangle_h),
                new Scalar(m_rectangle_color_r, m_rectangle_color_g, m_rectangle_color_b),
                m_rectangle_thickness);
        Utils.matToBitmap(image, m_bitmap_original_rectangle);
        image.release();
    }

    public void drawRectangleOnOutput(Mat image) {
        Imgproc.rectangle(image,
                new Point(m_rectangle_col , m_rectangle_row),
                new Point(m_rectangle_col + m_rectangle_w, m_rectangle_row + m_rectangle_h),
                new Scalar(m_rectangle_color_r, m_rectangle_color_g, m_rectangle_color_b),
                m_rectangle_thickness);
    }

    public void cartoon() {
        /*boolean resize_col = m_mat_original.cols() % 2 != 0;
        boolean resize_rows = m_mat_original.rows() % 2 != 0;

        // Por la bizzarreada del pyrup, tiene que ser par
        if ( resize_col && resize_rows) {
            Imgproc.resize(m_mat_original, m_mat_original, new Size(m_mat_original.cols() + 1 , m_mat_original.rows() + 1));
        } else if ( resize_col){
            Imgproc.resize(m_mat_original, m_mat_original, new Size(m_mat_original.cols() + 1 , m_mat_original.rows()));
        } else if ( resize_rows ) {
            Imgproc.resize(m_mat_original, m_mat_original, new Size(m_mat_original.cols() , m_mat_original.rows() + 1));
        }*/

        if (m_mat_output != null) {
            m_mat_output.release();
        }
        m_mat_output = new Mat();
        m_mat_original.copyTo(m_mat_output);

        // 1a-- PyrDown the image
        for (int i = 0; i < m_pyr_steps; i++) {
            Imgproc.pyrDown(m_mat_output, m_mat_output);
        }

        // 1b-- Apply n-times a small bilateral filter (it is faster than a huge one)
        Mat filtrada = new Mat();
        m_mat_output.copyTo(filtrada);
        for (int i = 0; i < m_median_filter_steps; i++) {
            Imgproc.bilateralFilter(m_mat_output, filtrada, 9, 9, 7);
            filtrada.copyTo(m_mat_output);
        }

        // 1c-- PyrUp the image
        for (int i = 0; i < m_pyr_steps; i++) {
            Imgproc.pyrUp(m_mat_output, m_mat_output);
        }

        // 1d-- Set the Region of Interest (from now on "image")
        Rect r = new Rect(m_rectangle_col, m_rectangle_row, m_rectangle_w, m_rectangle_h);
        Mat roi = m_mat_output.submat(r);

        // 2-- Convert the image to grayscale
        Mat gray = new Mat();
        Imgproc.cvtColor(roi, gray, Imgproc.COLOR_RGB2GRAY);

        // 3-- Apply median blur to the image
        Imgproc.medianBlur(gray, gray, 7);

        // 4-- Create a mask of the grayscale image using adaptive thresholding
        Imgproc.adaptiveThreshold(gray, gray, 255, Imgproc.ADAPTIVE_THRESH_MEAN_C, Imgproc.THRESH_BINARY, 9, 2 );
        if (m_do_skeleton) {
            skeletonization(gray);
        }

        // 5-- Combine the image from step 1 with the mask of step 4
        Imgproc.cvtColor(gray, gray, Imgproc.COLOR_GRAY2RGB);
        Core.bitwise_and(roi, gray, roi);

        // 5-- Draw the rectangle
        if (m_draw_rectangle_on_output) {
            drawRectangleOnOutput(m_mat_output);
        }

        m_bitmap_output = Bitmap.createBitmap(m_mat_output.cols(), m_mat_output.rows(), Bitmap.Config.RGB_565);
        Utils.matToBitmap(m_mat_output, m_bitmap_output);
        roi.release();
        filtrada.release();
        gray.release();
    }

    private void skeletonization(Mat binary_image) {
        Core.bitwise_not(binary_image, binary_image);
        Mat skel = Mat.zeros(binary_image.rows(), binary_image.cols(), CvType.CV_8U);
        Mat element = Imgproc.getStructuringElement(Imgproc.MORPH_CROSS, new Size(m_skeleton_kernel, m_skeleton_kernel));
        boolean done = false;
        Mat eroded = new Mat();
        Mat temp = new Mat();

        while (!done) {
            Imgproc.erode(binary_image, eroded, element);
            Imgproc.dilate(eroded, temp, element);
            Core.subtract(binary_image, temp, temp);
            Core.bitwise_or(skel, temp, skel);
            eroded.copyTo(binary_image);

            done = Core.countNonZero(binary_image) == 0;
        }

        element.release();
        element = Imgproc.getStructuringElement(Imgproc.MORPH_CROSS, new Size(m_skeleton_final_kernel, m_skeleton_final_kernel));

        Imgproc.dilate(skel, skel, element);
        //Imgproc.erode(skel, skel, element);
        //Imgproc.dilate(skel, skel, element);

        Core.bitwise_not(skel, skel);
        skel.copyTo(binary_image);

        skel.release();
        eroded.release();
        temp.release();
        element.release();
    }
}
