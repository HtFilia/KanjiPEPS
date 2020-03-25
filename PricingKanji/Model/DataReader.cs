using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using PricingLibrary.Utilities.MarketDataFeed;
using CsvHelper;
using System.Globalization;

namespace PricingKanji.Model
{
    public class DataReader
    {
        public List<DataFeed> ReadData(String path = Utilities.path)
        {
            List<DataFeed> feeds = new List<DataFeed>();
            IndexValue EuroValues = ParseFile(path, "ESTX 50");
            IndexValue SNPValues = ParseFile(path, "S&P 500");
            IndexValue HSIValues = ParseFile(path, "HANG SENG INDEX");
            foreach (DateTime date in EuroValues.cotations.Keys)
            {
                if (SNPValues.cotations.ContainsKey(date) && HSIValues.cotations.ContainsKey(date))
                {
                    Dictionary<string, decimal> PriceList = new Dictionary<string, decimal>();
                    DataFeed feed = new DataFeed(date, PriceList);
                    PriceList.Add("ESTX 50", EuroValues.cotations[date]);
                    PriceList.Add("S&P 500", SNPValues.cotations[date]);
                    PriceList.Add("HANG SENG INDEX", HSIValues.cotations[date]);
                    feeds.Add(feed);
                }
            }
            return feeds;
        }

        public List<DataFeed> ReadDataFX(String path, decimal r_usd, decimal r_hkd)
        {
            List<DataFeed> feeds = new List<DataFeed>();
            IndexValue EuroValues = ParseFile(path, "ESTX 50");
            IndexValue SNPValues = ParseFile(path, "S&P 500");
            IndexValue HSIValues = ParseFile(path, "HANG SENG INDEX");
            IndexValue EURUSDValues = ParseFile(path, "EURUSD");
            IndexValue EURHKDValues = ParseFile(path, "EURHKD");
            double timeSpan = 1.0 / 252.0;
            double t_counter = 0;
            foreach (DateTime date in EuroValues.cotations.Keys)
            {

                if (SNPValues.cotations.ContainsKey(date) && HSIValues.cotations.ContainsKey(date) && EURUSDValues.cotations.ContainsKey(date) && EURHKDValues.cotations.ContainsKey(date))
                {
                    Dictionary<string, decimal> PriceList = new Dictionary<string, decimal>();
                    DataFeed feed = new DataFeed(date, PriceList);
                    PriceList.Add("ESTX 50", EuroValues.cotations[date]);
                    PriceList.Add("S&P 500", SNPValues.cotations[date] / EURUSDValues.cotations[date]);
                    PriceList.Add("USD", (decimal)Math.Exp((double)r_usd * t_counter) / EURUSDValues.cotations[date]);
                    PriceList.Add("HANG SENG INDEX", HSIValues.cotations[date] / EURHKDValues.cotations[date]);
                    PriceList.Add("HDK", (decimal)Math.Exp((double)r_hkd * t_counter) / EURHKDValues.cotations[date]);
                    feeds.Add(feed);
                }
                t_counter += timeSpan;
            }

            return feeds;
        }

        public IndexValue ParseFile(String path, String Name)
        {
            String Path = "";
            switch (Name)
            {
                case "ESTX 50":
                    Path = path + @"\MarketData\eurostoxx.csv";
                    break;
                case "S&P 500":
                    Path = path + @"\MarketData\snp500.csv";
                    break;
                case "HANG SENG INDEX":
                    Path = path + @"\MarketData\hsi.csv";
                    break;
                case "EURUSD":
                    Path = path + @"\MarketData\EURUSD.csv";
                    break;
                case "EURHKD":
                    Path = path + @"\MarketData\EURHKD.csv";
                    break;
            }

            string line;
            FileStream aFile = new FileStream(Path, FileMode.Open);
            StreamReader sr = new StreamReader(aFile);
            IndexValue index_value = new IndexValue(Name);
            // read data in line by line
            sr.ReadLine();
            while ((line = sr.ReadLine()) != null)
            {

                index_value.AddCotation(line.Split(','));
            }
            sr.Close();

            return index_value;
        }
    }
}
