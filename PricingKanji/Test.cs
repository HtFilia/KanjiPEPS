using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PricingKanji.Model;
using PricingLibrary.Utilities.MarketDataFeed;

namespace PricingKanji
{
    class Test
    {
        static void Main(string[] args)
        {
            DataReader reader = new DataReader();
            List<DataFeed> data = reader.ReadData();
            Pricing pricer = new Pricing();
            List<double> prices = pricer.PriceKanji(data, 0.07);
            Console.WriteLine("hi");
        }
    }
}
