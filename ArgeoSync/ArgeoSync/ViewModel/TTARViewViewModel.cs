using ArgeoSync.Command;
using ArgeoSync.Model;
using System;
using System.ComponentModel.DataAnnotations;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace ArgeoSync.ViewModel
{
    public class TTARViewViewModel : EntityViewModel
    {
        private readonly DelegateCommand mLoadBitmapFrameCommand;
        public ICommand LoadBitmapFrameCommand => mLoadBitmapFrameCommand;

        private readonly DelegateCommand mLoadBitmapViewCommand;
        public ICommand LoadBitmapViewCommand => mLoadBitmapViewCommand;

        [Required]
        public override EntityType Type
        {
            get { return EntityType.TTARView; }
        }

        private ImageSource mBitmapFrame;
        [Required]
        public ImageSource BitmapFrame
        {
            get { return mBitmapFrame; }
            set { SetProperty(ref mBitmapFrame, value); }
        }

        private ImageSource mBitmapView;
        [Required]
        public ImageSource BitmapView
        {
            get { return mBitmapView; }
            set { SetProperty(ref mBitmapView, value); }
        }

        private double mYaw;
        [Required]
        public double Yaw
        {
            get { return mYaw; }
            set { SetProperty(ref mYaw, value); }
        }

        private double mPitch = 90;
        [Required]
        public double Pitch
        {
            get { return mPitch; }
            set { SetProperty(ref mPitch, value); }
        }

        private double mRoll;
        [Required]
        public double Roll
        {
            get { return mRoll; }
            set { SetProperty(ref mRoll, value); }
        }

        public TTARViewViewModel() : base()
        {
            this.mLoadBitmapFrameCommand = new DelegateCommand(OnLoadBitmapFrame, CanLoadBitmapFrame);
            this.mLoadBitmapViewCommand  = new DelegateCommand(OnLoadBitmapView, CanLoadBitmapView);

            //Define the URI location of the image
            BitmapImage myBitmapImage = new BitmapImage();
            myBitmapImage.BeginInit();
            myBitmapImage.UriSource = new Uri(@"pack://application:,,,/Resources/icons/Camera.png");

            // To save significant application memory, set the DecodePixelWidth or  
            // DecodePixelHeight of the BitmapImage value of the image source to the desired 
            // height or width of the rendered image. If you don't do this, the application will 
            // cache the image as though it were rendered as its normal size rather then just 
            // the size that is displayed.
            // Note: In order to preserve aspect ratio, set DecodePixelWidth
            // or DecodePixelHeight but not both.
            //Define the image display properties
            myBitmapImage.DecodePixelHeight = 50;
            myBitmapImage.EndInit();

            this.Image = myBitmapImage;
        }

        private ImageSource LoadImage()
        {
            // Create OpenFileDialog 
            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();

            // Set filter for file extension and default file extension 
            dlg.DefaultExt = ".png";
            dlg.Filter = "JPEG Files (*.jpeg)|*.jpeg|PNG Files (*.png)|*.png|JPG Files (*.jpg)|*.jpg|GIF Files (*.gif)|*.gif";

            // Display OpenFileDialog by calling ShowDialog method 
            Nullable<bool> result = dlg.ShowDialog();
            // Get the selected file name and display in a TextBox 
            if (result == true)
            {
                // Open document 
                string filename = dlg.FileName;

                //Define the URI location of the image
                BitmapImage myBitmapImage = new BitmapImage();
                myBitmapImage.BeginInit();
                myBitmapImage.UriSource = new Uri(filename);

                // To save significant application memory, set the DecodePixelWidth or  
                // DecodePixelHeight of the BitmapImage value of the image source to the desired 
                // height or width of the rendered image. If you don't do this, the application will 
                // cache the image as though it were rendered as its normal size rather then just 
                // the size that is displayed.
                // Note: In order to preserve aspect ratio, set DecodePixelWidth
                // or DecodePixelHeight but not both.
                //Define the image display properties
                myBitmapImage.DecodePixelHeight = 50;
                myBitmapImage.EndInit();

                var transform = new ScaleTransform(1, -1, 0, 0);

                TransformedBitmap tb = new TransformedBitmap();
                tb.BeginInit();
                tb.Source    = myBitmapImage;
                tb.Transform = transform;
                tb.EndInit();

                return tb;
            }

            return null;
        }

        private void OnLoadBitmapFrame(object commandParameter)
        {
            this.BitmapFrame = LoadImage();
        }

        private bool CanLoadBitmapFrame(object commandParameter)
        {
            return true;
        }

        private void OnLoadBitmapView(object commandParameter)
        {
            this.BitmapView = LoadImage();
        }

        private bool CanLoadBitmapView(object commandParameter)
        {
            return true;
        }
    }

    
}
