using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PricingKanji.Model
{
    public static class Utilities
    {

        public const string path = @"C:\Users\Fibo\source\repos\HtFilia\KanjiPEPS";

        // Adds busniessDayCount days to the startDate
        public static DateTime AddBusinessDays(DateTime startDate, int businessDayCount)
        {
            DateTime tmp = startDate;
            int remainingdays = businessDayCount;
            while (remainingdays > 0)
            {
                tmp = tmp.AddDays(1);
                if (tmp.DayOfWeek < DayOfWeek.Saturday && tmp.DayOfWeek > DayOfWeek.Sunday)
                {
                    remainingdays--;
                }
            }
            return tmp;
        }

        public static DateTime RemoveBusinessDays(DateTime startDate, int businessDayCount)
        {
            DateTime tmp = startDate;
            int remainingdays = businessDayCount;
            while (remainingdays > 0)
            {
                tmp = tmp.AddDays(-1);
                if (tmp.DayOfWeek < DayOfWeek.Saturday && tmp.DayOfWeek > DayOfWeek.Sunday)
                {
                    remainingdays--;
                }
            }
            return tmp;
        }

        // Returns a date which is businessDayCount business days before the startdate
        public static DateTime countBusinessDaysInverse(DateTime startDate, int businessDayCount)
        {
            DateTime tmp = startDate;
            int counter = 0;
            while (counter < businessDayCount)
            {
                tmp = tmp.AddDays(-1);
                if ((tmp.DayOfWeek != DayOfWeek.Saturday) && (tmp.DayOfWeek != DayOfWeek.Sunday))
                {
                    counter++;
                }
            }
            return tmp;
        }

        // method transforming the weight of each share to a weight
        public static double[] normalizeWeights(List<int> weights)
        {
            double[] result = new double[weights.Count];
            double s = weights.Sum();
            for (int i = 0; i < weights.Count; i++)
            {
                result[i] = (double)weights.ElementAt(i) / s;
            }
            return result;
        }

        public static double[] ToDouble(this decimal[] arr) =>
                         Array.ConvertAll(arr, x => (double)x);

        public static double[,] DotProduct(double[] a, double[] b)
        {
            int n = a.Length;
            double[,] c = new double[n, n];
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    c[i, j] = a[i] * b[j];
                }
            }
            return c;
        }
        public static double[,] addMatrix(double[,] a, double[,] b)
        {
            double[,] matrix = new double[a.GetLength(0),a.GetLength(1)];
            for (int i = 0; i < a.GetLength(0); i++)
            {
                for (int j = 0; j < a.GetLength(1); j++)
                {
                    matrix[i, j] = a[i, j] + b[i, j];
                }
            }
            return matrix;
        }

        public static double[] subVect(double[] a, double[] b)
        {
            int n = a.Length;
            double[] vect = new double[n];
            for (int i = 0; i < n; i++)
            {
                vect[i] = a[i] - b[i];
            }
            return vect;
        }

        public static void multiplyScalar(double[,]a, double scalar)
        {
            for (int i = 0; i < a.GetLength(0); i++)
            {
                for (int j = 0; j < a.GetLength(1); j++)
                {
                    a[i, j] *= scalar;
                }
            }

        }

        public static double[] GetColumn(double[,] matrix, int columnNumber)
        {
            return Enumerable.Range(0, matrix.GetLength(0))
                    .Select(x => matrix[x, columnNumber])
                    .ToArray();
        }

        public static double[] GetRow(double [,] matrix, int rowNumber)
        {
            return Enumerable.Range(0, matrix.GetLength(1))
                    .Select(x => matrix[rowNumber, x])
                    .ToArray();
        }

        public static double ComputeTime(DateTime start, DateTime end, Market market)
        {
            int count = market.feeds.IndexOf(market.getFeed(end)) - market.feeds.IndexOf(market.getFeed(start));
            return count / 252.0;
        }

    }
}
