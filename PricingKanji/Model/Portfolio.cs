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

        private Dictionary<string, double> composition;
        public double Value { get; set; }

        public Portfolio()
        {

            this.composition = new Dictionary<string, double>();

            composition.Add("ESTX 50", 0);
            composition.Add("S&P 500", 0);
            composition.Add("HANG SENG INDEX", 0);
        }

        
        public void UpdateComposition(double[] deltas, DataFeed market, DataFeed prevMarket, double optionPrice, DateTime startdate)
        {

            // computes the quantity of the zero coupons to be bought/sold.
            if (market.Date == startdate)
            {
                Value = optionPrice;
            }
            else
            {
                Value = GetValue(market, prevMarket);
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
        }

        public double GetValue(DataFeed market, DataFeed prevMarket)
        {
                double stockvalue = 0;
                foreach (string stock in composition.Keys)
                {
                    stockvalue += composition[stock] * (double)market.PriceList[stock];
                }

                double prev_stockvalue = 0;
                foreach (string stock in composition.Keys)
                {
                    prev_stockvalue += composition[stock] * (double)prevMarket.PriceList[stock];
                }

                int investment_time = DayCount.CountBusinessDays(prevMarket.Date, market.Date);
                double factor = RiskFreeRateProvider.GetRiskFreeRateAccruedValue(DayCount.ConvertToDouble(investment_time, 365));
                double riskfree_part = factor * (Value - prev_stockvalue);
                return stockvalue + riskfree_part;
            }
    }
}
