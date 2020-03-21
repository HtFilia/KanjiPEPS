using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PricingKanji.Model;
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
            //List<DataFeed> feeds = reader.ReadDataBis((decimal) 0.05, (decimal) 0.07);
            List<DataFeed> feeds = reader.ReadData();
            DataFeed last = feeds.Last();
            int estimationwindow = 200;
            DateTime maturityDate = new DateTime(2021, 03, 26);
            List<DataFeed> simulatedfeeds = Market.simulate(feeds, maturityDate, estimationwindow);
            feeds = feeds.Concat(simulatedfeeds).ToList();
            estimationwindow = 60;
            double r = 0.01;
            int freq = 1;
            Hedging hedging = new Hedging(estimationwindow, freq, feeds, r);
            Dictionary<DateTime, HedgeOutput> output = hedging.HedgeKandji();
            var csv = new StringBuilder();
            var newLine = string.Format("{0};{1};{2};{3};{4};{5};{6}", "Date", "Kanji Price", "Hedging Portfolio", "Error ", "EUROSTOXX", "S&P500", "Hang Seng");
            csv.AppendLine(newLine);
            double error;
            foreach (DateTime date in output.Keys)
            {
                error = (output[date].portfolioValue - output[date].optionValue);
                List<double> deltas = output[date].composition.Values.ToList();
                newLine = string.Format("{0};{1};{2};{3};{4};{5};{6}", date.ToString("d"), output[date].optionValue.ToString().Replace(",", "."), output[date].portfolioValue.ToString().Replace(",", "."), error.ToString().Replace(",", "."), deltas[0].ToString().Replace(",", "."), deltas[1].ToString().Replace(",", "."), deltas[2].ToString().Replace(",", "."));
                csv.AppendLine(newLine);
            }
            sw.Stop();
            csv.AppendLine((sw.ElapsedMilliseconds / 1000).ToString());
            File.WriteAllText(@"../../../../semihistoric-hedging.csv", csv.ToString());

        }

    }
}
