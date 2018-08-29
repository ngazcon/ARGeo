using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ArgeoSync.Model
{
    public enum EntityType
    {
        [Description("Plane")]
        Plane,

        [Description("Point of interest")]
        PointOfInterest,

        [Description("TTAR view")]
        TTARView,

        [Description("Core sample")]
        CoreSample,
    }
}
