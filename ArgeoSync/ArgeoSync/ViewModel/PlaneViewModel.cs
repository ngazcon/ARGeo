using ArgeoSync.Model;
using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;

namespace ArgeoSync.ViewModel
{
    public class PlaneViewModel : EntityViewModel
    {
        [Required]
        public override EntityType Type
        {
            get { return EntityType.Plane; }
        }

        private double mStrike;
        [Required]
        public double Strike
        {
            get { return mStrike; }
            set { SetProperty(ref mStrike, value); }
        }

        private double mDip;
        [Required]
        public double Dip
        {
            get { return mDip; }
            set { SetProperty(ref mDip, value); }
        }

        private double mSize = 4;
        [Required]
        public double Size
        {
            get { return mSize; }
            set { SetProperty(ref mSize, value); }
        }

        private double mThickness = 1;
        [Required]
        public double Thickness
        {
            get { return mThickness; }
            set { SetProperty(ref mThickness, value); }
        }

        private double mVirtualOrientation;
        [Required]
        public double VirtualOrientation
        {
            get { return mVirtualOrientation; }
            set { SetProperty(ref mVirtualOrientation, value); }

        }
        private bool mShowVirtualOrientation = false;
        [Required]
        public bool ShowVirtualOrientation
        {
            get { return mShowVirtualOrientation; }
            set { SetProperty(ref mShowVirtualOrientation, value); }
        }

        public PlaneViewModel() : base()
        {
            //Define the URI location of the image
            BitmapImage myBitmapImage = new BitmapImage();
            myBitmapImage.BeginInit();
            myBitmapImage.UriSource = new Uri(@"pack://application:,,,/Resources/icons/Plane.png");

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
