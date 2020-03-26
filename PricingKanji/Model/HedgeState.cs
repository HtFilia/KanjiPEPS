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
        public DataFeed feed;
        public double portfolioValue;
        public double optionValue;
        public Dictionary<string, double> composition;
        public Dictionary<string, double> getAssetValues()
        {
            // returns the value of each asset in the domestic currency
            double value = 0;
            Dictionary<string, double> values = new Dictionary<string, double>(composition.Count);
            double sum = 0;
            foreach (var index in composition.Keys)
            {
                value = composition[index] * (double)feed.PriceList[index] * 100 / portfolioValue;
                values.Add(index, value);
                sum += value;
            }
            values.Add("Riskless Asset", 100 - sum);
            return values;
        }
        public Dictionary<string, double> getAssetValuesEur()
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

    }
}
