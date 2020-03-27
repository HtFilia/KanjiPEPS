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
        public static double timespan = 1.0 / 252.0;

        public static double[,] getLogReturns(List<DataFeed> dataFeeds)
        {
            int nbValues = dataFeeds.Count;
            int nbAssets = dataFeeds.ElementAt(0).PriceList.Count;
            double[,] logreturns = new double[nbValues-1, nbAssets];
            double[] values;
            double[] prev_values = Utilities.ToDouble(dataFeeds[0].PriceList.Values.ToArray());
            for (int i = 1; i < nbValues; i++)
            {
                values = Utilities.ToDouble(dataFeeds[i].PriceList.Values.ToArray());
                for (int j = 0; j < nbAssets; j++)
                {
                    logreturns[i-1, j] = Math.Log(values[j]/prev_values[j]);
                }
                prev_values = values;
            }
            return logreturns;
        }

        public static double[,] getCovMatrix(List<DataFeed> dataFeeds)
        {
            int nbValues = dataFeeds.Count;
            int nbAssets = dataFeeds.ElementAt(0).PriceList.Count;
            double[,] logreturns = getLogReturns(dataFeeds);
            double[,] matrix = new double[nbAssets, nbAssets];
            double[] mean = new double[nbAssets];
            for (int j = 0; j < nbAssets; j++)
            {
                for (int i = 0; i < nbValues - 1; i++)
                {
                    mean[j] += logreturns[i,j] / (nbValues-1);
                }
            }
            double[] tmp;
            for (int k = 0; k < nbValues-1; k++)
            {
                tmp = Utilities.GetRow(logreturns, k);
                tmp = Utilities.subVect(tmp, mean);
                matrix=Utilities.addMatrix(matrix,Utilities.DotProduct(tmp,tmp));
            }
            Utilities.multiplyScalar(matrix, 1.0 / (nbValues - 2));
            return matrix;
        }

        public static double[] getVolatilities(List<DataFeed> dataFeeds)
        {
            int nbValues = dataFeeds.Count;
            int nbAssets = dataFeeds.ElementAt(0).PriceList.Count;
            double[] sigmas = new double[nbAssets];
            double[,] cov = getCovMatrix(dataFeeds);
            for (int i = 0; i < nbAssets; i++)
            {
                sigmas[i] = Math.Sqrt(cov[i, i] / timespan);
            }
            return sigmas;
        }

        public static double[,] getCorrelations(List<DataFeed> dataFeeds)
        {
            int nbAssets = dataFeeds.ElementAt(0).PriceList.Count;
            double[] sigmas = getVolatilities(dataFeeds);
            double[,] cov = getCovMatrix(dataFeeds);
            double[,] matrix = new double[nbAssets, nbAssets];
            for (int i = 0; i < nbAssets; i++)
            {
                for (int j = 0; j < nbAssets; j++)
                {
                    matrix[i, j] = cov[i, j] / (sigmas[i] * sigmas[j] * timespan);
                }
            }
            return matrix;
        }

        public static double[] getTrend(List<DataFeed> dataFeeds)
        {
            int nbValues = dataFeeds.Count;
            int nbAssets = dataFeeds.ElementAt(0).PriceList.Count;
            double[,] logreturns = getLogReturns(dataFeeds);
            double[] vect = new double[nbAssets];
            double[] mean = new double[nbAssets];
            double[] sigmas = getVolatilities(dataFeeds);
            for (int j = 0; j < nbAssets; j++)
            {
                for (int i = 0; i < nbValues - 1; i++)
                {
                    mean[j] += logreturns[i, j] / (nbValues - 1);
                }
            }
            for (int i = 0; i < nbAssets; i++)
            {
                vect[i] = mean[i] / timespan + sigmas[i] * sigmas[i] * 0.5;
            }
            return vect;
        }

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
                volatilities[diagIndex] = Math.Sqrt(covM[diagIndex, diagIndex] / timespan);
                diagIndex++;
            }
            return volatilities;
        }
        public static double[] Trends(List<DataFeed> dataFeeds)
        {
            int nbAssets = dataFeeds.ElementAt(0).PriceList.Keys.Count;
            int nbDates = dataFeeds.Count;
            double[] meanReturn = new double[nbAssets];
            double[] trends = new double[nbAssets];
            double[,] returns = WREMethods.arithmeticReturns(dataFeeds);
            meanReturn = WREMethods.ComputeTrend(nbDates, nbAssets, returns);
            for (int i = 0; i < nbAssets; i++)
            {
                trends[i] = Math.Log(1 + meanReturn[i]) / timespan;
            }
            return trends;
        }

        public static double[] Trends(List<DataFeed> dataFeeds, double[] Volatilities)
        {
            int nbAssets = dataFeeds.ElementAt(0).PriceList.Keys.Count;
            int nbDates = dataFeeds.Count;
            double[] meanLogReturn = new double[nbAssets];
            double[] trends = new double[nbAssets];
            double[,] logreturns = WREMethods.LogReturns(dataFeeds);
            meanLogReturn = WREMethods.ComputeMeanLogReturn(nbDates, nbAssets, logreturns);
            for (int i = 0; i < nbAssets; i++)
            {
                trends[i] = meanLogReturn[i] / timespan + 0.5 * Volatilities[i] * Volatilities[i];
            }
            return trends;
        }

    }

}
