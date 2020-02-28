using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace WebComponent.Models
{
    public class PriceFormModel
    {
        public int SampleNb { get; set; }
        public double Maturity { get; set; }
        public double InitialPrice { get; set; }
        public double Strike { get; set; }
        public double Volatility { get; set; }
        public double RiskFreeRate { get; set; }
    }
}