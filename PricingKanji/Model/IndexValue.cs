using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PricingLibrary.Utilities.MarketDataFeed;

namespace PricingKanji.Model
{
    class IndexValue
    {
        public String Name { get; set; }
        public DateTime Date { get; set; }
        public double Close { get; set; }

        public Dictionary<DateTime, decimal> cotations { get; set; }
        

        public IndexValue(String name)
        {
            Name = name;
            cotations = new Dictionary<DateTime, decimal>();
        }
        public void AddCotation(string[] csvArray)
        {
            int close_indx = 4;
            if(csvArray[4] != "null")
            {
                cotations.Add(Convert.ToDateTime(csvArray[0]), (decimal)Double.Parse(csvArray[close_indx]/*.Replace(".", ",")*/));
            }
        }

    }


}
