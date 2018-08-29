using System.ComponentModel.DataAnnotations.Schema;

namespace ArgeoSync.Model
{
    [Table("ttarview")]
    public class TTARView : Entity
    {
        [Column("yaw")]
        public double? Yaw { get; set; }

        [Column("pitch")]
        public double? Pitch { get; set; }

        [Column("roll")]
        public double? Roll { get; set; }

        [Column("bitmap_frame")]
        public byte[] BitmapFrame { get; set; }

        [Column("bitmap_view")]
        public byte[] BitmapView { get; set; }

        public TTARView()
        { }

        public TTARView(TTARView ttarview) : base(ttarview)
        {
            this.BitmapFrame = ttarview.BitmapFrame;
            this.BitmapView = ttarview.BitmapView;
            this.Pitch = ttarview.Pitch;
            this.Roll = ttarview.Roll;
            this.Yaw = ttarview.Yaw;
        }
    }
}
