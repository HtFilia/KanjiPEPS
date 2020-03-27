using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PricingKanji.Model;
using PricingLibrary.Utilities;
using PricingLibrary.Utilities.MarketDataFeed;

namespace PricingKanji
{
    class DataWriter
    {
        static void Main(string[] args)
        {
            Stopwatch sw = new Stopwatch();
            sw.Start();
            DataReader reader = new DataReader();
            //List<DataFeed> feeds = reader.ReadDataBis((decimal) 0.05, (decimal) 0.07);
            List<DataFeed> feeds = reader.ReadData();
            DataFeed last = feeds.Last();
            int estimationwindow = 200;
            DateTime maturityDate = new DateTime(2021, 03, 26);
            List<DataFeed> simulatedfeeds = Market.simulate(feeds, maturityDate, estimationwindow);
            feeds = feeds.Concat(simulatedfeeds).ToList();
            int freq = 1;
            int estimation_window = 80;
            Market market = new Market();
            market.feeds = feeds;
            Kanji kanji = new Kanji(market, Kanji.initialValueDates());
            Hedging hedging = new Hedging(estimation_window, freq, market, kanji);
            hedging.calibrateParameters(estimationwindow);
            List<DataFeed> effective_feeds = market.feeds.GetRange(hedging.previous_feeds_number, market.feeds.Count - hedging.previous_feeds_number);
            String s = "# maturity time: " + (hedging.computeTime(hedging.startdate, maturityDate)).ToString().Replace(",", ".") + "\n";
            s += "# model size: " + kanji.size;
            s += "# number of dates: " + (market.feeds.Count - hedging.previous_feeds_number).ToString() + "\n";
            s += "# spot: ";
            foreach (var item in market.getFeed(hedging.startdate).PriceList.Values)
            {
                s += item.ToString().Replace(",", ".") + " ";
            }
            s += "\n";
            s += "# dividend rate: 0.000000 0.000000 0.000000\n";
            s += "# correlation: 0.2\n";
            s += "# volatility: " ;
            foreach (var item in hedging.volatilities)
            {
                s += item.ToString().Replace(",", ".") + " ";
            }
            s += "\n";
            foreach (var feed in effective_feeds)
            {
                foreach (var index in feed.PriceList.Values)
                {
                    s += index.ToString().Replace(",", ".") + " ";
                }
                s += "\n";
            }
            Console.WriteLine(s);
            File.WriteAllText(@"../../../../kanji_market.dat", s);
        }
    }
}
