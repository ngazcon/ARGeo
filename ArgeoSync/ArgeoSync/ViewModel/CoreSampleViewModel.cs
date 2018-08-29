using ArgeoSync.Model;
using System;
using System.ComponentModel.DataAnnotations;
using System.Windows.Media.Imaging;

namespace ArgeoSync.ViewModel
{
    public class CoreSampleViewModel : EntityViewModel
    {
        [Required]
        public override EntityType Type
        {
            get { return EntityType.CoreSample; }
        }

        private double mLength = 100;
        [Required]
        public double Length
        {
            get { return mLength; }
            set { SetProperty(ref mLength, value); }
        }

        private double mBottomRadius = 20;
        [Required]
        public double BottomRadius
        {
            get { return mBottomRadius; }
            set { SetProperty(ref mBottomRadius, value); }
        }

        private double mTopRadius = 20;
        [Required]
        public double TopRadius
        {
            get { return mTopRadius; }
            set { SetProperty(ref mTopRadius, value); }
        }

        private double mSlices = 20;
        [Required]
        public double Slices
        {
            get { return mSlices; }
            set { SetProperty(ref mSlices, value); }
        }

        public CoreSampleViewModel() : base()
        {
            //Define the URI location of the image
            BitmapImage myBitmapImage = new BitmapImage();
            myBitmapImage.BeginInit();
            myBitmapImage.UriSource = new Uri(@"pack://application:,,,/Resources/icons/coresample.png");

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
    }
}
