using PricingLibrary.FinancialProducts;
using System;
using System.Collections.Generic;
using System.Linq;

namespace PricingKanji
{
    class Utilities
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

    }
}

