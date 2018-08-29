using ArgeoSync.Model;
using System;
using System.ComponentModel.DataAnnotations;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace ArgeoSync.ViewModel
{
    public abstract class EntityViewModel : ViewModelBase
    {
        private double mLatitude;
        [Required]
        public double Latitude
        {
            get { return mLatitude; }
            set { SetProperty(ref mLatitude, value); }
        }

        private double mLongitude;
        [Required]
        public double Longitude
        {
            get { return mLongitude; }
            set { SetProperty(ref mLongitude, value); }
        }

        private double mHeight;
        [Required]
        public double Height
        {
            get { return mHeight; }
            set { SetProperty(ref mHeight, value); }
        }

        private string mDescription;
        [StringLength(50)]
        [Required]
        public string Description
        {
            get { return mDescription; }
            set { SetProperty(ref mDescription, value); }
        }

        [Required]
        public abstract EntityType Type
        {
            get;
        }

        private ImageSource mImage;
        [Required]
        public virtual ImageSource Image
        {
            get { return mImage; }
            set { SetProperty(ref mImage, value); }
        }

        private DateTime mTimestamp;
        [Required]
        public DateTime Timestamp
        {
            get { return mTimestamp; }
            set { SetProperty(ref mTimestamp, value); }
        }

        private string mId;
        [Required]
        public string Id
        {
            get { return mId; }
            set { mId = value; }
        }

        private Int64 m_Id;
        [Required]
        public Int64 _Id
        {
            get { return m_Id; }
            set { m_Id = value; }
        }

        protected EntityViewModel()
        {
            this.Id = Guid.NewGuid().ToString();
        }
    }
}
