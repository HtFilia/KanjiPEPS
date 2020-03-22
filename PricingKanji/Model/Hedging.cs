using PricingLibrary.FinancialProducts;
using PricingLibrary.Utilities.MarketDataFeed;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Wrapper;

namespace PricingKanji.Model
{
    public class Hedging
    {
        private DateTime startdate ;
        private DateTime maturity_date;
        int estimationWindow;
        int rebalancingFrequency;
        List<DataFeed> feeds;
        Market market;
        public double[] volatilities;
        public double[] correlation_vector;
        int size = 3;
        KanjiOption kanji;

        public Hedging(int estimation, int freq, List<DataFeed> feeds_)
        {
            this.estimationWindow = estimation;
            this.rebalancingFrequency = freq;
            this.market = new Market(feeds_);
            feeds = feeds_;
            startdate = new DateTime(2013, 03, 26);
            maturity_date = new DateTime(2021, 03, 23);
            size = market.feeds.First().PriceList.Count;
            volatilities = new double[size];
            correlation_vector = new double[size * size];
            kanji = new KanjiOption(market,KanjiOption.initialValueDates());

        }


        List<DataFeed> KanjiFeeds(List<DataFeed> feeds)
        {
            List<DataFeed> effective_feeds = new List<DataFeed>();
            foreach (DataFeed feed in feeds)
            {
                if (feed.Date.CompareTo(startdate) >= 0 && feed.Date.CompareTo(maturity_date) <= 0)
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
                if (feed.Date.CompareTo(startdate) < 0 )
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
            double[] spots = { };
            var previous_feeds = PreviousFeeds(feeds);
            int counter = previous_feeds.Count;
            calibrateParameters(counter);
            var effective_feeds = KanjiFeeds(feeds);
            DateTime start_date = effective_feeds.First().Date;
            DateTime last_date = effective_feeds.Last().Date;
            double matu_in_years = Utilities.ComputeTime(start_date, maturity_date, market);
            DataFeed prevMarket = null;
            double netAssetValue = 100;
            double[] initial_values = Utilities.ToDouble(effective_feeds.First().PriceList.Values.ToArray());
            double[] past;
            HedgeOutput returnStruct;

            foreach (DataFeed feed in effective_feeds)
            {
                past = getPast(feed.Date);
                int nb_dates = past.Length / 3;
                double t_in_years = Utilities.ComputeTime(start_date, feed.Date,market);
                returnStruct = new HedgeOutput();
                if (counter == previous_feeds.Count)
                {
                    wc.getPriceDeltaPerft(netAssetValue,matu_in_years, t_in_years, past, initial_values, nb_dates, volatilities, correlation_vector, Market.r);
                    portfolio.UpdateComposition(wc.getDeltas(), feed, prevMarket, wc.getPrice(), start_date, market);
                    prevMarket = feed;
                }
                else if ((counter- previous_feeds.Count) % rebalancingFrequency == 0)
                {
                    calibrateParameters(counter);
                    wc.getPriceDeltaPerft(netAssetValue, matu_in_years, t_in_years, past, initial_values, nb_dates, volatilities, correlation_vector, Market.r);
                    portfolio.UpdateComposition(wc.getDeltas(), feed,prevMarket, wc.getPrice(), start_date, market);
                    prevMarket = feed;
                }
                else
                {
                    wc.getPricePerft(netAssetValue, matu_in_years, t_in_years, past, initial_values, nb_dates, volatilities, correlation_vector, Market.r);
                    portfolio.GetValue(feed, prevMarket, startdate, market);
                }
                returnStruct.portfolioValue = portfolio.Value;
                returnStruct.optionValue = wc.getPrice();
                returnStruct.composition = new Dictionary<string, double>(portfolio.composition);
                hedging.Add(feed.Date, returnStruct);
                //Console.WriteLine(returnStruct.optionValue + " " + returnStruct.portfolioValue + " " + (returnStruct.portfolioValue- returnStruct.optionValue));
                counter++;
            }
            return hedging;

        }


        public void calibrateParameters(int counter)
        { //estimate correlation and volatilities with prior estimationWindow dates, counter is date index in market.feeds
            List<DataFeed> estimationSample = feeds.GetRange(counter - estimationWindow, estimationWindow);
            double[,] correlationMatrix = Calibration.getCorrelations(estimationSample);
            volatilities = Calibration.getVolatilities(estimationSample);
            for (int i = 0; i < size; i++)
                for (int j = 0; j < size; j++)
                    correlation_vector[size * i + j] = correlationMatrix[i, j];
        }

        public double[] getPast(DateTime date)
        {
            DataFeed currFeed = market.getFeed(date);
            List<DataFeed> PastFeeds = new List<DataFeed> { market.getFeed(startdate) };
            foreach (DataFeed feed in market.feeds)
            {
                if (kanji.observationDates.Contains(feed.Date) && feed.Date <= date)
                {
                    PastFeeds.Add(feed);
                }
            }
            if (!PastFeeds.Contains(currFeed) && date <= kanji.observationDates.Last())
            {
                PastFeeds.Add(currFeed);
            }
            double[] past = new double[3 * PastFeeds.Count];
            int date_index;
            foreach (DataFeed feed in PastFeeds)
            {
                // there is only one occurence of the feed in the list of the effective feeds
                date_index = PastFeeds.IndexOf(feed);
                past[date_index * kanji.size] = (double)feed.PriceList["ESTX 50"];
                past[date_index * kanji.size + 1] = (double)feed.PriceList["S&P 500"];
                past[date_index * kanji.size + 2] = (double)feed.PriceList["HANG SENG INDEX"];
            }
            return past;

        }



    }
}
