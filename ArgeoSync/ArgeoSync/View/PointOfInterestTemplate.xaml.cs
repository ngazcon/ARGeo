using ArgeoSync.ViewModel;
using MahApps.Metro.Controls;
using System;
using System.Windows.Controls;
using System.Windows.Media.Imaging;

namespace ArgeoSync.View
{
    /// <summary>
    /// Interaction logic for PointOfInterestTemplate.xaml
    /// </summary>
    public partial class PointOfInterestTemplate : UserControl
    {
        public PointOfInterestTemplate()
        {
            InitializeComponent();
        }

        private void FlipView_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            var flipview = ((FlipView)sender);
            PointOfInterestViewModel viewModel = this.DataContext as PointOfInterestViewModel;

            switch (flipview.SelectedIndex)
            {
                case 0:
                    flipview.BannerText = "Star";
                    viewModel.Graphics.ImageId = "star";
                    setImage("pack://application:,,,/Resources/icons/032.png");
                    break;
                case 1:
                    flipview.BannerText = "Map marker";
                    viewModel.Graphics.ImageId = "map-marker";
                    setImage("pack://application:,,,/Resources/icons/map-marker.png");
                    break;
                case 2:
                    flipview.BannerText = "Flag";
                    viewModel.Graphics.ImageId = "flag";
                    setImage("pack://application:,,,/Resources/icons/flag-variant.png");
                    break;
            }
        }

        private void setImage(string path)
        {
            //Define the URI location of the image
            BitmapImage myBitmapImage = new BitmapImage();
            myBitmapImage.BeginInit();
            myBitmapImage.UriSource = new Uri(path);

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

            PointOfInterestViewModel viewModel = this.DataContext as PointOfInterestViewModel;
            viewModel.Image = myBitmapImage;
        }
    }
}
