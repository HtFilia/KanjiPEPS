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
        public DateTime startdate;
        public DateTime maturity_date;
        public int estimationWindow;
        public int rebalancingFrequency;
        public Market market;
        public double[] volatilities;
        public double[] correlation_vector;
        int size;
        public KanjiOption kanji;
        WrapperClass wc;
        Portfolio portfolio;
        DataFeed previousFeed;
        int previous_feeds_count;
        double[] initial_values;
        public bool FX;
        public Hedging(int estimation, int freq, DateTime userDate, bool FX_)
        {
            FX = FX_;
            startdate = new DateTime(2013, 03, 26);
            maturity_date = new DateTime(2021, 03, 23);
            estimationWindow = estimation;
            rebalancingFrequency = freq;
            market = new Market(userDate, FX);
            market.completeMarket(maturity_date, estimationWindow);
            size = market.feeds.First().PriceList.Count;
            volatilities = new double[size];
            correlation_vector = new double[size * size];
            portfolio = new Portfolio(FX);
            kanji = new KanjiOption(market,KanjiOption.initialValueDates());
            wc = new WrapperClass();
        }

        public Dictionary<DateTime, HedgeState> HedgeKandji()
        {
            //computeNetAssetValue();
            Dictionary<DateTime, HedgeState> hedging = new Dictionary<DateTime, HedgeState>();
            double[] spots = { };
            var previous_feeds = market.PreviousFeeds(market.feeds, startdate);
            var effective_feeds = market.KanjiFeeds(market.feeds, startdate, maturity_date, FX);
            //effective_feeds = effective_feeds.GetRange(0, effective_feeds.Count - effective_feeds.Count%16 + 1);
            //maturity_date = effective_feeds.Last().Date;
            //KanjiOption.observationDates = kanji.computeObservationDate(effective_feeds);
            DateTime last_date = effective_feeds.Last().Date;
            initial_values = kanji.InitialValues.Values.ToArray();
            HedgeState returnStruct;
            previous_feeds_count = previous_feeds.Count;
            int counter = previous_feeds_count;
            calibrateParameters(counter);
            foreach (DataFeed feed in effective_feeds)
            {
                returnStruct = hedgingStep(counter);
                hedging.Add(feed.Date, returnStruct);
                Console.WriteLine(returnStruct.optionValue + " " + returnStruct.portfolioValue + " " + (returnStruct.portfolioValue- returnStruct.optionValue));
                counter++;
            }
            return hedging;

        }


        public void calibrateParameters(int counter)
        { //estimate correlation and volatilities with prior estimationWindow dates, counter is date index in market.feeds
            List<DataFeed> estimationSample = market.feeds.GetRange(counter - estimationWindow, estimationWindow);
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
                if (KanjiOption.observationDates.Contains(feed.Date) && feed.Date.CompareTo(date) <= 0)
                {
                    PastFeeds.Add(feed);
                }
            }
            if (!PastFeeds.Contains(currFeed) && date.CompareTo(KanjiOption.observationDates.Last()) <= 0)
            {
                PastFeeds.Add(currFeed);
            }
            if (KanjiOption.observationDates.Contains(date))
            {
                ;
            }
            double[] past = new double[size * PastFeeds.Count];
            int date_index;
            foreach (DataFeed feed in PastFeeds)
            {
                // there is only one occurence of the feed in the list of the effective feeds
                date_index = PastFeeds.IndexOf(feed);
                if (!FX)
                {
                    past[date_index * size] = (double)feed.PriceList["ESTX 50"];
                    past[date_index * size + 1] = (double)feed.PriceList["S&P 500"];
                    past[date_index * size + 2] = (double)feed.PriceList["HANG SENG INDEX"];
                }
                else if(FX)
                {
                    past[date_index * size] = (double)feed.PriceList["ESTX 50"];
                    past[date_index * size + 1] = (double)feed.PriceList["USDEUR"];
                    past[date_index * size + 2] = (double)feed.PriceList["S&P 500"];
                    past[date_index * size + 3] = (double)feed.PriceList["HKDEUR"];
                    past[date_index * size + 4] = (double)feed.PriceList["HANG SENG INDEX"];
                }
            }
            return past;

        }

        public double price(DateTime date)
        {
            double matu_in_years = Utilities.ComputeTime(startdate, maturity_date, market);
            double t_in_years = Utilities.ComputeTime(startdate, date, market);
            double[] past = getPast(date);
            int nb_dates = past.Length / size;
            calibrateParameters(market.feeds.IndexOf(market.getFeed(date)));
            wc.ComputePrice(kanji.NetAssetValue, matu_in_years, t_in_years, past, kanji.InitialValues.Values.ToArray(), nb_dates, volatilities, correlation_vector, Market.r_eur, Market.r_usd, Market.r_hkd, FX);
            return wc.getPrice(FX);
        }

        public void computeNetAssetValue()
        {
            List<DataFeed> pricingFeeds = kanji.netassetValueFeeds(market.feeds);
            DataFeed firstFeed = pricingFeeds.First();
            double matu_in_years = Utilities.ComputeTime(firstFeed.Date, maturity_date, market);
            List<double> prices = new List<double>();
            double t_in_years;
            double[] past;
            foreach (DataFeed feed in pricingFeeds)
            {
                t_in_years = Utilities.ComputeTime(firstFeed.Date, feed.Date, market);
                past =  Utilities.ToDouble(feed.PriceList.Values.ToArray());
                calibrateParameters(market.feeds.IndexOf(feed));
                wc.ComputePrice(kanji.NetAssetValue, matu_in_years, t_in_years, past, kanji.InitialValues.Values.ToArray(), 1, volatilities, correlation_vector, Market.r_eur, Market.r_usd, Market.r_hkd, FX);
                prices.Add(price(feed.Date));
            }
            kanji.NetAssetValue = prices.Max();
        }

        public HedgeState hedgingStep(int counter)

        {
            DataFeed feed = market.feeds[counter];
            double matu_in_years = Utilities.ComputeTime(startdate, maturity_date, market);
            double t_in_years = Utilities.ComputeTime(startdate, feed.Date, market);
            double[] past = getPast(feed.Date);
            int nb_dates = past.Length / size;
            HedgeState returnStruct = new HedgeState();
            if (counter == market.feeds.Count - 1)
            {
                portfolio.GetValue(feed, previousFeed, startdate, market);
            }

            else if (counter == previous_feeds_count)
            {
                wc.ComputePriceDelta(kanji.NetAssetValue, matu_in_years, t_in_years, past, initial_values, nb_dates, volatilities, correlation_vector, Market.r_eur, Market.r_usd, Market.r_hkd, FX);
                portfolio.UpdateComposition(wc.getDeltas(FX), feed, previousFeed, wc.getPrice(FX), startdate, market);
                previousFeed = feed;
            }
            else if ((counter - previous_feeds_count) % rebalancingFrequency == 0)
            {
                calibrateParameters(counter);
                wc.ComputePriceDelta(kanji.NetAssetValue, matu_in_years, t_in_years, past, initial_values, nb_dates, volatilities, correlation_vector, Market.r_eur, Market.r_usd, Market.r_hkd, FX);
                portfolio.UpdateComposition(wc.getDeltas(FX), feed, previousFeed, wc.getPrice(FX), startdate, market);
                previousFeed = feed;
            }
            else
            {
                wc.ComputePrice(kanji.NetAssetValue, matu_in_years, t_in_years, past, initial_values, nb_dates, volatilities, correlation_vector, Market.r_eur, Market.r_usd, Market.r_hkd, FX);
                portfolio.GetValue(feed, previousFeed, startdate, market);
            }
            returnStruct.feed = feed;
            returnStruct.portfolioValue = portfolio.Value;
            returnStruct.optionValue = wc.getPrice(FX);
            returnStruct.composition = new Dictionary<string, double>(portfolio.composition);
            return returnStruct;
        }

    }
}
