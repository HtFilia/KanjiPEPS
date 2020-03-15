﻿using System;
using System.Collections.Generic;
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
            WrapperClass wc = new WrapperClass();
            double[] spots = { 100, 100, 100 };
            double[] trends = { 0.01, 0.02, 0.03 };
            double[] sigmas = { 0.2, 0.25, 0.3 };
            double[] correlation = { 1, 0.1, 0.2, 0.1, 1, -0.3, 0.2, -0.3, 1 };
            double r = 0.01;
            double maturity = 4;
            double t = maturity;
            int nbHedging_dates = (int)maturity * 252;
            wc.SimulMarket(t, maturity, nbHedging_dates, spots, trends, sigmas, correlation, r);
            double[] market = wc.getPath();
            List<DataFeed> feeds = new List<DataFeed>();
            DataFeed feed;
            DateTime day = new DateTime(2013, 03, 22);
            Dictionary<string, decimal> PriceList = new Dictionary<string, decimal>();
            List<string> names = new List<string>();
            names.Add("EuroStoxx 50");
            names.Add("S&P 500");
            names.Add("Hang Seng");
            for (int i = 0; i < market.Length / 3; i++)
            {
                PriceList = new Dictionary<string, decimal>();
                for (int j = 0; j < 3; j++)
                {
                    PriceList[names[j]] = (decimal)market[3 * i + j];
                }
                feed = new DataFeed(day, PriceList);
                feeds.Add(feed);
                day = Utilities.AddBusinessDays(day, 1);
            }
            int estimationwindow = 60;
            DateTime maturityDate = new DateTime(2021, 03, 26);
            List<DataFeed> simulatedfeeds = Market.simulate(feeds, maturityDate, estimationwindow);
        }

    }
}
