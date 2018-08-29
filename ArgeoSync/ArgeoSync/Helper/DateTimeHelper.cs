using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ArgeoSync.Helper
{
    public static class DateTimeHelper
    {
        public static Int64 DateTimeToUnixTimeStamp(DateTime dateTime)
        {
            return (Int64)(dateTime.Subtract(new DateTime(1970, 1, 1))).TotalSeconds;
        }

        public static DateTime UnixTimeStampToDateTime(Int64 unixTimeStamp)
        {
            // Unix timestamp is seconds past epoch
            System.DateTime dtDateTime = new DateTime(1970, 1, 1, 0, 0, 0, 0, System.DateTimeKind.Utc);
            dtDateTime = dtDateTime.AddSeconds(unixTimeStamp).ToLocalTime();
            return dtDateTime;
        }

        public static DateTime JavaTimeStampToDateTime(Int64 javaTimeStamp)
        {
            // Java timestamp is milliseconds past epoch
            System.DateTime dtDateTime = new DateTime(1970, 1, 1, 0, 0, 0, 0, System.DateTimeKind.Utc);
            dtDateTime = dtDateTime.AddMilliseconds(javaTimeStamp).ToLocalTime();
            return dtDateTime;
        }
    }
}
