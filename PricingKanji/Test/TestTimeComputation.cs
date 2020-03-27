using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using PricingKanji.Model;
using PricingLibrary.Utilities.MarketDataFeed;
using PricingLibrary.Utilities;
namespace PricingKanji
{
    class TestTimeComputation
    {
        static void Main(string[] args)
        {
            int estimationwindow = 60;
            int freq = 1;
            DateTime userDate = new DateTime(2013, 3, 20);
            Hedging hedging = new Hedging(estimationwindow, freq, userDate, false);
            var feeds = hedging.market.KanjiFeeds(hedging.market.feeds, hedging.startdate, hedging.maturity_date, false);

            foreach (var feed in feeds)
            {
                Console.WriteLine(feed.Date.ToString("d") + " " + Utilities.getTime(feed.Date, feeds, 8, 16));
            }
        }

    }
}
