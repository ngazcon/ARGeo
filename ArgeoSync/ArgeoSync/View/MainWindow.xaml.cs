using System;
using System.Windows;
using System.Windows.Controls;
using System.Globalization;
using Microsoft.Maps.MapControl.WPF;
using Microsoft.Maps.MapControl.WPF.Design;
using System.Windows.Media.Imaging;
using MahApps.Metro.Controls;
using ArgeoSync.ViewModel;
using System.ComponentModel;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Input;
using System.Data;

namespace ArgeoSync.View
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : MetroWindow
    {
        private LocationConverter locConverter = new LocationConverter();
        private MapLayer layer                 = new MapLayer();

        private Dictionary<string, Image> images = new Dictionary<string, Image>();
        private Dictionary<Image, string> guids  = new Dictionary<Image, string>();

        public MainWindowViewModel ViewModel
        {
            get { return this.DataContext as MainWindowViewModel; }
        }

        public MainWindow()
        {
            InitializeComponent();

            //Set focus on the map
            myMap.Focus();

            //Add the image layer to the map
            myMap.Children.Add(layer);

            this.ViewModel.PropertyChanged += (object o, PropertyChangedEventArgs e) =>
            {
                if (e.PropertyName == "Status")
                {
                    switch(this.ViewModel.Status)
                    {
                        case MainWindowViewModel.WorkingStatus.Wait:
                            this.EditButton.IsEnabled   = true;
                            this.AddButton.IsEnabled    = true;
                            break;

                        case MainWindowViewModel.WorkingStatus.Edit:
                            this.EditButton.IsEnabled   = false;
                            this.AddButton.IsEnabled    = false; 
                            break;

                        case MainWindowViewModel.WorkingStatus.New:
                            this.EditButton.IsEnabled   = false;
                            this.AddButton.IsEnabled    = false;
                            break;
                    }                    
                }
                else if(e.PropertyName == "SelectedItem")
                {
                    if (this.ViewModel.SelectedItem != null)
                    {
                        myMap.Center = new Location(
                            this.ViewModel.SelectedItem.Latitude,
                            this.ViewModel.SelectedItem.Longitude);
                        myMap.ZoomLevel = 6;
                    }
                }
            };

            this.ViewModel.Entities.CollectionChanged += pointsOfInterests_CollectionChanged;
            this.ViewModel.LoadDataBase();
        }

        private void pointsOfInterests_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            switch(e.Action)
            {
                case System.Collections.Specialized.NotifyCollectionChangedAction.Add:
                    foreach (var addedPointOfInterest in e.NewItems)
                    {
                        EntityViewModel viewModel = addedPointOfInterest as EntityViewModel;
                        viewModel.PropertyChanged += (object s, PropertyChangedEventArgs ev) =>
                        {
                            if (ev.PropertyName == "Latitude" || ev.PropertyName == "Longitude")
                            {
                                removePointOfInterest(viewModel);
                                addPointOfInterest(viewModel);
                            }
                        };
                        addPointOfInterest(viewModel);
                    }
                    break;
                case System.Collections.Specialized.NotifyCollectionChangedAction.Remove:
                    foreach (var addedPointOfInterest in e.OldItems)
                    {
                        EntityViewModel viewModel = addedPointOfInterest as EntityViewModel;
                        removePointOfInterest(viewModel);
                    }
                    break;
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
                guids.Add(image, entity.Id);
            }            
        }

        private void removePointOfInterest(EntityViewModel pointOfInterest)
        {
            Image image = null;
            if (images.TryGetValue(pointOfInterest.Id, out image))
            {
                images.Remove(pointOfInterest.Id);
                guids.Remove(image);

                //Remove the image to the defined map layer
                layer.Children.Remove(image);
            }            
        }

        private void myMap_MouseDoubleClick(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            e.Handled = true;
            // Creates a location for a single polygon point and adds it to
            // the polygon's point location list.
            Point mousePosition = e.GetPosition(myMap);

            //Convert the mouse coordinates to a location on the map
            Location location = myMap.ViewportPointToLocation(mousePosition);

            Image image = e.OriginalSource as Image;
            if (image != null)
            {
                string id;
                if (guids.TryGetValue(image, out id))
                {
                    EntityViewModel selected =
                        this.ViewModel.Entities.FirstOrDefault(s => s.Id == id);
                    this.ViewModel.SelectedItem = selected;
                }                
            }
        }

        private void PointOfInterestView_Closing(object sender, CancelEventArgs e)
        {
            this.ViewModel.Status = MainWindowViewModel.WorkingStatus.Wait;
        }

        private void Row_DoubleClick(object sender, MouseButtonEventArgs e)
        {
            IInputElement element = e.MouseDevice.DirectlyOver;
            if (element != null && element is FrameworkElement)
            {
                if (((FrameworkElement)element).Parent is DataGridCell)
                {
                    var grid = sender as DataGrid;
                    if (grid != null && grid.SelectedItems != null && grid.SelectedItems.Count == 1)
                    {
                        var rowview = grid.SelectedItem as EntityViewModel;
                        if (rowview != null)
                        {
                            this.ViewModel.EditPointOfInterestCommand.Execute(null);
                        }
                    }
                }
                else if (((FrameworkElement)element).TemplatedParent is ContentPresenter)
                {
                    var selectedItem = (((FrameworkElement)element).TemplatedParent as ContentPresenter).Content;
                    if (selectedItem as EntityViewModel != null)
                    {
                        this.ViewModel.EditPointOfInterestCommand.Execute(null);
                    }
                }
            }
        }
    }
}
