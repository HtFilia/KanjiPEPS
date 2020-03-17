using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PricingKanji.Model;

namespace PricingKanji.Test
{
    class TestVLR
    {
        static void Main(string[] args)
        {
            int estimation_window = 60;
            Market market = new Market();
            DateTime date = new DateTime(2013, 1, 11);
            List<DateTime> dates = Kanji.initialValueDates();
            Kanji kanji = new Kanji(market, dates);
            int freq = 1;
            Hedging hedging = new Hedging(estimation_window, freq, market, kanji);
            DateTime date1 = new DateTime(2014, 3, 17);
            double[] past = hedging.getPast(date1);
            double prix = hedging.price(date);
        }
    }
}
