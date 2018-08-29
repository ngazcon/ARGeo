using System;
using System.ComponentModel.DataAnnotations.Schema;

namespace ArgeoSync.Model
{
    [Table("poi")]
    public class PointOfInterest : Entity
    {
        [Column("graphics_id")]
        public Int64 IdGraphics { get; set; }


        public PointOfInterest() 
        { }

        public PointOfInterest(PointOfInterest pointOfInterest) : base(pointOfInterest)
        {
            this.IdGraphics = pointOfInterest.IdGraphics;
        }
    }
}
