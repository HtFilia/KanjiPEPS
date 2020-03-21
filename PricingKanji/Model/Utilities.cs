using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PricingKanji.Model
{
    public static class Utilities
    {
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



    }
}
