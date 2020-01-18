using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PricingLibrary;
using PricingLibrary.Computations;
using PricingLibrary.Utilities;
using PricingLibrary.Utilities.MarketDataFeed;
using Wrapper;

namespace PricingKanji.Model
{
    class Pricing
    {
        // function that takes a list of data feeds (between issue date of the product and X)
        // each feed corresponds to the value  of the three indices at time t betwen issue date and X
        // output : list of pricing results
        // for this stage, pricing result will only contain the price of the product at time t
        List<DataFeed> KanjiFeeds(List<DataFeed> feeds)
        {
            List<DataFeed> effective_feeds = new List<DataFeed>();
            foreach(DataFeed feed in feeds)
            {
                if(feed.Date.CompareTo(new DateTime(2013, 03, 22)) >= 0)
                {
                    effective_feeds.Add(feed);
                }
            }
            return effective_feeds;
        }
        List<DataFeed> PreviousFeeds(List<DataFeed> feeds)
        {
            List<DataFeed> previous_feeds = new List<DataFeed>();
            foreach (DataFeed feed in feeds)
            {
                if (feed.Date.CompareTo(new DateTime(2013, 03, 22)) < 0)
                {
                    previous_feeds.Add(feed);
                }
            }
            return previous_feeds;
        }
    
        public List<double> PriceKanji(List<DataFeed> feeds, double interest_rate)
        {
            List<double> prices = new List<double>();
            // for each feed (datetime)
            // we estimate the vector of volatilities and correlation matrix based on the previous data feeds
            // we price the kanji product
            // we insert the price in the output
            // the maturity of the product (double) will be obtained by the number of seconds between issue date and maturity date
            // the instant of pricing = number of secs between issue date and the datetime of the considered feed
            // matrix past : voir avec meloki
            List<DataFeed> effective_feeds = KanjiFeeds(feeds);

            DateTime start_date = effective_feeds.First().Date;
            DateTime last_date = effective_feeds.Last().Date;

            // construct the whole path of our indices from the effective feeds (from issue_date to X : last date in the feeds)
            //int H = DayCount.CountBusinessDays(start_date, last_date);
            int H = effective_feeds.Count();
            int size = 3; // size of the underlying indices
            int date_index;
            double[] volatilities;
            double correlation;
            double[] path = new double[size * H];
            double maturity = (last_date.Date - start_date).TotalSeconds;
            foreach (DataFeed feed in effective_feeds)
            {
                // there is only one occurence of the feed in the list of the effective feeds
                date_index = effective_feeds.IndexOf(feed);
                path[date_index * size] = (double) feed.PriceList["ESTX 50"];
                path[date_index * size + 1] = (double) feed.PriceList["S&P 500"];
                path[date_index  * size + 2] = (double)feed.PriceList["HANG SENG INDEX"];
            }

            // now, for each effective feed, we will estimate the vol and the correlation matrix from the whole data feeds
            foreach(DataFeed feed in effective_feeds)
            {
                List<DataFeed> previous_feeds = PreviousFeeds(feeds);
                // estimate the parameters of the model
                //volatilities = Calibration.Volatilities(previous_feeds);
                //correlation = Calibration.CorrMatrix(previous_feeds)[0,1];

                // WRE NOT WORKING !!!
                volatilities = new double[] { 0.2, 0.2, 0.2 };
                correlation = 0.5;
                // price the kanji product at instant t = feed.DateTime
                double t = (feed.Date - start_date).TotalSeconds;
                WrapperClass wc = new WrapperClass();
                Console.WriteLine("Pricing");
                double[] spots = new double[] { path[0], path[1], path[2] };
                wc.getPriceDeltaPerf(50000, maturity,spots, volatilities, correlation, interest_rate);
                //wc.getPriceDeltaPerft(50000, maturity, t, path, H, volatilities, correlation, interest_rate);
                prices.Add(wc.getPrice());
                Console.WriteLine("price at  " + feed.Date + " : " + wc.getPrice());
                // add the feed as history (for the calibration)
                previous_feeds.Add(feed);
            }

            return prices;
        }
    }
}
