using PricingLibrary.FinancialProducts;
using PricingLibrary.Utilities.MarketDataFeed;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Wrapper;

namespace PricingKanji.Model
{
    class Hedging
    {
        DateTime startdate;
        int estimationWindow;
        int rebalancingFrequency;
        List<DataFeed> feeds;
        double interest_rate;


        public Hedging(int estimation, int freq, List<DataFeed> feeds, double interest_rate)
        {
            this.estimationWindow = estimation;
            this.rebalancingFrequency = freq;
            this.feeds = feeds;
            this.interest_rate = interest_rate;
        }

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

        public Dictionary<DateTime, HedgeOutput> HedgeKandji()
        {
            WrapperClass wc = new WrapperClass();

            Dictionary<DateTime, HedgeOutput> hedging = new Dictionary<DateTime, HedgeOutput>();
            Portfolio portfolio = new Portfolio();

            List<DataFeed> estimationSample;
            double[] spots = { };

            var previous_feeds = PreviousFeeds(feeds);
            int counter = previous_feeds.Count;
            var volatilities = Calibration.Volatilities(previous_feeds);
            var correlationMatrix = Calibration.CorrMatrix(previous_feeds);
            int size = 3;
            double[] correlation_vector = new double[size * size];
            for (int i = 0; i < size; i++)
                for (int j = 0; j < size; j++)
                    correlation_vector[size * i + j] = correlationMatrix[i, j];

            var effective_feeds = KanjiFeeds(feeds);
            DateTime maturity_date = new DateTime(2021, 03, 26);
            DateTime start_date = effective_feeds.First().Date;
            DateTime last_date = effective_feeds.Last().Date;
            double maturity = (maturity_date - start_date).TotalDays;
            double matu_in_years = maturity / 365.25;
            int discretisation_number = Discretisation_number(matu_in_years, (last_date - start_date).TotalDays / 365.25);

            int H = effective_feeds.Count();
            List<int> discretisation_indices = Discretisation_instants(H, discretisation_number);
            int date_index;
            double[] path = new double[size * H];
            foreach (DataFeed feed in effective_feeds)
            {
                date_index = effective_feeds.IndexOf(feed);
                path[date_index * size] = (double)feed.PriceList["ESTX 50"];
                path[date_index * size + 1] = (double)feed.PriceList["S&P 500"];
                path[date_index * size + 2] = (double)feed.PriceList["HANG SENG INDEX"];
            }
            DataFeed prev_market = null;
            foreach (DataFeed market in effective_feeds)
            {
                if (effective_feeds.IndexOf(market) > 0)
                {
                    prev_market = effective_feeds[effective_feeds.IndexOf(market) - 1];
                }
                spots = Market.marketSpots(market);
                double[] past = GetPast(market, effective_feeds, discretisation_indices);
                int nb_dates = past.Length / 3;
                double t = (market.Date - start_date).TotalDays;
                double t_in_years = t / 365.25;

                if (counter == previous_feeds.Count)
                {
                    wc.getPriceDeltaPerft(50000, matu_in_years, t_in_years, past, nb_dates, volatilities, correlation_vector, interest_rate);
                    portfolio.UpdateComposition(wc.getDeltas(), market, wc.getPrice(), start_date);
                    HedgeOutput returnStruct = new HedgeOutput();
                    returnStruct.portfolioValue = portfolio.GetValue(market);
                    returnStruct.optionValue = wc.getPrice();
                    hedging.Add(market.Date, returnStruct);
                }
                else if (counter % rebalancingFrequency == 0)
                {
                    // at rebanlancing date we estimate the parameters
                    estimationSample = feeds.GetRange(counter - estimationWindow, estimationWindow);
                    correlationMatrix = Calibration.CorrMatrix(estimationSample);
                    volatilities = Calibration.Volatilities(estimationSample);

                    for (int i = 0; i < size; i++)
                        for (int j = 0; j < size; j++)
                            correlation_vector[size * i + j] = correlationMatrix[i, j];

                    wc.getPriceDeltaPerft(50000, matu_in_years, t_in_years, past, nb_dates, volatilities, correlation_vector, interest_rate);
                    portfolio.UpdateComposition(wc.getDeltas(), market, wc.getPrice(), start_date);
                    HedgeOutput returnStruct = new HedgeOutput();
                    returnStruct.portfolioValue = portfolio.GetValue(market);
                    returnStruct.optionValue = wc.getPrice();
                    hedging.Add(market.Date, returnStruct);
                }
                else
                {
                    wc.getPriceDeltaPerft(50000, matu_in_years, t_in_years, past, nb_dates, volatilities, correlation_vector, interest_rate);
                    HedgeOutput returnStruct = new HedgeOutput();
                    returnStruct.portfolioValue = portfolio.GetValue(market);
                    returnStruct.optionValue = wc.getPrice();
                    hedging.Add(market.Date, returnStruct);
                }

                counter++;
            }

            return hedging;

        }



    }
}
