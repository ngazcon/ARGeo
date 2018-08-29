using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace ArgeoSync.Model
{
    [Table("igraphic")]
    public class Graphics
    {
        [Column("type")]
        public string Type { get; set; }

        [Column("image_id")]
        public string ImageId { get; set; }

        [Column("image")]
        public byte[] Image { get; set; }

        [Column("height")]
        public Int64 Height { get; set; }

        [Column("width")]
        public Int64 Width { get; set; }

        [Column("_id")]
        [Key]
        public Int64 _Id { get; set; }

        [Column("timestamp")]
        public Int64 Timestamp { get; set; }

        public Graphics()
        { }

        public Graphics(Graphics graphics)
        {
            this._Id = graphics._Id;
            this.Height = graphics.Height;
            this.Width = graphics.Width;
            this.Type = graphics.Type;
            this.Image = graphics.Image;
            this.ImageId = graphics.ImageId;
            this.Timestamp = graphics.Timestamp;
        }
    }
}
