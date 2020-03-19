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
using Wrapper;

namespace PricingKanji
{
    class TestSemihistoric
    {
        static void Main(string[] args)
        {
            Stopwatch sw = new Stopwatch();
            sw.Start();
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

            //foreach (DateTime date in output.Keys)
            //{
            //    Console.WriteLine(date + " : " + output[date].optionValue.ToString() + " // " + output[date].portfolioValue.ToString());
            //}

            var csv = new StringBuilder();
            var newLine = string.Format("{0};{1};{2};{3};{4};{5};{6}", "Date", "Kanji Price", "Hedging Portfolio","Error ", "EUROSTOXX", "S&P500", "Hang Seng");
            csv.AppendLine(newLine);
            double error;
            foreach (DateTime date in output.Keys)
            {
                error = (output[date].portfolioValue - output[date].optionValue);
                List<double> deltas = output[date].composition.Values.ToList();
                newLine = string.Format("{0};{1};{2};{3};{4};{5};{6}", date.ToString("d"), output[date].optionValue.ToString().Replace(",", "."), output[date].portfolioValue.ToString().Replace(",", "."), error.ToString().Replace(",", "."),deltas[0].ToString().Replace(",", "."), deltas[1].ToString().Replace(",", "."), deltas[2].ToString().Replace(",", "."));
                csv.AppendLine(newLine);
            }
            sw.Stop();
            csv.AppendLine((sw.ElapsedMilliseconds / 1000).ToString());
            File.WriteAllText(@"C:\Users\anas\source\repos\PEPS\semihistoric-hedging.csv", csv.ToString());
        }

    }
}
