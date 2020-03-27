using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PricingLibrary.Utilities.MarketDataFeed;

namespace PricingKanji.Model
{
    public class HedgeState
    {
        internal DataFeed feed;
        internal double portfolioValue;
        internal double optionValue;
        public Dictionary<string, double> composition;
        public Dictionary<string, double> getAssetValues()
        {
            // returns the value of each asset in the domestic currency
            double value = 0;
            Dictionary<string, double> values = new Dictionary<string, double>(composition.Count);
            foreach (var index in composition.Keys)
            {
                value = composition[index] * (double)feed.PriceList[index];
                values.Add(index, value);
            }
            return values;
        }
        public Dictionary<string, double> getAssetParts()
        {
            double value = 0;
            Dictionary<string, double> parts = new Dictionary<string, double>(composition.Count);
            foreach (var index in composition.Keys)
            {
                value = composition[index] * (double)feed.PriceList[index] / portfolioValue;
                parts.Add(index, value);
            }
            return parts;
        }
    }
}
