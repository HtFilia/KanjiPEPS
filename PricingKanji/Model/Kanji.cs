using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PricingLibrary.Utilities.MarketDataFeed;
using Wrapper;

namespace PricingKanji.Model
{
    class Kanji
    {
        public Dictionary<string, double> InitialValues { get; set; }
        DateTime maturity;
        DateTime startDate;
        int nbTimeSteps;
        public int size;
        List<string> indexesName;
        public double NetAssetValue { get; set; } // valeur liquidiative

        public static List<DateTime> initialValueDates()
        {
            DateTime date0 = new DateTime(2013, 3, 26);
            DateTime date1 = new DateTime(2013, 3, 22);
            DateTime date2 = new DateTime(2013, 3, 25);
            List<DateTime> dates = new List<DateTime>() { date0, date1, date2 };
            return dates;
        }

        public Kanji(Market market, List<DateTime> initialValueDates, double netAssetValue_ = 100)
        {
            NetAssetValue = netAssetValue_;
            size = market.feeds.First().PriceList.Count;
            nbTimeSteps = 16;
            maturity = new DateTime(2021, 3, 26);
            startDate = new DateTime(2013, 3, 26);
            InitialValues = new Dictionary<string, double>();
            List<double> initialvalues = new List<double> { 0, 0, 0 };
            indexesName = new List<string> { "ESTX 50", "S&P 500", "HANG SENG INDEX" };
            int count;
            foreach (DateTime date in initialValueDates)
            {
                DataFeed feed = market.getFeed(date);
                count = 0;
                foreach(string name in indexesName)
                {
                    initialvalues[count] += (double)feed.PriceList[name]/initialValueDates.Count;
                    count++;
                }
                count = 0;
            }
            count = 0;
            foreach (string name in indexesName)
            {
                InitialValues[name] = initialvalues[count];
                count++;
            }

        }

        public void computeNetAssetValue(Market market, List<DateTime> dates)
        {
            List<double> prices = new List<double>();
            foreach (DateTime date in dates)
            {
                //DataFeed feed =
            }
        }
    }
}
