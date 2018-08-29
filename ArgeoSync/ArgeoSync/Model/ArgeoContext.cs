using System.Data.Entity;
using System.Data.SQLite;

namespace ArgeoSync.Model
{
    public class ArgeoContext : DbContext
    {
        public ArgeoContext(string filename)
        : base(new SQLiteConnection()
        {
            ConnectionString = new SQLiteConnectionStringBuilder()
            {
                DataSource = filename, ForeignKeys = true
            }.ConnectionString
        }, true)
        { }


        public ArgeoContext()
        : base("ArgeoContext") { }

        
        public DbSet<Plane> Planes { get; set; }

        public DbSet<CoreSample> CoreSamples { get; set; }

        public DbSet<TTARView> TTARViews { get; set; }

        public DbSet<PointOfInterest> PointsOfInterest { get; set; }

        public DbSet<Graphics> Graphics { get; set; }
    }
}
