using System;
using System.ComponentModel.DataAnnotations.Schema;

namespace ArgeoSync.Model
{
    [Table("plane")]
    public class Plane : Entity
    {
        [Column("virtual_orientation")]
        public double? VirtualOrientation { get; set; }

        [Column("dip")]
        public double? Dip { get; set; }

        [Column("strike")]
        public double? Strike { get; set; }

        [Column("size")]
        public double? Size { get; set; }

        [Column("thickness")]
        public double? Thickness { get; set; }

        [Column("show_virtual_orientation")]
        public bool? ShowVirtualOrientation { get; set; }

        public Plane()
        { }

        public Plane(Plane plane) : base(plane)
        {
            this.ShowVirtualOrientation = plane.ShowVirtualOrientation;
            this.Size = plane.Size;
            this.Strike = plane.Strike;
            this.Thickness = plane.Thickness;
            this.VirtualOrientation = plane.VirtualOrientation;
            this.Dip = plane.Dip;
        }
    }
}
