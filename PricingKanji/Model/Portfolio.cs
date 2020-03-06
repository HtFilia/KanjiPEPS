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
        private double RiskFreeInvestment;
        private DateTime RiskFreeInvestmentDate;

        public Portfolio()
        {

            this.composition = new Dictionary<string, double>();

            composition.Add("ESTX 50", 0);
            composition.Add("S&P 500", 0);
            composition.Add("HANG SENG INDEX", 0);
        }

        public double GetValue(DataFeed market)
        {
            double stockvalue = 0;
            foreach (string stock in composition.Keys)
            {
                stockvalue += composition[stock] * (double)market.PriceList[stock];
            }
            int investment_time = DayCount.CountBusinessDays(RiskFreeInvestmentDate, market.Date);
            double riskfreevalue = RiskFreeRateProvider.GetRiskFreeRateAccruedValue(DayCount.ConvertToDouble(investment_time, 365)) * RiskFreeInvestment;

            return stockvalue + riskfreevalue;

        }

        public void UpdateComposition(double[] deltas, DataFeed market, double optionPrice, DateTime startdate)
        {
            // updates the deltas of the stocks.
            Dictionary<string, double> newComposition = new Dictionary<string, double>();
            int shareindex = 0;
            foreach (string id in composition.Keys)
            {
                newComposition.Add(id, deltas[shareindex]);
                shareindex++;
            }
            composition = newComposition;

            // computes the quantity of the zero coupons to be bought/sold.
            double stockvalue = 0;
            foreach (string stock in composition.Keys)
            {
                stockvalue += composition[stock] * (double)market.PriceList[stock];
            }
            RiskFreeInvestmentDate = market.Date;

            RiskFreeInvestment = GetValue(market) - stockvalue;
            if (market.Date == startdate)
            {
                RiskFreeInvestment = optionPrice - stockvalue;
            }
        }




    }
}
