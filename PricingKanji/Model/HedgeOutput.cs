﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PricingKanji.Model
{
    class HedgeOutput
    {
        public double portfolioValue;
        public double optionValue;
        public Dictionary<string, double> composition;
    }
}
