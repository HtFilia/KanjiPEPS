using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Web;

namespace WebComponent.Models
{
    //DataContract for Serializing Data - required to serve in JSON format
    [DataContract]
    public class PastCoveragePoint
    {
        public PastCoveragePoint(int year, int month, int day, double y)
        {
            this.Date = new DateTime(year, month, day);
            this.Y = y;
        }

        //Explicitly setting the name to be used while serializing to JSON.
        [DataMember(Name = "x")]
        public Nullable<DateTime> Date { get; set; }

        //Explicitly setting the name to be used while serializing to JSON.
        [DataMember(Name = "y")]
        public Nullable<double> Y = null;
    }
}