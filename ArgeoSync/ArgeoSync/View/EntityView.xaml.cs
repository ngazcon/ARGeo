using ArgeoSync.ViewModel;
using MahApps.Metro.SimpleChildWindow;
using Microsoft.Maps.MapControl.WPF;
using Microsoft.Maps.MapControl.WPF.Design;
using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace ArgeoSync.View
{
    /// <summary>
    /// Interaction logic for PointOfInterestView.xaml
    /// </summary>
    public partial class EntityView : ChildWindow
    {
        private LocationConverter locConverter = new LocationConverter();
        private MapLayer layer = new MapLayer();
        private Dictionary<string, Image> images = new Dictionary<string, Image>();

        public EntityViewModel ViewModel
        {
            get { return this.DataContext as EntityViewModel; }
            set { this.DataContext = value; }
        }

        public EntityView()
        {
            InitializeComponent();

            //Set focus on the map
            myMap.Focus();

            //Add the image layer to the map
            myMap.Children.Add(layer);
        }

        private void ViewModel_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Latitude" || e.PropertyName == "Longitude" ||
                e.PropertyName == "Image")
            {
                removePointOfInterest(this.ViewModel);
                addPointOfInterest(this.ViewModel);
            }
        }

        private void addPointOfInterest(EntityViewModel entity)
        {
            //Add the image to the defined map layer
            if (!images.ContainsKey(entity.Id))
            {
                Image image  = new Image();
                image.Height = 50;
                image.Source = entity.Image;

                //Center the image around the location specified
                PositionOrigin position = PositionOrigin.Center;
                layer.AddChild(image, new Location(entity.Latitude, entity.Longitude), position);
                images.Add(entity.Id, image);
            }         
        }

        private void removePointOfInterest(EntityViewModel pointOfInterest)
        {
            Image image = null;
            if (images.TryGetValue(pointOfInterest.Id, out image))
            {
                images.Remove(pointOfInterest.Id);
                //Remove the image to the defined map layer
                layer.Children.Remove(image);
            }
        }

        private void myMap_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            e.Handled = true;
            // Creates a location for a single polygon point and adds it to
            // the polygon's point location list.
            Point mousePosition = e.GetPosition(myMap);

            //Convert the mouse coordinates to a location on the map
            Location location = myMap.ViewportPointToLocation(mousePosition);

            this.ViewModel.Latitude  = location.Latitude;
            this.ViewModel.Longitude = location.Longitude;
        }

        private void ChildWindow_DataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            if (e.OldValue as EntityViewModel != null)
            {
                (e.OldValue as EntityViewModel).PropertyChanged -= ViewModel_PropertyChanged;
            }

            if (e.NewValue as EntityViewModel != null)
            {
                (e.NewValue as EntityViewModel).PropertyChanged += ViewModel_PropertyChanged;
            }            
        }

        private void clearLayer()
        {
            foreach (var entry in this.images)
            {
                this.layer.Children.Remove(entry.Value);
            }
            this.images.Clear();
        }

        private void ChildWindow_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            this.clearLayer();
        }

        private void ChildWindow_IsOpenChanged(object sender, RoutedEventArgs e)
        {
            this.clearLayer();

            if (this.IsOpen)
            {
                this.addPointOfInterest(this.ViewModel);
            }
        }
    }
}
