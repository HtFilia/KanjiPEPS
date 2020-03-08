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
            
            Hedging hedging = new Hedging(80, 100, data, 0.01);
            Dictionary<DateTime, HedgeOutput> output = hedging.HedgeKandji();

            foreach (DateTime date in output.Keys)
            {
                Console.WriteLine(date + " : " + output[date].optionValue.ToString() + " // " + output[date].portfolioValue.ToString());
            }

            var csv = new StringBuilder();
            var newLine = string.Format("{0};{1};{2}", "Date", "Kanji Price", "Hedging Portfolio");
            csv.AppendLine(newLine);

            foreach (DateTime date in output.Keys)
            {
                newLine = string.Format("{0};{1};{2}", date.ToString("d"), output[date].optionValue.ToString(), output[date].portfolioValue.ToString());
                csv.AppendLine(newLine);

            }
            File.WriteAllText(@"C:\Users\anas\source\repos\PEPS\hedging-res.csv", csv.ToString());

        }

    }
}
