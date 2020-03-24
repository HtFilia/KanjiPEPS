using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PricingLibrary.FinancialProducts;
using PricingLibrary.Utilities;
using PricingLibrary.Utilities.MarketDataFeed;

namespace PricingKanji.Model
{
    class Portfolio
    {

        public Dictionary<string, double> composition;
        public double Value { get; set; }
        public double PrevValue { get; set; }

        public Portfolio()
        {

            this.composition = new Dictionary<string, double>();
            composition.Add("ESTX 50", 0);
            composition.Add("S&P 500", 0);
            composition.Add("HANG SENG INDEX", 0);
        }


        public void UpdateComposition(double[] deltas, DataFeed feed, DataFeed prevFeed, double optionPrice, DateTime startdate, Market market)
        {

            // computes the quantity of the zero coupons to be bought/sold.
            if (feed.Date == startdate)
            {
                Value = optionPrice;
            }
            else
            {
                GetValue(feed, prevFeed, startdate, market);
            }
            // updates the deltas of the stocks.
            Dictionary<string, double> newComposition = new Dictionary<string, double>();
            int shareindex = 0;
            foreach (string id in composition.Keys)
            {
                newComposition.Add(id, deltas[shareindex]);
                shareindex++;
            }

            composition = newComposition;
            PrevValue = Value;
        }

        public void GetValue(DataFeed feed, DataFeed prevFeed, DateTime startdate, Market market)
        {
            double stockvalue = 0;
            foreach (string stock in composition.Keys)
            {
                stockvalue += composition[stock] * (double)feed.PriceList[stock];
            }

            double prev_stockvalue = 0;
            foreach (string stock in composition.Keys)
            {
                prev_stockvalue += composition[stock] * (double)prevFeed.PriceList[stock];
            }

            double investment_time = Utilities.ComputeTime(prevFeed.Date, feed.Date, market);
            double factor = Math.Exp(Market.r * investment_time);
            double riskfree_part = factor * (PrevValue - prev_stockvalue); 
            Value = stockvalue + riskfree_part;
        }
    }
}
