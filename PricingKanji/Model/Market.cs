using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PricingLibrary.FinancialProducts;
using PricingLibrary.Utilities;
using PricingLibrary.Utilities.MarketDataFeed;
using Wrapper;

namespace PricingKanji.Model
{
    class Market
    {
        static double r = 0.001;
        static double businessDdaysPerYear = 252;


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
        public static List<DataFeed> simulate(List<DataFeed> feeds, DateTime maturity, int estimationwindow)
        {
            WrapperClass wc = new WrapperClass();
            int nbDates = feeds.Count;
            List<DataFeed> estimationFeeds = feeds.GetRange(nbDates-estimationwindow, estimationwindow);
            double[] estimated_volatilities = Calibration.Volatilities(estimationFeeds);
            double[,] estimated_correlation = Calibration.CorrMatrix(estimationFeeds);
            //double[] estimated_trend = Calibration.Trends(estimationFeeds);
            double[] estimated_trend = { 0.01, 0.02, 0.03 };
            List<DataFeed> simulatedFeeds = new List<DataFeed>();
            DateTime lastDay = feeds.Last().Date;
            DateTime firstDay = feeds.First().Date;
            int nbSimulatedDates = DayCount.CountBusinessDays(lastDay, maturity);
            DateTime simulatedDate = lastDay;
            DataFeed feed;
            Dictionary<string, decimal> priceList = null;
            double matu_in_years = (DayCount.CountBusinessDays(Utilities.AddBusinessDays(lastDay,1),maturity) ) / businessDdaysPerYear;
            double t_in_years = DayCount.CountBusinessDays(firstDay, lastDay) / businessDdaysPerYear;
            double[] contigous_correlation = new double[estimated_correlation.Length];
            Buffer.BlockCopy(estimated_correlation, 0, contigous_correlation, 0, estimated_correlation.Length);
            double[] spots = Market.marketSpots(feeds.Last());
            wc.SimulMarket(t_in_years, matu_in_years, nbSimulatedDates, spots, estimated_trend, estimated_volatilities, contigous_correlation, r);
            double[] path = wc.getPath();
            DataFeed firstFeed = feeds.First();
            int size = firstFeed.PriceList.Count;
            List<string> names = new List<string>(firstFeed.PriceList.Keys);
            for (int i = 0; i < nbSimulatedDates; i++)
            {
                simulatedDate = Utilities.AddBusinessDays(simulatedDate, 1);
                priceList = new Dictionary<string, decimal>();
                for (int j = 0; j < size ; j++)
                {
                    priceList[names[j]] = (decimal)path[i * size + j];
                }
                feed = new DataFeed(simulatedDate, priceList);
                simulatedFeeds.Add(feed);
            }
            return simulatedFeeds;
        }

    }
}
