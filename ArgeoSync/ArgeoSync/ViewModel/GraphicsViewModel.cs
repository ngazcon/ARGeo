using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace ArgeoSync.ViewModel
{
    public class GraphicsViewModel : ViewModelBase
    {
        private Int64 m_Id;
        [Required]
        public Int64 _Id
        {
            get { return m_Id; }
            set { SetProperty(ref m_Id, value); }

        }

        private string mImageId;
        [Required]
        public string ImageId
        {
            get { return mImageId; }
            set { SetProperty(ref mImageId, value); }
        }

        private string mType = "billboard";
        [Required]
        public string Type
        {
            get { return mType; }
            set { SetProperty(ref mType, value); }
        }

        private Int64 mWidth;
        [Required]
        public Int64 Width
        {
            get { return mWidth; }
            set { SetProperty(ref mWidth, value); }
        }

        private Int64 mHeight;
        [Required]
        public Int64 Height
        {
            get { return mHeight; }
            set { SetProperty(ref mHeight, value); }
        }

        private DateTime mTimestamp;
        [Required]
        public DateTime Timestamp
        {
            get { return mTimestamp; }
            set { SetProperty(ref mTimestamp, value); }
        }

        private ImageSource mImage;
        [Required]
        public ImageSource Image
        {
            get { return mImage; }
            set { SetProperty(ref mImage, value); }
        }        
    }
}
