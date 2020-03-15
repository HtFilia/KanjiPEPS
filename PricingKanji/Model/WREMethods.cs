using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using PricingLibrary;
using PricingLibrary.Utilities.MarketDataFeed;

namespace PricingKanji.Model
{
    class WREMethods
    {

        // import WRE dlls
        [DllImport(@"C:\Users\anas\source\repos\PEPS\wre-ensimag-c-4.1.dll", EntryPoint = "WREmodelingCov", CallingConvention = CallingConvention.Cdecl)]
        // declaration
        public static extern int WREmodelingCov(
            ref int returnsSize,
            ref int nbSec,
            double[,] secReturns,
            double[,] covMatrix,
            ref int info
        );

        [DllImport(@"C:\Users\anas\source\repos\PEPS\wre-ensimag-c-4.1.dll", EntryPoint = "WREmodelingLogReturns", CallingConvention = CallingConvention.Cdecl)]
        public static extern int WREmodelingLogReturns(
            ref int nbValues,
            ref int nbAssets,
            double[,] assetsValues,
            ref int horizon,
            double[,] assetsReturns,
            ref int info);
        [DllImport(@"C:\Users\anas\source\repos\PEPS\wre-ensimag-c-4.1.dll", EntryPoint = "WREmodelingCorr", CallingConvention = CallingConvention.Cdecl)]
        public static extern int WREmodelingCorr(
            ref int nbValues,
            ref int nbAssets,
            double[,] assetsReturns,
            double[,] corr,
            ref int info);

        [DllImport(@"C:\Users\anas\source\repos\PEPS\wre-ensimag-c-4.1.dll", EntryPoint = "WREanalysisExpostVolatility", CallingConvention = CallingConvention.Cdecl)]
        public static extern int WREanalysisExpostVolatility(
            ref int nbValues,
            ref int nbAssets,
            double[,] portfolioReturns,
            double[,] corr,
            ref int info);

        [DllImport(@"C:\Users\anas\source\repos\PEPS\wre-ensimag-c-4.1.dll", EntryPoint = "WREmodelingMeanReturn", CallingConvention = CallingConvention.Cdecl)]
        public static extern int WREmodelingMeanReturn(
        ref int nbDates,
        ref int nbAssets,
        double[,] HistoricalReturns,
        double[] MeanReturn,
        ref int info);


        // returns the matrix of the log returns estimated from a datafeed
        public static double[,] LogReturns(List<DataFeed> dataFeeds)
        {
            int nbValues = dataFeeds.Count;
            int nbAssets = dataFeeds.ElementAt(0).PriceList.Count;
            double[,] assetsValues = new double[nbValues, nbAssets];
            int keyCount;
            foreach (DataFeed feed in dataFeeds)
            {
                keyCount = 0;
                foreach (string key in feed.PriceList.Keys)
                {
                    assetsValues[dataFeeds.IndexOf(feed), keyCount] = (double)feed.PriceList[key];
                    keyCount++;
                }
            }
            int horizon = 1;

            double[,] assetsReturns = new double[nbValues - horizon, nbAssets];
            int info = 0;
            int res;
            res = WREmodelingLogReturns(ref nbValues, ref nbAssets, assetsValues, ref horizon, assetsReturns, ref info);
            if (res != 0)
            {
                if (res < 0)
                    throw new Exception("ERROR: WREmodelingLogReturns encountred a problem. See info parameter for more details");
                else
                    throw new Exception("WARNING: WREmodelingLogReturns encountred a problem. See info parameter for more details");
            }
            return assetsReturns;
        }


        // Computes the covariance matrix from the logreturns matrix of the assets
        public static double[,] ComputeCovarianceMatrix(double[,] logreturns)
        {
            int dataSize = logreturns.GetLength(0);
            int nbAssets = logreturns.GetLength(1);
            double[,] covMatrix = new double[nbAssets, nbAssets];
            int info = 0;
            int res;
            res = WREmodelingCov(ref dataSize, ref nbAssets, logreturns, covMatrix, ref info);
            if (res != 0)
            {
                if (res < 0)
                    throw new Exception("ERROR: WREmodelingCov encountred a problem. See info parameter for more details");
                else
                    throw new Exception("WARNING: WREmodelingCov encountred a problem. See info parameter for more details");
            }
            return covMatrix;
        }


        // Computes the correlation matrix from the 
        public static double[,] ComputeCorrelationMatrix(int nbValues, int nbAssets, double[,] logreturns)
        {
            double[,] corrMatrix = new double[nbAssets, nbAssets];
            int info = 0;
            int res;
            res = WREmodelingCorr(ref nbValues, ref nbAssets, logreturns, corrMatrix, ref info);
            if (res != 0)
            {
                if (res < 0)
                    throw new Exception("ERROR: WREmodelingCorrelation encountred a problem. See info parameter for more details");
                else
                    throw new Exception("WARNING: WREmodelingCorrelation encountred a problem. See info parameter for more details");
            }
            return corrMatrix;
        }

        public static double[] ComputeTrend(int nbDates, int nbAssets, double[,] HistoricalReturns)
        {
            double[] MeanReturn = new double[nbAssets];
            int info = 0;
            int res = WREmodelingMeanReturn(ref nbDates, ref nbAssets, HistoricalReturns, MeanReturn, ref info);
            if (res != 0)
            {
                if (res < 0)
                    throw new Exception("ERROR: WREmodelingCorrelation encountred a problem. See info parameter for more details");
                else
                    throw new Exception("WARNING: WREmodelingCorrelation encountred a problem. See info parameter for more details");
            }

            return MeanReturn;
        }

    }

}
