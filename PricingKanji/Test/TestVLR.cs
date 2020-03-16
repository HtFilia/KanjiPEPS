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
            List<DateTime> dates = new List<DateTime> { new DateTime(2013, 1, 11) };
            Kanji kanji = new Kanji(market, dates);
        }
    }
}
