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
    public class Market
    {
        public static double r = 0.001;
        public static double r_usd = 0.001;
        public static double r_hkd = 0.001;
        public static double businessDdaysPerYear = 252.0;
        public List<DataFeed> pastFeeds;
        public List<DataFeed> feeds;
        public static bool FX;
        public Market(DateTime userDate, bool FX_)
        {
            FX = FX_;
            feeds = new List<DataFeed>();
            DataReader reader = new DataReader();
            List<DataFeed> data;
            if (FX)
            {
                data = reader.ReadDataFX(r_usd, r_hkd);
            }
            else
            {
                data = reader.ReadData();
            }
            foreach (DataFeed feed in data)
            {
                if (feed.Date.CompareTo(userDate) <= 0)
                {
                    feeds.Add(feed);
                }
            }
            pastFeeds = feeds;
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

        public void completeMarket(DateTime maturity, int estimationwindow)
        {
            List<DataFeed> simulatedFeeds = Market.simulateMarket(pastFeeds, maturity, estimationwindow);
            feeds = pastFeeds.Concat(simulatedFeeds).ToList();
        }

        // Returns a list of simulated datafeeds for a certain option starting from startdate
        public static List<DataFeed> simulateMarket(List<DataFeed> feeds, DateTime maturity, int estimationwindow)
        {
            WrapperClass wc = new WrapperClass();
            int nbDates = feeds.Count;
            List<DataFeed> estimationFeeds = feeds.GetRange(nbDates - estimationwindow, estimationwindow);
            double[] estimated_volatilities = Calibration.getVolatilities(estimationFeeds);
            double[,] estimated_correlation = Calibration.getCorrelations(estimationFeeds);
            double[] estimated_trend = Calibration.getTrend(estimationFeeds);
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
            wc.SimulMarket(t_in_years, matu_in_years, nbSimulatedDates + 1, spots, estimated_trend, estimated_volatilities, contigous_correlation, r, r_usd, r_hkd, FX);
            double[] path = wc.getPath(FX);
            DataFeed firstFeed = feeds.First();
            int size = firstFeed.PriceList.Count;
            List<string> names = firstFeed.PriceList.Keys.ToList();
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

        public List<DataFeed> KanjiFeeds(List<DataFeed> feeds, DateTime startdate, DateTime maturity, bool FX)
        {
            List<DataFeed> effective_feeds = new List<DataFeed>();
            foreach (DataFeed feed in feeds)
            {
                if (feed.Date.CompareTo(startdate) >= 0 && feed.Date.CompareTo(maturity) <= 0)
                {
                    effective_feeds.Add(feed);
                }
            }
            if (FX)
            {
                double t = 0;
                foreach (DataFeed feed in effective_feeds)
                {
                    feed.PriceList["USDEUR"] *= (decimal)Math.Exp(Market.r_usd * t);
                    feed.PriceList["HKDEUR"] *= (decimal)Math.Exp(Market.r_hkd * t);
                    t += 1.0 / businessDdaysPerYear;
                }
            }
            return effective_feeds;
        }

        public List<DataFeed> PreviousFeeds(List<DataFeed> feeds, DateTime startdate)
        {
            List<DataFeed> previous_feeds = new List<DataFeed>();
            foreach (DataFeed feed in feeds)
            {
                if (feed.Date.CompareTo(startdate) < 0)
                {
                    previous_feeds.Add(feed);
                }
            }
            return previous_feeds;
        }

        public List<DataFeed> domesticFeeds(List<DataFeed> feeds)
        {
            List<DataFeed> domestic_feeds = new List<DataFeed>(feeds);
            double t = 0;
            foreach (DataFeed feed in domestic_feeds)
            {
                feed.PriceList["USDEUR"] /= (decimal)Math.Exp(Market.r_usd * t);
                feed.PriceList["HKDEUR"] /= (decimal)Math.Exp(Market.r_hkd * t);
                feed.PriceList["S&P 500"] /= feed.PriceList["USDEUR"];
                feed.PriceList["HANG SENG INDEX"] /= feed.PriceList["HKDEUR"];
                t += 1.0 / businessDdaysPerYear;
            }
            return domestic_feeds;
        }
    }
}
