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
        public static double r = 0.001;
        public static double businessDdaysPerYear = 252.0;
        public List<DataFeed> feeds;

        public Market()
        {
            DataReader reader = new DataReader();
            List<DataFeed> data = reader.ReadData();
            feeds = data;
        }
        public Market(List<DataFeed> feeds_)
        {
            feeds = feeds_;
        }

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
            List<DataFeed> estimationFeeds = feeds.GetRange(nbDates - estimationwindow, estimationwindow);
            double[] estimated_volatilities = Calibration.Volatilities(estimationFeeds);
            double[,] estimated_correlation = Calibration.CorrMatrix(estimationFeeds);
            //double[] estimated_trend = Calibration.Trends(estimationFeeds);
            double[] estimated_trend = Calibration.Trends(estimationFeeds, estimated_volatilities);
            List<DataFeed> simulatedFeeds = new List<DataFeed>();
            DateTime lastDay = feeds.Last().Date;
            DateTime firstDay = feeds.First().Date;
            int nbSimulatedDates = DayCount.CountBusinessDays(lastDay, maturity);
            DateTime simulatedDate = lastDay;
            DataFeed feed;
            Dictionary<string, decimal> priceList = null;
            double matu_in_years = (DayCount.CountBusinessDays(firstDay, maturity)) / businessDdaysPerYear;
            double t_in_years = DayCount.CountBusinessDays(firstDay, lastDay) / businessDdaysPerYear;
            double[] contigous_correlation = new double[estimated_correlation.Length];
            int k = 0;
            foreach (double x in estimated_correlation)
            {
                contigous_correlation[k] = x;
                k++;
            }
            double[] spots = Market.marketSpots(feeds.Last());
            wc.SimulMarket(t_in_years, matu_in_years, nbSimulatedDates + 1, spots, estimated_trend, estimated_volatilities, contigous_correlation, r);
            double[] path = wc.getPath();
            DataFeed firstFeed = feeds.First();
            int size = firstFeed.PriceList.Count;
            List<string> names = new List<string>(firstFeed.PriceList.Keys);
            for (int i = 1; i <= nbSimulatedDates; i++)
            {
                simulatedDate = Utilities.AddBusinessDays(simulatedDate, 1);
                priceList = new Dictionary<string, decimal>();
                for (int j = 0; j < size; j++)
                {
                    priceList[names[j]] = (decimal)path[i * size + j];
                }
                feed = new DataFeed(simulatedDate, priceList);
                simulatedFeeds.Add(feed);
            }
            return simulatedFeeds;
        }

        public DataFeed getFeed(DateTime date)
        {
            foreach (DataFeed feed in feeds)
            {
                if (feed.Date == date)
                {
                    return feed;
                }
            }
            throw new Exception("Date introuvable sur le marché");
        }
    }
}
