using ArgeoSync.Command;
using ArgeoSync.Model;
using SharpAdbClient;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.Data.Entity.Core;
using System.IO;
using System.Linq;
using System.Net;
using System.Reflection;
using System.Threading;
using System.Windows.Input;

namespace ArgeoSync.ViewModel
{
    public class MainWindowViewModel : ViewModelBase
    {
        public enum WorkingStatus
        {
            [Description("Wait")]
            Wait,

            [Description("New")]
            New,

            [Description("Edit")]
            Edit,
        };

        public ArgeoContext GetNewRemoteRepositoryInstance()
        {
            return new ArgeoContext(
                    System.IO.Path.GetTempPath() +
                    @"\ArgeoDB.db");
        }

        public ArgeoContext GetNewLocalRepositoryInstance()
        {
            return new ArgeoContext(
                @"C:\Users\juanmanuel\desktop\ArgeoDB.db");
        }

        ObservableCollection<EntityViewModel> mEntities =
            new ObservableCollection<EntityViewModel>();
        public ObservableCollection<EntityViewModel> Entities
        {
            get { return mEntities; }
        }

        ObservableCollection<EntityViewModel> mEntitiesToRemove =
            new ObservableCollection<EntityViewModel>();
        public ObservableCollection<EntityViewModel> EntitiesToRemove
        {
            get { return mEntitiesToRemove; }
        }

        private EntityViewModel mSelectedItem;
        public EntityViewModel SelectedItem
        {
            get { return mSelectedItem; }
            set { SetProperty(ref mSelectedItem, value); }
        }

        private bool mIsEntityWindowOpen;
        public bool IsEntityWindowOpen
        {
            get { return mIsEntityWindowOpen; }
            set { SetProperty(ref mIsEntityWindowOpen, value); }
        }

        private WorkingStatus mStatus;
        public WorkingStatus Status
        {
            get { return mStatus; }
            set
            {
                IsEntityWindowOpen = (value == WorkingStatus.Edit || value == WorkingStatus.New);
                SetProperty(ref mStatus, value);
            }
        }

        private readonly DelegateCommand mAddPointOfInterestCommand;
        public ICommand AddPointOfInterestCommand => mAddPointOfInterestCommand;

        private readonly DelegateCommand mSavePointOfInterestCommand;
        public ICommand SavePointOfInterestCommand => mSavePointOfInterestCommand;

        private readonly DelegateCommand mEditPointOfInterestCommand;
        public ICommand EditPointOfInterestCommand => mEditPointOfInterestCommand;

        private readonly DelegateCommand mAddPlaneCommand;
        public ICommand AddPlaneCommand => mAddPlaneCommand;

        private readonly DelegateCommand mAddCoreSampleCommand;
        public ICommand AddCoreSampleCommand => mAddCoreSampleCommand;

        private readonly DelegateCommand mAddTTARViewCommand;
        public ICommand AddTTARViewCommand => mAddTTARViewCommand;

        private readonly DelegateCommand mRemoveEntityCommand;
        public ICommand RemoveEntityCommand => mRemoveEntityCommand;

        private readonly DelegateCommand mPushDataBaseCommand;
        public ICommand PushDataBaseCommand => mPushDataBaseCommand;

        private readonly DelegateCommand mPullDataBaseCommand;
        public ICommand PullDataBaseCommand => mPullDataBaseCommand;

        private readonly DelegateCommand mExportCSVCommand;
        public ICommand ExportCSVCommand => mExportCSVCommand;

        private readonly DelegateCommand mImportCSVCommand;
        public ICommand ImportCSVCommand => mImportCSVCommand;

        public MainWindowViewModel()
        {
            this.Entities.CollectionChanged += Entities_CollectionChanged;

            this.mPullDataBaseCommand = new DelegateCommand(OnPullDataBase, CanPullDataBase);
            this.mPushDataBaseCommand = new DelegateCommand(OnPushDataBase, CanPushDataBase);

            this.mRemoveEntityCommand = new DelegateCommand(OnRemoveEntity, CanRemoveEntity);
            this.mAddPointOfInterestCommand = new DelegateCommand(OnAddPointOfInterest, CanAddPointOfInterest);
            this.mAddCoreSampleCommand = new DelegateCommand(OnAddCoreSample, CanAddCoreSample);
            this.mAddPlaneCommand = new DelegateCommand(OnAddPlane, CanAddPlane);
            this.mAddTTARViewCommand = new DelegateCommand(OnAddTTARView, CanAddTTARView);
            this.mExportCSVCommand = new DelegateCommand(OnExportCSV, CanExportCSV);
            this.mImportCSVCommand = new DelegateCommand(OnImportCSV, CanImportCSV);

            this.mSavePointOfInterestCommand = new DelegateCommand(OnSavePointOfInterest, CanSavePointOfInterest);
            this.mEditPointOfInterestCommand = new DelegateCommand(OnEditPointOfInterest, CanEditPointOfInterest);

            if (!File.Exists(@"C:\Users\juanmanuel\desktop\ArgeoDB.db"))
            {
                this.GenerateDBFile();
            }

            this.LoadDataBase();
        }

        private void GenerateDBFile()
        {
            if (File.Exists(@"C:\Users\juanmanuel\desktop\ArgeoDB.db"))
            {
                File.Delete(@"C:\Users\juanmanuel\desktop\ArgeoDB.db");
            }

            string executableLocation = Path.GetDirectoryName(
                Assembly.GetExecutingAssembly().Location);
            File.Copy(executableLocation + @"\Resources\db\ArgeoDB.db", @"C:\Users\juanmanuel\desktop\ArgeoDB.db");
        }

        private void Entities_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            switch (e.Action)
            {
                case System.Collections.Specialized.NotifyCollectionChangedAction.Add:
                    this.SelectedItem = e.NewItems[e.NewItems.Count - 1] as EntityViewModel;
                    break;
            }
        }

        private void OnImportCSV(object commandParameter)
        {
            List<string> columns = new List<string>();
            Dictionary<string, List<string>> values = new Dictionary<string, List<string>>();
            ArgeoContext localRepository = GetNewLocalRepositoryInstance();

            // Import Core Sample
            Helper.Importer.ImportCSVToDataBase(
                out columns,
                out values,
                @"C:\Users\juanmanuel\desktop\coresamples.csv");

            int count = values.First().Value.Count;
            for (int i = 0; i < count; i++)
            {
                CoreSample model = new CoreSample
                {
                    _Id = Int64.Parse(values["_Id"][i]),
                    Id = values["Id"][i],
                    Description = values["Description"][i],
                    Longitude = Double.Parse(values["Longitude"][i]),
                    Latitude = Double.Parse(values["Latitude"][i]),
                    Height = Double.Parse(values["Height"][i]),
                    BottomRadius = Double.Parse(values["BottomRadius"][i]),
                    TopRadius = Double.Parse(values["TopRadius"][i]),
                    Length = Double.Parse(values["Length"][i]),
                    Slices = Double.Parse(values["Slices"][i]),
                    Timestamp = Int64.Parse(values["Timestamp"][i]),
                };

                CoreSample coresample = localRepository.CoreSamples.Find(model._Id);
                if (coresample == null || model.Timestamp > coresample.Timestamp)
                {
                    localRepository.CoreSamples.Add(model);
                }
            }


            // Import TTAR Views
            Helper.Importer.ImportCSVToDataBase(
                out columns,
                out values,
                @"C:\Users\juanmanuel\desktop\ttarviews.csv");

            count = values.First().Value.Count;
            for (int i = 0; i < count; i++)
            {
                TTARView model = new TTARView
                {
                    _Id = Int64.Parse(values["_Id"][i]),
                    Id = values["Id"][i],
                    Description = values["Description"][i],
                    Longitude = Double.Parse(values["Longitude"][i]),
                    Latitude = Double.Parse(values["Latitude"][i]),
                    Height = Double.Parse(values["Height"][i]),
                    Pitch = Double.Parse(values["Pitch"][i]),
                    Roll = Double.Parse(values["Roll"][i]),
                    Yaw = Double.Parse(values["Yaw"][i]),
                    BitmapFrame = Convert.FromBase64String(values["BitmapFrame"][i]),
                    BitmapView = Convert.FromBase64String(values["BitmapView"][i]),
                    Timestamp = Int64.Parse(values["Timestamp"][i]),
                };

                TTARView ttarview = localRepository.TTARViews.Find(model._Id);
                if (ttarview == null || model.Timestamp > ttarview.Timestamp)
                {
                    localRepository.TTARViews.Add(model);
                }
            }

            // Import Planes
            Helper.Importer.ImportCSVToDataBase(
                out columns,
                out values,
                @"C:\Users\juanmanuel\desktop\planes.csv");

            count = values.First().Value.Count;
            for (int i = 0; i < count; i++)
            {
                Plane model = new Plane
                {
                    _Id = Int64.Parse(values["_Id"][i]),
                    Id = values["Id"][i],
                    Description = values["Description"][i],
                    Longitude = Double.Parse(values["Longitude"][i]),
                    Latitude = Double.Parse(values["Latitude"][i]),
                    Height = Double.Parse(values["Height"][i]),
                    Dip = Double.Parse(values["Dip"][i]),
                    ShowVirtualOrientation = Boolean.Parse(values["ShowVirtualOrientation"][i]),
                    Size = Double.Parse(values["Size"][i]),
                    Strike = Double.Parse(values["Strike"][i]),
                    Thickness = Double.Parse(values["Thickness"][i]),
                    VirtualOrientation = Double.Parse(values["VirtualOrientation"][i]),
                    Timestamp = Int64.Parse(values["Timestamp"][i]),
                };

                Plane plane = localRepository.Planes.Find(model._Id);
                if (plane == null || model.Timestamp > plane.Timestamp)
                {
                    localRepository.Planes.Add(model);
                }
            }

            // Import Pois
            Helper.Importer.ImportCSVToDataBase(
                out columns,
                out values,
                @"C:\Users\juanmanuel\desktop\pois.csv");

            count = values.First().Value.Count;
            for (int i = 0; i < count; i++)
            {
                PointOfInterest model = new PointOfInterest
                {
                    _Id = Int64.Parse(values["_Id"][i]),
                    Id = values["Id"][i],
                    Description = values["Description"][i],
                    Longitude = Double.Parse(values["Longitude"][i]),
                    Latitude = Double.Parse(values["Latitude"][i]),
                    Height = Double.Parse(values["Height"][i]),
                    IdGraphics = Int64.Parse(values["IdGraphics"][i]),
                    Timestamp = Int64.Parse(values["Timestamp"][i]),
                };

                PointOfInterest poi = localRepository.PointsOfInterest.Find(model._Id);
                if (poi == null || model.Timestamp > poi.Timestamp)
                {
                    localRepository.PointsOfInterest.Add(model);
                }
            }

            // Import Graphics
            Helper.Importer.ImportCSVToDataBase(
                out columns,
                out values,
                @"C:\Users\juanmanuel\desktop\graphics.csv");

            count = values.First().Value.Count;
            for (int i = 0; i < count; i++)
            {
                Graphics model = new Graphics
                {
                    _Id = Int64.Parse(values["_Id"][i]),
                    ImageId = values["ImageId"][i],
                    Image = Convert.FromBase64String(values["Image"][i]),
                    Height = Int64.Parse(values["Height"][i]),
                    Width = Int64.Parse(values["Width"][i]),
                    Type = values["Type"][i],
                    Timestamp = Int64.Parse(values["Timestamp"][i]),
                };
                Graphics graphics = localRepository.Graphics.Find(model._Id);
                if (graphics == null || model.Timestamp > graphics.Timestamp)
                {
                    localRepository.Graphics.Add(model);
                }
            }

            localRepository.SaveChanges();

            this.LoadDataBase();
        }

        private bool CanImportCSV(object commandParameter)
        {
            return true;
        }

        private void OnExportCSV(object commandParameter)
        {
            List<string> columns = new List<string>();
            List<List<string>> values = new List<List<string>>();
            ArgeoContext localRepository = GetNewLocalRepositoryInstance();

            // Export Core Sample
            PropertyInfo[] infos = typeof(CoreSample).GetProperties();
            foreach (PropertyInfo info in infos)
            {
                columns.Add(info.Name);
            }

            foreach (CoreSample model in localRepository.CoreSamples)
            {
                List<string> columnValues = new List<string>();
                foreach (PropertyInfo info in infos)
                {
                    object value = info.GetValue(model);
                    value = value is byte[] ? Convert.ToBase64String(value as byte[]) : value;
                    columnValues.Add(value.ToString());
                }
                values.Add(columnValues);
            }

            Helper.Exporter.ExportToCSVFile(
                columns,
                values,
                @"C:\Users\juanmanuel\desktop\coresamples.csv");


            // Export Points of interest
            columns.Clear();
            values.Clear();
            infos = typeof(PointOfInterest).GetProperties();
            foreach (PropertyInfo info in infos)
            {
                columns.Add(info.Name);
            }

            foreach (PointOfInterest model in localRepository.PointsOfInterest)
            {
                List<string> columnValues = new List<string>();
                foreach (PropertyInfo info in infos)
                {
                    object value = info.GetValue(model);
                    value = value is byte[] ? Convert.ToBase64String(value as byte[]) : value;
                    columnValues.Add(value.ToString());
                }
                values.Add(columnValues);
            }

            Helper.Exporter.ExportToCSVFile(
                columns,
                values,
                @"C:\Users\juanmanuel\desktop\pois.csv");

            // Export Graphics
            columns.Clear();
            values.Clear();
            infos = typeof(Graphics).GetProperties();
            foreach (PropertyInfo info in infos)
            {
                columns.Add(info.Name);
            }

            foreach (Graphics model in localRepository.Graphics)
            {
                List<string> columnValues = new List<string>();
                foreach (PropertyInfo info in infos)
                {
                    object value = info.GetValue(model);
                    value = value is byte[] ? Convert.ToBase64String(value as byte[]) : value;
                    columnValues.Add(value.ToString());
                }
                values.Add(columnValues);
            }

            Helper.Exporter.ExportToCSVFile(
                columns,
                values,
                @"C:\Users\juanmanuel\desktop\graphics.csv");

            // Export TTARViews
            columns.Clear();
            values.Clear();
            infos = typeof(TTARView).GetProperties();
            foreach (PropertyInfo info in infos)
            {
                columns.Add(info.Name);
            }

            foreach (TTARView model in localRepository.TTARViews)
            {
                List<string> columnValues = new List<string>();
                foreach (PropertyInfo info in infos)
                {
                    object value = info.GetValue(model);
                    value = value is byte[] ? Convert.ToBase64String(value as byte[]) : value;
                    columnValues.Add(value.ToString());
                }
                values.Add(columnValues);
            }

            Helper.Exporter.ExportToCSVFile(
                columns,
                values,
                @"C:\Users\juanmanuel\desktop\ttarviews.csv");

            // Export Planes
            columns.Clear();
            values.Clear();
            infos = typeof(Plane).GetProperties();
            foreach (PropertyInfo info in infos)
            {
                columns.Add(info.Name);
            }

            foreach (Plane model in localRepository.Planes)
            {
                List<string> columnValues = new List<string>();
                foreach (PropertyInfo info in infos)
                {
                    object value = info.GetValue(model);
                    value = value is byte[] ? Convert.ToBase64String(value as byte[]) : value;
                    columnValues.Add(value.ToString());
                }
                values.Add(columnValues);
            }

            Helper.Exporter.ExportToCSVFile(
                columns,
                values,
                @"C:\Users\juanmanuel\desktop\planes.csv");
        }

        private bool CanExportCSV(object commandParameter)
        {
            return true;
        }

        private void OnSavePointOfInterest(object commandParameter)
        {
            if (this.Status == WorkingStatus.New)
            {
                this.SelectedItem.Timestamp = DateTime.Now;
                this.Entities.Add(this.SelectedItem);
                this.SaveDataBase();
            }
            else if (this.Status == WorkingStatus.Edit)
            {
                this.SelectedItem.Timestamp = DateTime.Now;
                this.SaveDataBase();
            }

            this.Status = WorkingStatus.Wait;
        }

        private bool CanSavePointOfInterest(object commandParameter)
        {
            ValidationContext context = new ValidationContext(this, null, null);
            List<ValidationResult> result = new List<ValidationResult>();

            if (!Validator.TryValidateObject(this, context, result, true))
            {
                return false;
            }

            if (commandParameter != null)
            {
                context = new ValidationContext(commandParameter, null, null);
                result = new List<ValidationResult>();
                if (!Validator.TryValidateObject(commandParameter, context, result, true))
                {
                    return false;
                }
            }
            else
            {
                return false;
            }

            return true;
        }

        private void OnAddPlane(object commandParameter)
        {
            this.Status = WorkingStatus.New;
            this.SelectedItem = new PlaneViewModel();
        }

        private bool CanAddPlane(object commandParameter)
        {
            return true;
        }

        private void OnAddCoreSample(object commandParameter)
        {
            this.Status = WorkingStatus.New;
            this.SelectedItem = new CoreSampleViewModel();
        }

        private bool CanAddCoreSample(object commandParameter)
        {
            return true;
        }

        private void OnAddPointOfInterest(object commandParameter)
        {
            this.Status = WorkingStatus.New;
            this.SelectedItem = new PointOfInterestViewModel
            {
                Graphics = new GraphicsViewModel
                {
                    Width = 60,
                    Height = 60
                }
            };
        }

        private bool CanAddPointOfInterest(object commandParameter)
        {
            return true;
        }

        private void OnAddTTARView(object commandParameter)
        {
            this.Status = WorkingStatus.New;
            this.SelectedItem = new TTARViewViewModel();
        }

        private bool CanAddTTARView(object commandParameter)
        {
            return true;
        }

        private void OnRemoveEntity(object commandParameter)
        {
            this.EntitiesToRemove.Add(this.SelectedItem);
            this.Entities.Remove(this.SelectedItem);
            this.SaveDataBase();
        }

        private bool CanRemoveEntity(object commandParameter)
        {
            return (this.SelectedItem as EntityViewModel) != null;
        }

        private void OnEditPointOfInterest(object commandParameter)
        {
            this.Status = WorkingStatus.Edit;
        }

        private bool CanEditPointOfInterest(object commandParameter)
        {
            return (this.SelectedItem as EntityViewModel) != null;
        }

        private void OnPullDataBase(object commandParameter)
        {
            PullDataBase();
            SyncDataBase();
        }

        private bool CanPullDataBase(object commandParameter)
        {
            return true;
        }

        private void OnPushDataBase(object commandParameter)
        {
            PushDataBase();
        }

        private bool CanPushDataBase(object commandParameter)
        {
            return true;
        }

        public void SaveDataBase()
        {
            using (ArgeoContext context = GetNewLocalRepositoryInstance())
            {
                foreach (PointOfInterestViewModel poi in this.Entities.OfType<PointOfInterestViewModel>().Where(s => s._Id == 0))
                {
                    Graphics old = context.Graphics.Find(poi._Id);
                    if (old != null)
                    {
                        old.Height = poi.Graphics.Height;
                        old.Width = poi.Graphics.Width;
                        old.Type = poi.Graphics.Type;
                        old.Image = Helper.BitmapHelper.SaveImage(poi.Graphics.Image);
                        old.ImageId = poi.Graphics.ImageId;
                        old.Timestamp = Helper.DateTimeHelper.DateTimeToUnixTimeStamp(poi.Graphics.Timestamp);
                        context.SaveChanges();
                    }
                    else
                    {
                        Graphics model = new Graphics
                        {
                            Height = poi.Graphics.Height,
                            Width = poi.Graphics.Width,
                            Type = poi.Graphics.Type,
                            Image = Helper.BitmapHelper.SaveImage(poi.Graphics.Image),
                            ImageId = poi.Graphics.ImageId,
                            Timestamp = Helper.DateTimeHelper.DateTimeToUnixTimeStamp(poi.Graphics.Timestamp),
                        };
                        context.Graphics.Add(model);
                        // Need _Id obtained by autoincrement
                        context.SaveChanges();
                        poi.Graphics._Id = model._Id;
                    }
                }

                foreach (PointOfInterestViewModel poi in this.Entities.OfType<PointOfInterestViewModel>().Where(s => s._Id == 0))
                {
                    PointOfInterest old = context.PointsOfInterest.Find(poi._Id);
                    if (old != null)
                    {
                        old.Id = poi.Id;
                        old.Description = poi.Description;
                        old.Height = poi.Height;
                        old.Latitude = poi.Latitude;
                        old.Longitude = poi.Longitude;
                        old.IdGraphics = poi.Graphics._Id;
                        old.Timestamp = Helper.DateTimeHelper.DateTimeToUnixTimeStamp(poi.Timestamp);
                        context.SaveChanges();
                    }
                    else
                    {
                        PointOfInterest model = new PointOfInterest
                        {
                            Id = poi.Id,
                            Description = poi.Description,
                            Height = poi.Height,
                            Latitude = poi.Latitude,
                            Longitude = poi.Longitude,
                            IdGraphics = poi.Graphics._Id,
                            Timestamp = Helper.DateTimeHelper.DateTimeToUnixTimeStamp(poi.Timestamp)
                        };
                        context.PointsOfInterest.Add(model);
                        // Need _Id obtained by autoincrement
                        context.SaveChanges();
                        poi._Id = model._Id;
                    }
                }

                foreach (PlaneViewModel plane in this.Entities.OfType<PlaneViewModel>())
                {
                    Plane old = context.Planes.Find(plane._Id);
                    if (old != null)
                    {
                        old.Id = plane.Id;
                        old.Description = plane.Description;
                        old.Dip = plane.Dip;
                        old.Height = plane.Height;
                        old.Latitude = plane.Latitude;
                        old.Longitude = plane.Longitude;
                        old.ShowVirtualOrientation = plane.ShowVirtualOrientation;
                        old.Size = plane.Size;
                        old.Strike = plane.Strike;
                        old.Thickness = plane.Thickness;
                        old.VirtualOrientation = plane.VirtualOrientation;
                        old.Timestamp = Helper.DateTimeHelper.DateTimeToUnixTimeStamp(plane.Timestamp);
                        context.SaveChanges();
                    }
                    else
                    {
                        Plane model = new Plane
                        {
                            Id = plane.Id,
                            Description = plane.Description,
                            Dip = plane.Dip,
                            Height = plane.Height,
                            Latitude = plane.Latitude,
                            Longitude = plane.Longitude,
                            ShowVirtualOrientation = plane.ShowVirtualOrientation,
                            Size = plane.Size,
                            Strike = plane.Strike,
                            Thickness = plane.Thickness,
                            VirtualOrientation = plane.VirtualOrientation,
                            Timestamp = Helper.DateTimeHelper.DateTimeToUnixTimeStamp(plane.Timestamp)
                        };

                        context.Planes.Add(model);
                        // Need _Id obtained by autoincrement
                        context.SaveChanges();
                        plane._Id = model._Id;
                    }
                }

                foreach (TTARViewViewModel ttarview in this.Entities.OfType<TTARViewViewModel>())
                {
                    TTARView old = context.TTARViews.Find(ttarview._Id);
                    if (old != null)
                    {
                        old.Id = ttarview.Id;
                        old.Description = ttarview.Description;
                        old.BitmapFrame = Helper.BitmapHelper.SaveImage(ttarview.BitmapFrame);
                        old.BitmapView = Helper.BitmapHelper.SaveImage(ttarview.BitmapView);
                        old.Pitch = ttarview.Pitch;
                        old.Roll = ttarview.Roll;
                        old.Yaw = ttarview.Yaw;
                        old.Height = ttarview.Height;
                        old.Latitude = ttarview.Latitude;
                        old.Longitude = ttarview.Longitude;
                        old.Timestamp = Helper.DateTimeHelper.DateTimeToUnixTimeStamp(ttarview.Timestamp);
                        context.SaveChanges();
                    }
                    else
                    {
                        TTARView model = new TTARView
                        {
                            Id = ttarview.Id,
                            Description = ttarview.Description,
                            BitmapFrame = Helper.BitmapHelper.SaveImage(ttarview.BitmapFrame),
                            BitmapView = Helper.BitmapHelper.SaveImage(ttarview.BitmapView),
                            Pitch = ttarview.Pitch,
                            Roll = ttarview.Roll,
                            Yaw = ttarview.Yaw,
                            Height = ttarview.Height,
                            Latitude = ttarview.Latitude,
                            Longitude = ttarview.Longitude,
                            Timestamp = Helper.DateTimeHelper.DateTimeToUnixTimeStamp(ttarview.Timestamp)
                        };

                        context.TTARViews.Add(model);
                        // Need _Id obtained by autoincrement
                        context.SaveChanges();
                        ttarview._Id = model._Id;
                    }
                }


                foreach (CoreSampleViewModel coresample in this.Entities.OfType<CoreSampleViewModel>())
                {
                    CoreSample old = context.CoreSamples.Find(coresample._Id);
                    if (old != null)
                    {
                        old.Id = coresample.Id;
                        old.Description = coresample.Description;
                        old.Height = coresample.Height;
                        old.Latitude = coresample.Latitude;
                        old.Longitude = coresample.Longitude;
                        old.BottomRadius = coresample.BottomRadius;
                        old.TopRadius = coresample.TopRadius;
                        old.Length = coresample.Length;
                        old.Slices = coresample.Slices;
                        old.Timestamp = Helper.DateTimeHelper.DateTimeToUnixTimeStamp(coresample.Timestamp);
                        context.SaveChanges();
                    }
                    else
                    {
                        CoreSample model = new CoreSample
                        {
                            Id = coresample.Id,
                            Description = coresample.Description,
                            Height = coresample.Height,
                            Latitude = coresample.Latitude,
                            Longitude = coresample.Longitude,
                            BottomRadius = coresample.BottomRadius,
                            TopRadius = coresample.TopRadius,
                            Length = coresample.Length,
                            Slices = coresample.Slices,
                            Timestamp = Helper.DateTimeHelper.DateTimeToUnixTimeStamp(coresample.Timestamp)
                        };

                        context.CoreSamples.Add(model);
                        // Need _Id obtained by autoincrement
                        context.SaveChanges();
                        coresample._Id = model._Id;
                    }
                }

                foreach (PlaneViewModel entityToRemove in this.EntitiesToRemove.OfType<PlaneViewModel>())
                {
                    context.Planes.Remove(context.Planes.Find(entityToRemove._Id));
                }

                foreach (TTARViewViewModel entityToRemove in this.EntitiesToRemove.OfType<TTARViewViewModel>())
                {
                    context.TTARViews.Remove(context.TTARViews.Find(entityToRemove._Id));
                }

                foreach (PointOfInterestViewModel entityToRemove in this.EntitiesToRemove.OfType<PointOfInterestViewModel>())
                {
                    context.PointsOfInterest.Remove(context.PointsOfInterest.Find(entityToRemove._Id));
                }

                foreach (CoreSampleViewModel entityToRemove in this.EntitiesToRemove.OfType<CoreSampleViewModel>())
                {
                    context.CoreSamples.Remove(context.CoreSamples.Find(entityToRemove._Id));
                }

                foreach (GraphicsViewModel entityToRemove in this.EntitiesToRemove.OfType<GraphicsViewModel>())
                {
                    context.Graphics.Remove(context.Graphics.Find(entityToRemove._Id));
                }

                context.SaveChanges();
                this.EntitiesToRemove.Clear();
            }
        }

        public void LoadDataBase()
        {
            this.Entities.Clear();

            try
            {
                using (ArgeoContext context = GetNewLocalRepositoryInstance())
                {
                    foreach (Plane plane in context.Planes)
                    {
                        this.Entities.Add(new PlaneViewModel
                        {
                            Id = plane.Id,
                            _Id = plane._Id,
                            Description = plane.Description,
                            Latitude = plane.Latitude.Value,
                            Longitude = plane.Longitude.Value,
                            Height = plane.Height.Value,
                            Dip = plane.Dip.Value,
                            Strike = plane.Strike.Value,
                            Timestamp = Helper.DateTimeHelper.UnixTimeStampToDateTime(plane.Timestamp),
                        });
                    }

                    foreach (PointOfInterest poi in context.PointsOfInterest)
                    {
                        Graphics graphics = context.Graphics.Find(poi.IdGraphics);
                        this.Entities.Add(new PointOfInterestViewModel
                        {
                            Id = poi.Id,
                            _Id = poi._Id,
                            Description = poi.Description,
                            Latitude = poi.Latitude.Value,
                            Longitude = poi.Longitude.Value,
                            Height = poi.Height.Value,

                            Timestamp = Helper.DateTimeHelper.UnixTimeStampToDateTime(poi.Timestamp),

                            Graphics = new GraphicsViewModel()
                            {
                                _Id = graphics._Id,
                                ImageId = graphics.ImageId,
                                Height = graphics.Height,
                                Width = graphics.Width,
                                Type = graphics.Type,
                                Image = Helper.BitmapHelper.LoadImage(graphics.Image),
                                Timestamp = Helper.DateTimeHelper.UnixTimeStampToDateTime(graphics.Timestamp)
                            }
                        });
                    }

                    foreach (TTARView ttarview in context.TTARViews)
                    {
                        this.Entities.Add(new TTARViewViewModel
                        {
                            Id = ttarview.Id,
                            _Id = ttarview._Id,
                            Description = ttarview.Description,
                            Latitude = ttarview.Latitude.Value,
                            Longitude = ttarview.Longitude.Value,
                            Height = ttarview.Height.Value,
                            Roll = ttarview.Roll.Value,
                            Yaw = ttarview.Yaw.Value,
                            Pitch = ttarview.Pitch.Value,
                            Timestamp = Helper.DateTimeHelper.UnixTimeStampToDateTime(ttarview.Timestamp),
                            BitmapFrame = Helper.BitmapHelper.LoadImage(ttarview.BitmapFrame),
                            BitmapView = Helper.BitmapHelper.LoadImage(ttarview.BitmapView),
                        });
                    }

                    foreach (CoreSample coresample in context.CoreSamples)
                    {
                        this.Entities.Add(new CoreSampleViewModel
                        {
                            Id = coresample.Id,
                            _Id = coresample._Id,
                            Description = coresample.Description,
                            Latitude = coresample.Latitude.Value,
                            Longitude = coresample.Longitude.Value,
                            Height = coresample.Height.Value,
                            Slices = coresample.Slices.Value,
                            BottomRadius = coresample.BottomRadius.Value,
                            TopRadius = coresample.TopRadius.Value,
                            Length = coresample.Length.Value,
                            Timestamp = Helper.DateTimeHelper.UnixTimeStampToDateTime(coresample.Timestamp),
                        });
                    }
                }
            }
            catch (EntityException ex)
            {
                this.GenerateDBFile();
            }
        }

        public void SyncDataBase()
        {
            ArgeoContext remote = GetNewRemoteRepositoryInstance();
            ArgeoContext local = GetNewLocalRepositoryInstance();

            foreach (Plane plane in remote.Planes)
            {
                if (local.Planes.Find(plane._Id) == null)
                {
                    local.Planes.Add(new Plane(plane));
                }
                else
                {
                    if (local.Planes.Find(plane._Id).Timestamp > plane.Timestamp)
                    {
                        local.Planes.Add(new Plane(plane));
                    }
                }
            }

            foreach (Plane plane in local.Planes)
            {
                if (remote.Planes.Find(plane._Id) == null)
                {
                    remote.Planes.Add(new Plane(plane));
                }
                else
                {
                    if (remote.Planes.Find(plane._Id).Timestamp > plane.Timestamp)
                    {
                        remote.Planes.Add(new Plane(plane));
                    }
                }
            }

            foreach (CoreSample coresample in remote.CoreSamples)
            {
                if (local.CoreSamples.Find(coresample._Id) == null)
                {
                    local.CoreSamples.Add(new CoreSample(coresample));
                }
                else
                {
                    if (local.CoreSamples.Find(coresample._Id).Timestamp > coresample.Timestamp)
                    {
                        local.CoreSamples.Add(new CoreSample(coresample));
                    }
                }
            }

            foreach (CoreSample coresample in local.CoreSamples)
            {
                if (remote.CoreSamples.Find(coresample._Id) == null)
                {
                    remote.CoreSamples.Add(new CoreSample(coresample));
                }
                else
                {
                    if (remote.CoreSamples.Find(coresample._Id).Timestamp > coresample.Timestamp)
                    {
                        remote.CoreSamples.Add(new CoreSample(coresample));
                    }
                }
            }

            foreach (TTARView ttarview in remote.TTARViews)
            {
                if (local.TTARViews.Find(ttarview._Id) == null)
                {
                    local.TTARViews.Add(new TTARView(ttarview));
                }
                else
                {
                    if (local.TTARViews.Find(ttarview._Id).Timestamp > ttarview.Timestamp)
                    {
                        local.TTARViews.Add(new TTARView(ttarview));
                    }
                }
            }

            foreach (TTARView ttarview in local.TTARViews)
            {
                if (remote.TTARViews.Find(ttarview._Id) == null)
                {
                    remote.TTARViews.Add(new TTARView(ttarview));
                }
                else
                {
                    if (remote.TTARViews.Find(ttarview._Id).Timestamp > ttarview.Timestamp)
                    {
                        remote.TTARViews.Add(new TTARView(ttarview));
                    }
                }
            }

            foreach (PointOfInterest poi in remote.PointsOfInterest)
            {
                if (local.PointsOfInterest.Find(poi._Id) == null)
                {
                    local.PointsOfInterest.Add(new PointOfInterest(poi));
                }
                else
                {
                    if (local.PointsOfInterest.Find(poi._Id).Timestamp > poi.Timestamp)
                    {
                        local.PointsOfInterest.Add(new PointOfInterest(poi));
                    }
                }
            }

            foreach (PointOfInterest poi in local.PointsOfInterest)
            {
                if (remote.PointsOfInterest.Find(poi._Id) == null)
                {
                    remote.PointsOfInterest.Add(new PointOfInterest(poi));
                }
                else
                {
                    if (remote.PointsOfInterest.Find(poi._Id).Timestamp > poi.Timestamp)
                    {
                        remote.PointsOfInterest.Add(new PointOfInterest(poi));
                    }
                }
            }

            foreach (Graphics graphics in remote.Graphics)
            {
                if (local.Graphics.Find(graphics._Id) == null)
                {
                    local.Graphics.Add(new Graphics(graphics));
                }
                else
                {
                    if (local.Graphics.Find(graphics._Id).Timestamp > graphics.Timestamp)
                    {
                        local.Graphics.Add(new Graphics(graphics));
                    }
                }
            }

            foreach (Graphics graphics in local.Graphics)
            {
                if (remote.Graphics.Find(graphics._Id) == null)
                {
                    remote.Graphics.Add(new Graphics(graphics));
                }
                else
                {
                    if (remote.Graphics.Find(graphics._Id).Timestamp > graphics.Timestamp)
                    {
                        remote.Graphics.Add(new Graphics(graphics));
                    }
                }
            }

            local.SaveChanges();
            remote.SaveChanges();

            LoadDataBase();
        }

        void PullDataBase()
        {
            AdbServer server = new AdbServer();

            StartServerResult result = server.StartServer(
                Environment.GetEnvironmentVariable("ANDROID_HOME") +
                @"\platform-tools\adb.exe",
                restartServerIfNewer: false);

            var device = AdbClient.Instance.GetDevices().First();

            using (SyncService service = new SyncService(new AdbSocket(new IPEndPoint(IPAddress.Loopback, AdbClient.AdbServerPort)), device))
            using (Stream stream = File.OpenWrite(
                System.IO.Path.GetTempPath() +
                @"\ArgeoDB.db"))
            {
                service.Pull(
                    "/storage/self/primary/ArgeoDB.db",
                    stream,
                    null,
                    CancellationToken.None);
            }
        }

        void PushDataBase()
        {
            AdbServer server = new AdbServer();

            StartServerResult result = server.StartServer(
                Environment.GetEnvironmentVariable("ANDROID_HOME") +
                @"\platform-tools\adb.exe",
                restartServerIfNewer: false);

            var device = AdbClient.Instance.GetDevices().First();

            using (SyncService service = new SyncService(new AdbSocket(new IPEndPoint(IPAddress.Loopback, AdbClient.AdbServerPort)), device))
            using (Stream stream = File.OpenRead(@"C:\Users\juanmanuel\desktop\ArgeoDB.db"))
            {
                service.Push(
                    stream,
                    "/storage/self/primary/ArgeoDB.db",
                    444,
                    DateTime.Now,
                    null,
                    CancellationToken.None);
            }
        }
    }
}
