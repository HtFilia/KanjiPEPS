using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PricingKanji.Model;
using PricingLibrary.Utilities.MarketDataFeed;
using Wrapper;

namespace PricingKanji
{
    class Test
    {
        static void Main(string[] args)
        {
            WrapperClass wc = new WrapperClass();
            wc.getPriceCallEuro(50000, 1, 100, 100, 0.2, 0.01);
            Console.WriteLine(wc.getPrice());
            //DataReader reader = new DataReader();
            //List<DataFeed> data = reader.ReadData();
            //Pricing pricer = new Pricing();
            //var prices = pricer.PriceKanji(data, 0.07).Values;
            //Console.WriteLine("hi");
        }
    }
}
