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

        // get the discretisation number of our data (between start date and last date, given in years
        public int Discretisation_number(double maturity, double last_t)
        {
            return (int)Math.Ceiling((double)(last_t * 16.0) / maturity);
        }

        public List<int> Discretisation_instants(int H, int discret_number)
        {
            List<int> instants = new List<int>();
            int k = (int)H / discret_number;
            for (int i = 0; i <= discret_number; i++)
            {
                instants.Add(i * k);
            }

            return instants;

        }
        List<DataFeed> KanjiFeeds(List<DataFeed> feeds)
        {
            List<DataFeed> effective_feeds = new List<DataFeed>();
            foreach (DataFeed feed in feeds)
            {
                if (feed.Date.CompareTo(new DateTime(2013, 03, 22)) >= 0)
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

        double[] GetPast(DataFeed current_feed, List<DataFeed> feeds, List<int> instants)
        {

            List<DataFeed> previous_discret_feeds = new List<DataFeed>();
            foreach (DataFeed feed in feeds)
            {
                if ((feeds.IndexOf(feed) < feeds.IndexOf(current_feed) && instants.Contains(feeds.IndexOf(feed))) || feeds.IndexOf(feed) == feeds.IndexOf(current_feed))
                {
                    previous_discret_feeds.Add(feed);
                }

            }
            double[] past = new double[3 * previous_discret_feeds.Count()];
            int date_index;
            foreach (DataFeed feed in previous_discret_feeds)
            {
                // there is only one occurence of the feed in the list of the effective feeds
                date_index = previous_discret_feeds.IndexOf(feed);
                past[date_index * 3] = (double)feed.PriceList["ESTX 50"];
                past[date_index * 3 + 1] = (double)feed.PriceList["S&P 500"];
                past[date_index * 3 + 2] = (double)feed.PriceList["HANG SENG INDEX"];
            }

            return past;
        }

        public Dictionary<DateTime, double> PriceKanji(List<DataFeed> feeds, double interest_rate)
        {
            Dictionary<DateTime, double> prices = new Dictionary<DateTime, double>();
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
            DateTime maturity_date = new DateTime(2021, 03, 26);

            // construct the whole path of our indices from the effective feeds (from issue_date to X : last date in the feeds)
            //int H = DayCount.CountBusinessDays(start_date, last_date);
            int H = effective_feeds.Count();
            int size = 3; // size of the underlying indices
            int date_index;
            double[] volatilities;
            double[,] correlation;
            double[] path = new double[size * H];
            double maturity = (maturity_date - start_date).TotalDays;
            double matu_in_years = maturity / 365.25;

            int discretisation_number = Discretisation_number(matu_in_years, (last_date - start_date).TotalDays / 365.25);
            List<int> discretisation_indices = Discretisation_instants(H, discretisation_number);
            foreach (DataFeed feed in effective_feeds)
            {
                // there is only one occurence of the feed in the list of the effective feeds
                date_index = effective_feeds.IndexOf(feed);
                path[date_index * size] = (double)feed.PriceList["ESTX 50"];
                path[date_index * size + 1] = (double)feed.PriceList["S&P 500"];
                path[date_index * size + 2] = (double)feed.PriceList["HANG SENG INDEX"];
            }

            // now, for each effective feed, we will estimate the vol and the correlation matrix from the whole data feeds
            List<DataFeed> previous_feeds = new List<DataFeed>();

            foreach (DataFeed feed in effective_feeds)
            {

                if (effective_feeds.IndexOf(feed) == 0)
                {
                    previous_feeds = PreviousFeeds(feeds);

                }
                //volatilities = new double[]{ 0.3, 0.3, 0.3 };
                // estimate the parameters of the model
                volatilities = Calibration.Volatilities(previous_feeds);
                correlation = Calibration.CorrMatrix(previous_feeds);
                // fill the correlation 
                double[] correlation_vector = new double[size * size];
                for (int i = 0; i < size; i++)
                {
                    for (int j = 0; j < size; j++)
                    {
                        correlation_vector[size * i + j] = correlation[i, j];
                    }
                }
                // after estimation 
                previous_feeds.Add(feed);
                double[] past = GetPast(feed, effective_feeds, discretisation_indices);
                int nb_dates = past.Length / 3;
                // WRE NOT WORKING !!!
                // price the kanji product at instant t = feed.DateTime
                double t = (feed.Date - start_date).TotalDays;
                double t_in_years = t / 365.25;
                WrapperClass wc = new WrapperClass();
                //double[] spots = new double[] { path[0], path[1], path[2] };
                //wc.getPriceDeltaPerf(50000, maturity,spots, volatilities, correlation, interest_rate);
                wc.getPriceDeltaPerft(50000, matu_in_years, t_in_years, past, nb_dates, volatilities, correlation_vector, interest_rate);
                prices.Add(feed.Date, wc.getPrice());
                Console.WriteLine("price at  " + feed.Date + " : " + wc.getPrice());
                // add the feed as history (for the calibration)

            }

            return prices;
        }
    }
}
