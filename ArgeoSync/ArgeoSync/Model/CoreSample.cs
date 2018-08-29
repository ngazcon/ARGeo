using System.ComponentModel.DataAnnotations.Schema;

namespace ArgeoSync.Model
{
    [Table("coresample")]
    public class CoreSample : Entity
    {
        [Column("length")]
        public double? Length { get; set; }

        [Column("bottom_radius")]
        public double? BottomRadius { get; set; }

        [Column("top_radius")]
        public double? TopRadius { get; set; }

        [Column("slices")]
        public double? Slices { get; set; }

        public CoreSample()
        { }

        public CoreSample(CoreSample coreSample) : base(coreSample)
        {
            this.BottomRadius = coreSample.BottomRadius;
            this.TopRadius = coreSample.TopRadius;
            this.Length = coreSample.Length;
            this.Slices = coreSample.Slices;
        }
    }
}
