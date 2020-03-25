using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace WebComponent.Utilities
{
    public class DateConverter
    {
        public static string AspDateToWebDate(DateTime date) 
        {
            return date.Day + "-" + date.Month + "-" + date.Year;
        }
    }
}