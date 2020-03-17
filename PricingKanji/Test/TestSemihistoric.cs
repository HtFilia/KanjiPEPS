using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PricingKanji.Model;
using PricingLibrary.Utilities;
using PricingLibrary.Utilities.MarketDataFeed;
using Wrapper;

namespace PricingKanji
{
    class TestSemihistoric
    {
        static void Main(string[] args)
        {
            DataReader reader = new DataReader();
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
            Dictionary<DateTime, HedgeOutput> output = hedging.HedgeKandji();

            foreach (DateTime date in output.Keys)
            {
                Console.WriteLine(date + " : " + output[date].optionValue.ToString() + " // " + output[date].portfolioValue.ToString());
            }

            var csv = new StringBuilder();
            var newLine = string.Format("{0};{1};{2};{3}", "Date", "Kanji Price", "Hedging Portfolio", "Error * 100");
            csv.AppendLine(newLine);
            double error;
            foreach (DateTime date in output.Keys)
            {
                error = 100 * (output[date].portfolioValue - output[date].optionValue);
                newLine = string.Format("{0};{1};{2};{3}", date.ToString("d"), output[date].optionValue.ToString().Replace(",", "."), output[date].portfolioValue.ToString().Replace(",", "."), error.ToString().Replace(",", "."));
                csv.AppendLine(newLine);
            }
            File.WriteAllText(@"C:\Users\IEUser\Desktop\Kanji\semihistoric-hedging.csv", csv.ToString());

        }

    }
}
