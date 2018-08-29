using ArgeoSync.Model;
using System.ComponentModel.DataAnnotations;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace ArgeoSync.ViewModel
{
    public class PointOfInterestViewModel : EntityViewModel
    {
        [Required]
        public override EntityType Type
        {
            get { return EntityType.PointOfInterest; }
        }


        private GraphicsViewModel mGraphics;
        [Required]
        public GraphicsViewModel Graphics
        {
            get { return mGraphics; }
            set
            {
                SetProperty(ref mGraphics, value);
                this.Image = this.Graphics.Image;
            }
        }

        public override ImageSource Image
        {
            get { return base.Image; }
            set
            {
                base.Image = value;
                this.Graphics.Image = value;
            }
        }
    }
}
