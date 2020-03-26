using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Wrapper;
using System.IO;
using PricingKanji.Model;
using PricingLibrary.Utilities.MarketDataFeed;
using PricingLibrary.Utilities;
namespace PricingKanji
{
    class TestEstimation
    {
        static void Main(string[] args)
        {
            WrapperClass wc = new WrapperClass();
            double[] spots = { 2000, 3000, 5000 };
            double[] trends = { 0.01, 0.02, 0.03 };
            double[] sigmas = { 0.2, 0.25, 0.3 };
            double[] correlation = {1, 0.1, 0.2, 0.1, 1,-0.3, 0.2, -0.3, 1};
            double r = 0.01;
            double maturity = 8;
            double t = 4;
            int nbHedging_dates = (int)maturity*252;
            wc.SimulMarket(t, maturity, nbHedging_dates, spots, trends, sigmas, correlation, r);
            double[] market = wc.getPath();
            List<DataFeed> feeds = new List<DataFeed>();
            DataFeed feed;
            DateTime day = DateTime.Today;
            Dictionary<string, decimal> PriceList = new Dictionary<string, decimal>();
            List<string> names = new List<string>();
            names.Add("EuroStoxx 50");
            names.Add("S&P 500");
            names.Add("Hang Seng");
            for (int i = 0; i < market.Length/3; i++)
            {
                PriceList = new Dictionary<string, decimal>();
                for (int j = 0; j < 3; j++)
                {
                    PriceList[names[j]] = (decimal)market[3*i + j];
                }
                feed = new DataFeed(day, PriceList);
                feeds.Add(feed);
                day = Utilities.AddBusinessDays(day,1);
            }
            //double[] estimated_volatilities = Calibration.Volatilities(feeds);
            //double[,] estimated_correlation = Calibration.CorrMatrix(feeds);
            //double[] trend = Calibration.Trends(feeds);
            var csv = new StringBuilder();
            var newLine = string.Format("N = {0} \n", feeds.Count);
            csv.Append(newLine);
            newLine = string.Format("sigmas = [{0}, {1}, {2}] \n", sigmas[0], sigmas[1], sigmas[2]);
            csv.Append(newLine);
            newLine = string.Format("correlations = [{0}, {1}, {2}] \n", correlation[1], correlation[2], correlation[5]);
            csv.Append(newLine);
            newLine = string.Format("trends = [{0}, {1}, {2}] \n", trends[0], trends[1], trends[2]);
            csv.Append(newLine);
            double[,] logreturns;
            double[,] corrMatrix;
            double[] sigma ;
            double[] trend;
            List<DataFeed> estimation_feeds = new List<DataFeed>();
            int Window = 60;
            int estimationWindow = 0;
            for (int i = 1; i < 7; i++)
            {
                csv.Append("\n");
                estimationWindow = i*Window;
                estimation_feeds = feeds.GetRange(feeds.Count - estimationWindow, estimationWindow);
                newLine = string.Format("Estimation market : Last {0} feeds \n", estimationWindow);
                if(i == 6)
                {
                    estimation_feeds = feeds;
                    newLine = string.Format("Estimation market : Whole market \n");
                }
                csv.Append(newLine);

                logreturns = Calibration.getLogReturns(estimation_feeds);
                corrMatrix = Calibration.getCorrelations(estimation_feeds);
                sigma = Calibration.getVolatilities(estimation_feeds);
                trend = Calibration.getTrend(estimation_feeds);
                newLine = string.Format("estimated sigmas = [{0}, {1}, {2}] \n", sigma[0], sigma[1], sigma[2]);
                csv.Append(newLine);
                newLine = string.Format("estimated correlations = [{0}, {1}, {2}] \n", corrMatrix[0, 1], corrMatrix[0, 2], corrMatrix[1, 2]);
                csv.Append(newLine);
                newLine = string.Format("estimated trends = [{0}, {1}, {2}] \n", trend[0], trend[1], trend[2]);
                csv.Append(newLine);
            }

            File.WriteAllText(@"../../../../Validation/validation_calibration.csv", csv.ToString());

            Console.WriteLine();

        }

    }
}
