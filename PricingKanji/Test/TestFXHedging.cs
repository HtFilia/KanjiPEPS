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
    class TestFXHedging
    {
        static void Main(string[] args)
        {
            Stopwatch sw = new Stopwatch();
            sw.Start();
            int estimationwindow = 200;
            estimationwindow = 60;
            int freq = 1;
            DataReader reader = new DataReader();
            //02 / 03 / 2020
            List<DataFeed> data = reader.ReadDataFX();
            DateTime userDate = data.Last().Date;
           //DateTime userDate = new DateTime(2018, 02, 12);
            Hedging hedging = new Hedging(estimationwindow, freq, userDate, true);
            Dictionary<DateTime, HedgeState> output = hedging.HedgeKandji();
            var csv = new StringBuilder();
            var newLine = string.Format("{0};{1};{2};{3};{4};{5};{6};{7};{8}", "Date", "Kanji Price", "Hedging Portfolio", "Error ", "EUROSTOXX", "USDEUR", "S&P500", "HKDEUR", "HANG SENG");
            csv.AppendLine(newLine);
            double error;
            foreach (DateTime date in output.Keys)
            {
                error = (output[date].portfolioValue - output[date].optionValue);
                List<double> parts = output[date].getAssetValuesEur().Values.ToList();
                newLine = string.Format("{0};{1};{2};{3};{4};{5};{6};{7};{8}", date.ToString("d"), output[date].optionValue.ToString().Replace(",", "."), output[date].portfolioValue.ToString().Replace(",", "."), error.ToString().Replace(",", "."), parts[0].ToString().Replace(",", "."), parts[1].ToString().Replace(",", "."), parts[2].ToString().Replace(",", "."), parts[3].ToString().Replace(",", "."), parts[4].ToString().Replace(",", "."));
                csv.AppendLine(newLine);
            }
            sw.Stop();
            csv.AppendLine((sw.ElapsedMilliseconds / 1000).ToString());
            File.WriteAllText(@"../../../../semihistoric-hedging-FX.csv", csv.ToString());
        }
    }
}
