using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace ArgeoSync.Model
{
    public abstract class Entity
    {
        [Column("description")]
        public string Description { get; set; }

        [Column("lat")]
        public double? Latitude { get; set; }

        [Column("long")]
        public double? Longitude { get; set; }

        [Column("height")]
        public double? Height { get; set; }

        [Column("_id")]
        [Key]
        public Int64 _Id { get; set; }

        [Column("id")]
        public string Id { get; set; }

        [Column("timestamp")]
        public Int64 Timestamp { get; set; }

        protected Entity()
        { }

        protected Entity(Entity entity)
        {
            this._Id = entity._Id;
            this.Id = entity.Id;
            this.Description = entity.Description;
            this.Latitude = entity.Latitude;
            this.Longitude = entity.Longitude;
            this.Height = entity.Height;
            this.Timestamp = entity.Timestamp;
        }

    }
}
