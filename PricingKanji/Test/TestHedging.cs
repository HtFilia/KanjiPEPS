using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Threading.Tasks;
using PricingKanji.Model;
using PricingLibrary.FinancialProducts;
using PricingLibrary.Utilities.MarketDataFeed;


namespace PricingKanji
{
    class TestHedging
    {
        static void Main(string[] args)
        {
            DataReader reader = new DataReader();
            List<DataFeed> data = reader.ReadData();
            int freq = 1;
            int estimation_window = 80;
            double r = 0.01;
            Hedging hedging = new Hedging(estimation_window, freq, data, r);
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
                error = 100*(output[date].portfolioValue - output[date].optionValue);
                newLine = string.Format("{0};{1};{2};{3}", date.ToString("d"), output[date].optionValue.ToString().Replace(",","."), output[date].portfolioValue.ToString().Replace(",", "."), error.ToString().Replace(",", "."));
                csv.AppendLine(newLine);
            }
            File.WriteAllText(@"C:\Users\anas\source\repos\PEPS\hedging-res.csv", csv.ToString());

        }

    }
}
