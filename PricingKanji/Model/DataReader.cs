﻿using System;
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
                    PriceList.Add("S&P 500", SNPValues.cotations[date]);
                    PriceList.Add("HANG SENG INDEX", HSIValues.cotations[date]);
                    feeds.Add(feed);
                }
            }
            return feeds;
        }

        public List<DataFeed> ReadDataFX(double r_usd, double r_hkd)
        {
            List<DataFeed> feeds = new List<DataFeed>();
            IndexValue EuroValues = ParseFile("ESTX 50");
            IndexValue SNPValues = ParseFile("S&P 500");
            IndexValue HSIValues = ParseFile("HANG SENG INDEX");
            IndexValue USDEURValues = ParseFile("USDEUR");
            IndexValue HKDEURValues = ParseFile("HKDEUR");
            double t = 0;
            foreach (DateTime date in EuroValues.cotations.Keys)
            {
                if (SNPValues.cotations.ContainsKey(date) && HSIValues.cotations.ContainsKey(date) && USDEURValues.cotations.ContainsKey(date) && HKDEURValues.cotations.ContainsKey(date))
                {
                    Dictionary<string, decimal> PriceList = new Dictionary<string, decimal>();
                    DataFeed feed = new DataFeed(date, PriceList);
                    PriceList.Add("ESTX 50", EuroValues.cotations[date]);
                    PriceList.Add("USDEUR", USDEURValues.cotations[date] * (decimal)Math.Exp(r_usd*t));
                    PriceList.Add("S&P 500", SNPValues.cotations[date] * USDEURValues.cotations[date]);
                    PriceList.Add("HKDEUR", HKDEURValues.cotations[date] * (decimal)Math.Exp(r_usd * t));
                    PriceList.Add("HANG SENG INDEX", HSIValues.cotations[date] * HKDEURValues.cotations[date]);
                    feeds.Add(feed);
                    t += 1.0 / 252.0;
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
                    Path = "../../../../MarketData/^STOXX50E.csv";
                    break;
                case "S&P 500":
                    Path = "../../../../MarketData/^GSPC.csv";
                    break;
                case "HANG SENG INDEX":
                    Path = "../../../../MarketData/^HSI.csv";
                    break;
                case "USDEUR":
                    Path = "../../../../MarketData/USDEUR.csv";
                    break;
                case "HKDEUR":
                    Path = "../../../../MarketData/HKDEUR.csv";
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
