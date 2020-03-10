using PricingLibrary.Utilities.MarketDataFeed;
using PricingLibrary.Utilities;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace PricingKanji.Model
{
    class Calibration
    {
        // Estimated assets covariance matrix from a datafeed
        public static double[,] CovMatrix(List<DataFeed> dataFeeds)
        {
            double[,] logreturns = WREMethods.LogReturns(dataFeeds);
            // call WRE via computeCovarianceMatrix encapsulation
            double[,] myCovMatrix = WREMethods.ComputeCovarianceMatrix(logreturns);
            return myCovMatrix;
        }

        // Estimated assets correlation matrix from a datafeed
        public static double[,] CorrMatrix(List<DataFeed> dataFeeds)
        {
            double[,] logreturns = WREMethods.LogReturns(dataFeeds);
            // call WRE via computeCovarianceMatrix encapsulation
            double[,] myCorrMatrix = WREMethods.ComputeCorrelationMatrix(dataFeeds.Count, dataFeeds.ElementAt(0).PriceList.Count, logreturns);
            return myCorrMatrix;
        }

        // Estimated assets volatilities from a datafeed
        public static double[] Volatilities(List<DataFeed> dataFeeds)
        {
            double[,] covM = CovMatrix(dataFeeds);
            double[] volatilities = new double[dataFeeds.ElementAt(0).PriceList.Keys.Count];
            int diagIndex = 0;
            while (diagIndex < volatilities.Length)
            {
                volatilities[diagIndex] = Math.Sqrt(covM[diagIndex, diagIndex] *252 );
                diagIndex++;
            }
            return volatilities;
        }
        public static double[] Trends(List<DataFeed> dataFeeds)
        {
            int nbAssets = dataFeeds.ElementAt(0).PriceList.Keys.Count;
            int nbDates = dataFeeds.Count;
            double[] MeanReturn = new double[nbAssets];
            double[,] logreturns = WREMethods.LogReturns(dataFeeds);
            MeanReturn = WREMethods.ComputeTrend(nbDates, nbAssets, logreturns);
            return MeanReturn;
        }
    }

}
