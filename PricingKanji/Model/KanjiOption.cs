﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PricingLibrary.Utilities.MarketDataFeed;
using Wrapper;

namespace PricingKanji.Model
{
    class KanjiOption
    {
        public Dictionary<string, double> InitialValues { get; set; }
        public List<DateTime> observationDates { get; set; }
        DateTime maturity;
        DateTime startDate;
        int nbTimeSteps;
        public int size;
        List<string> indexesName;
        public double NetAssetValue { get; set; } // valeur liquidiative

        public static List<DateTime> initialValueDates()
        {
            DateTime date0 = new DateTime(2013, 3, 26);
            DateTime date1 = new DateTime(2013, 3, 22);
            DateTime date2 = new DateTime(2013, 3, 25);
            List<DateTime> dates = new List<DateTime>() { date0, date1, date2 };
            return dates;
        }

        public KanjiOption(Market market, List<DateTime> initialValueDates, double netAssetValue_ = 100)
        {
            NetAssetValue = netAssetValue_;
            size = market.feeds.First().PriceList.Count;
            nbTimeSteps = 16;
            maturity = new DateTime(2021, 3, 26);
            startDate = new DateTime(2013, 3, 26);
            InitialValues = new Dictionary<string, double>();
            List<double> initialvalues = new List<double> { 0, 0, 0, 0, 0 };
            indexesName = market.feeds.First().PriceList.Keys.ToList();
            int count;
            foreach (DateTime date in initialValueDates)
            {
                DataFeed feed = market.getFeed(date);
                count = 0;
                foreach (string name in indexesName)
                {
                    initialvalues[count] += (double)feed.PriceList[name] / initialValueDates.Count;
                    count++;
                }
                count = 0;
            }
            count = 0;
            foreach (string name in indexesName)
            {
                InitialValues[name] = initialvalues[count];
                count++;
            }
            observationDates = new List<DateTime>
            { new DateTime(2013, 9, 23), new DateTime(2014, 3, 24), new DateTime(2014, 9, 22), new DateTime(2015, 3, 23),
            new DateTime(2015, 9, 22), new DateTime(2016, 3, 22), new DateTime(2016, 9, 22), new DateTime(2017, 3, 22),
            new DateTime(2017, 9, 22), new DateTime(2018, 3, 22), new DateTime(2018, 9, 24), new DateTime(2019, 3, 22),
            new DateTime(2019, 9, 23), new DateTime(2020, 3, 23), new DateTime(2020, 9, 22), new DateTime(2021, 3, 23)};
        }


        public List<DataFeed> netassetValueFeeds(List<DataFeed> feeds)
        {
            DateTime firstDate = new DateTime(2013, 1, 11);
            DateTime lastDate = new DateTime(2013, 3, 22);
            List<DataFeed> returnFeeds = new List<DataFeed>();
            foreach (var feed in feeds)
            {
                if (feed.Date.CompareTo(firstDate) >= 0 && feed.Date.CompareTo(lastDate) <= 0)
                {
                    returnFeeds.Add(feed);
                }
                if(feed.Date >= lastDate)
                {
                    break;
                }
            }
            return returnFeeds;
        }
        
    }
}
