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
            int estimationwindow = 200;
            estimationwindow = 60;
            int freq = 1;
            DataReader reader = new DataReader();
            List<DataFeed> data = reader.ReadData();
            DateTime userDate = data.Last().Date;
            //DateTime userDate = new DateTime(2013, 3, 20);
            Hedging hedging = new Hedging(estimationwindow, freq, userDate, false);
            var effective_feeds = hedging.market.KanjiFeeds(hedging.market.feeds, hedging.startdate, hedging.maturity_date, false);
            int count = 0;
            int i = 1;
            KanjiOption.observationDates = hedging.kanji.computeObservationDate(effective_feeds);
            foreach (DataFeed feed in effective_feeds)
            {
                if (KanjiOption.observationDates.Contains(feed.Date))
                {
                    Console.WriteLine("date " + i + " index " + count) ;
                    i++;
                }
                count++;
            }
            Dictionary<DateTime, HedgeState> output = hedging.HedgeKandji();
            var csv = new StringBuilder();
            var newLine = string.Format("{0};{1};{2};{3};{4};{5};{6}", "Date", "Kanji Price", "Hedging Portfolio", "Error ", "EUROSTOXX", "S&P500", "Hang Seng");
            csv.AppendLine(newLine);
            double error;
            foreach (DateTime date in output.Keys)
            {
                error = (output[date].portfolioValue - output[date].optionValue);
                List<double> parts = output[date].composition.Values.ToList();
                newLine = string.Format("{0};{1};{2};{3};{4};{5};{6}", date.ToString("d"), output[date].optionValue.ToString().Replace(",", "."), output[date].portfolioValue.ToString().Replace(",", "."), error.ToString().Replace(",", "."), parts[0].ToString().Replace(",", "."), parts[1].ToString().Replace(",", "."), parts[2].ToString().Replace(",", "."));
                csv.AppendLine(newLine);
            }
            sw.Stop();
            csv.AppendLine((sw.ElapsedMilliseconds / 1000).ToString());
            File.WriteAllText(@"../../../../semihistoric-hedging.csv", csv.ToString());
        }
    }
}
