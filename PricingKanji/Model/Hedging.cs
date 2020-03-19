using PricingLibrary.FinancialProducts;
using PricingLibrary.Utilities;
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
        private DateTime startdate;
        private DateTime maturity_date;
        int estimationWindow;
        int rebalancingFrequency;
        Market market;
        public static WrapperClass wc = new WrapperClass();
        public static int size;
        public double[] volatilities;
        public double[] correlation_vector;
        public int previous_feeds_number;
        Kanji kanji;

        public Hedging(int estimation, int freq, Market market_, Kanji kanji_)
        {
            this.estimationWindow = estimation;
            this.rebalancingFrequency = freq;
            this.market = market_;
            startdate = new DateTime(2013, 03, 22);
            maturity_date = new DateTime(2021, 03, 26);
            size = market.feeds.First().PriceList.Count;
            volatilities = new double[size];
            correlation_vector = new double[size * size];
            previous_feeds_number = NpreviousFeeds(market.feeds);
            kanji = kanji_;
        }

        int NpreviousFeeds(List<DataFeed> feeds)
        { //return size of previous feeds
            int count = 0;
            foreach (DataFeed feed in feeds)
            {
                if (feed.Date.CompareTo(startdate) < 0)
                {
                    count++;
                }
            }
            return count;
        }

        public Dictionary<DateTime, HedgeOutput> HedgeKandji()
        {

            Dictionary<DateTime, HedgeOutput> hedging = new Dictionary<DateTime, HedgeOutput>();
            Portfolio portfolio = new Portfolio();

            double[] spots = { };
            List<DataFeed> previous_feeds = market.feeds.GetRange(0, previous_feeds_number);
            List<DataFeed> effective_feeds = market.feeds.GetRange(previous_feeds_number, market.feeds.Count - previous_feeds_number);
            int counter = previous_feeds_number;
            calibrateParameters(counter);

            DateTime start_date = effective_feeds.First().Date;
            DateTime last_date = effective_feeds.Last().Date;
            double maturity = (maturity_date - start_date).TotalDays;
            double matu_in_years = maturity / Market.businessDdaysPerYear;
            DataFeed prevFeed = null;
            foreach (DataFeed feed in effective_feeds)
            {
                spots = Market.marketSpots(feed);
                double[] past = getPast(feed.Date);
                int nb_dates = past.Length / kanji.size;
                double t = (feed.Date - start_date).TotalDays;
                double t_in_years = t / Market.businessDdaysPerYear;

                if (counter == previous_feeds.Count)
                {
                    HedgeOutput returnStruct = initializePortfolio(portfolio, effective_feeds, matu_in_years, past);
                    hedging.Add(feed.Date, returnStruct);
                    prevFeed = feed;
                }
                else if ((counter - previous_feeds.Count) % rebalancingFrequency == 0)
                {
                    HedgeOutput returnStruct = rebalance(portfolio, effective_feeds, counter, matu_in_years, past);
                    hedging.Add(feed.Date, returnStruct);
                    prevFeed = feed;
                }
                else
                {
                    HedgeOutput returnStruct = computePortfolio(portfolio, effective_feeds, counter, matu_in_years, past);
                    hedging.Add(feed.Date, returnStruct);
                }
                counter++;
            }

            return hedging;

        }


        HedgeOutput rebalance(Portfolio portfolio, List<DataFeed> effectiveFeeds, int counter, double matu_in_years, double[] past)
        { //compute new portfolio composition
            // Computes new delta and price after re-estimating the parameters
            DateTime start_date = effectiveFeeds.First().Date;
            DataFeed feed = effectiveFeeds[counter - previous_feeds_number];
            DataFeed prevFeed = effectiveFeeds[counter - previous_feeds_number - 1];
            double t = DayCount.CountBusinessDays(start_date, feed.Date);
            //double t = (feed.Date - start_date).TotalDays;
            double t_in_years = t / Market.businessDdaysPerYear;
            calibrateParameters(counter);
            double[] initial_values = kanji.InitialValues.Values.ToArray();
            wc.getPriceDeltaPerft(kanji.NetAssetValue, matu_in_years, t_in_years, past, initial_values, past.Length / kanji.size, volatilities, correlation_vector, Market.r);
            portfolio.UpdateComposition(wc.getDeltas(), feed, prevFeed, wc.getPrice(), start_date);
            HedgeOutput returnStruct = new HedgeOutput();
            returnStruct.portfolioValue = portfolio.Value;
            returnStruct.optionValue = wc.getPrice();
            returnStruct.composition = new Dictionary<string, double>(portfolio.composition);
            return returnStruct;
        }

        HedgeOutput computePortfolio(Portfolio portfolio, List<DataFeed> effectiveFeeds, int counter, double matu_in_years, double[] past)
        {
            // computes the value of the porftolio with the last compositions(delta) and estimated parameters using the current market feed
            DateTime start_date = effectiveFeeds.First().Date;
            DataFeed feed = effectiveFeeds[counter - previous_feeds_number];
            DataFeed prevFeed = effectiveFeeds[counter - previous_feeds_number - 1];
            double t = DayCount.CountBusinessDays(start_date, feed.Date);
            //double t = (feed.Date - start_date).TotalDays;
            double t_in_years = t / Market.businessDdaysPerYear;
            double[] initial_values = kanji.InitialValues.Values.ToArray();
            wc.getPricePerft(kanji.NetAssetValue, matu_in_years, t_in_years, past, initial_values, past.Length / kanji.size, volatilities, correlation_vector, Market.r);
            HedgeOutput returnStruct = new HedgeOutput();
            portfolio.Value = portfolio.GetValue(feed, prevFeed);
            returnStruct.portfolioValue = portfolio.Value;
            returnStruct.optionValue = wc.getPrice();
            returnStruct.composition = new Dictionary<string, double>( portfolio.composition);
            return returnStruct;
        }


        HedgeOutput initializePortfolio(Portfolio portfolio, List<DataFeed> effectiveFeeds, double matu_in_years, double[] past)
        { //compute first composition of portfolio, and returns option and portfolio values at t = 0
            DateTime start_date = effectiveFeeds.First().Date;
            DataFeed feed = effectiveFeeds[0];
            double[] initial_values = kanji.InitialValues.Values.ToArray();
            wc.getPriceDeltaPerft(kanji.NetAssetValue, matu_in_years, 0, past, initial_values, past.Length / kanji.size, volatilities, correlation_vector, Market.r);
            portfolio.UpdateComposition(wc.getDeltas(), feed, null, wc.getPrice(), start_date);
            HedgeOutput returnStruct = new HedgeOutput();
            returnStruct.portfolioValue = portfolio.Value;
            returnStruct.optionValue = wc.getPrice();
            returnStruct.composition = new Dictionary<string, double>(portfolio.composition);
            return returnStruct;
        }

        void calibrateParameters(int counter)
        { //estimate correlation and volatilities with prior estimationWindow dates, counter is date index in market.feeds
            List<DataFeed> estimationSample = market.feeds.GetRange(counter - estimationWindow, estimationWindow);
            double[,] correlationMatrix = Calibration.CorrMatrix(estimationSample);
            volatilities = Calibration.Volatilities(estimationSample);
            for (int i = 0; i < size; i++)
                for (int j = 0; j < size; j++)
                    correlation_vector[size * i + j] = correlationMatrix[i, j];
        }

        public double[] getPast( DateTime date)
        {
            DataFeed currFeed = market.getFeed(date);
            List<DataFeed> PastFeeds = new List<DataFeed> { market.getFeed(startdate) };
            foreach(DataFeed feed in market.feeds)
            {
                if (kanji.observationDates.Contains(feed.Date) && feed.Date <= date )
                {
                    PastFeeds.Add(feed);
                }
            }
            if (!PastFeeds.Contains(currFeed) && date <= kanji.observationDates.Last()){
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

        public double price(DateTime date)
        {
            DataFeed feed = market.getFeed(date);
            List<DataFeed> effectiveFeeds = market.feeds.GetRange(previous_feeds_number, market.feeds.Count - previous_feeds_number);
            DateTime start_date = effectiveFeeds.First().Date;
            DateTime last_date = effectiveFeeds.Last().Date;
            double maturity = (maturity_date - start_date).TotalDays;
            double matu_in_years = maturity / Market.businessDdaysPerYear;
            double t = DayCount.CountBusinessDays(start_date, feed.Date);
            double t_in_years = t / Market.businessDdaysPerYear;
            double[] past = getPast(date);
            int nb_dates = past.Length / kanji.size;
            calibrateParameters(market.feeds.IndexOf(market.getFeed(date)));
            wc.getPricePerft(kanji.NetAssetValue, matu_in_years, t_in_years, past, kanji.InitialValues.Values.ToArray(), nb_dates, volatilities, correlation_vector, Market.r);
            return wc.getPrice();
        }
    }

}