using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PricingKanji.Model;
using PricingLibrary.Utilities.MarketDataFeed;


namespace PricingKanji
{
    class TestWRE
    {
        static void Main(string[] args)
        {
            DateTime day = DateTime.Today;
            Dictionary<string, decimal> PriceList = new Dictionary<string, decimal>();
            DataFeed feed;
            List<DataFeed> feeds = new List<DataFeed>();

            PriceList.Add("Index1", 1);
            PriceList.Add("Index2", 4);
            PriceList.Add("Index3", 5);
            feed = new DataFeed(day, PriceList);
            feeds.Add(feed);

            PriceList = new Dictionary<string, decimal>();
            PriceList.Add("Index1", 2);
            PriceList.Add("Index2", 5);
            PriceList.Add("Index3", 6);
            feed = new DataFeed(day.AddDays(1), PriceList);
            feeds.Add(feed);

            PriceList = new Dictionary<string, decimal>();
            PriceList.Add("Index1", 16);
            PriceList.Add("Index2", 8);
            PriceList.Add("Index3", 9);
            feed = new DataFeed(day.AddDays(1), PriceList);
            feeds.Add(feed);

            double[] volatilities = Calibration.Volatilities(feeds);
            double[,] correlation = Calibration.CorrMatrix(feeds);

            System.Diagnostics.Debug.WriteLine("***Volatilities***");
            for (int i = 0; i < 3; i++)
            {
                System.Diagnostics.Debug.WriteLine(volatilities[i]);
            }

        }
    }
}

