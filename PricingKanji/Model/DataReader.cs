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
        public string ProjectPath = Utilities.GetProjectPath();

        public List<DataFeed> ReadData()
        {
            List<DataFeed> feeds = new List<DataFeed>();
            IndexValue EuroValues = ParseFile("ESTX 50");
            IndexValue SNPValues = ParseFile("S&P 500");
            IndexValue HSIValues = ParseFile("HANG SENG INDEX");
            foreach (DateTime date in EuroValues.cotations.Keys)
            {
                if (SNPValues.cotations.ContainsKey(date) && HSIValues.cotations.ContainsKey(date))
                {
                    Dictionary<string, decimal> PriceList = new Dictionary<string, decimal>();
                    DataFeed feed = new DataFeed(date, PriceList);
                    PriceList.Add("ESTX 50", EuroValues.cotations[date]);
                    PriceList.Add("S&P 500", SNPValues.cotations[date]*(decimal)Market.USDEUR);
                    PriceList.Add("HANG SENG INDEX", HSIValues.cotations[date]*(decimal)Market.HKDEUR);
                    feeds.Add(feed);
                }
            }
            return feeds;
        }

        public List<DataFeed> ReadDataFX()
        {
            List<DataFeed> feeds = new List<DataFeed>();
            IndexValue EuroValues = ParseFile("ESTX 50");
            IndexValue SNPValues = ParseFile("S&P 500");
            IndexValue HSIValues = ParseFile("HANG SENG INDEX");
            IndexValue USDEURValues = ParseFile("USDEUR");
            IndexValue HKDEURValues = ParseFile("HKDEUR");
            foreach (DateTime date in EuroValues.cotations.Keys)
            {
                if (SNPValues.cotations.ContainsKey(date) && HSIValues.cotations.ContainsKey(date) && USDEURValues.cotations.ContainsKey(date) && HKDEURValues.cotations.ContainsKey(date))
                {
                    Dictionary<string, decimal> PriceList = new Dictionary<string, decimal>();
                    DataFeed feed = new DataFeed(date, PriceList);
                    PriceList.Add("ESTX 50", EuroValues.cotations[date]);
                    PriceList.Add("USDEUR", USDEURValues.cotations[date] );
                    PriceList.Add("S&P 500", SNPValues.cotations[date] * USDEURValues.cotations[date]);
                    PriceList.Add("HKDEUR", HKDEURValues.cotations[date]);
                    PriceList.Add("HANG SENG INDEX", HSIValues.cotations[date] * HKDEURValues.cotations[date]);
                    feeds.Add(feed);
                }
            }

            return feeds;
        }

        public IndexValue ParseFile(String Name)
        {
            String Path = "";
            switch (Name)
            {
                case "ESTX 50":
                    Path = ProjectPath + @"\MarketData\^STOXX50E.csv";
                    break;
                case "S&P 500":
                    Path = ProjectPath + @"\MarketData\^GSPC.csv";
                    break;
                case "HANG SENG INDEX":
                    Path = ProjectPath + @"\MarketData\^HSI.csv";
                    break;
                case "USDEUR":
                    Path = ProjectPath + @"\MarketData\USDEUR.csv";
                    break;
                case "HKDEUR":
                    Path = ProjectPath + @"\MarketData\HKDEUR.csv";
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
