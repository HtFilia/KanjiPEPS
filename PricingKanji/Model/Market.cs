using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PricingLibrary.FinancialProducts;
using PricingLibrary.Utilities.MarketDataFeed;


namespace PricingKanji.Model
{
    class Market
    {
        // returns the spots for a certain datafeed in a certain DateTime
        public static double[] marketSpots(DataFeed market)
        {
            List<double> spots = new List<double>();
            foreach (double value in market.PriceList.Values)
            {
                spots.Add(value);
            }
            return spots.ToArray();
        }

        // Returns a list of simulated datafeeds for a certain option starting from startdate
        public static List<DataFeed> SimulateUnderlyingShares(Option option, DateTime startdate)
        {
            SimulatedDataFeedProvider simulator = new SimulatedDataFeedProvider();
            return simulator.GetDataFeed(option, startdate);
        }

    }
}
