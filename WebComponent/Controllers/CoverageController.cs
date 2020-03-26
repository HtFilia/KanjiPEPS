using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using WebComponent.Models;
using PricingLibrary.FinancialProducts;
using PricingKanji.Model;
using PricingLibrary.Utilities.MarketDataFeed;
using System.Diagnostics;
using Microsoft.Ajax.Utilities;

namespace WebComponent.Controllers
{
    public class CoverageController : Controller
    {

        public Utilities.Path path = new Utilities.Path();

        // GET: Coverage
        [HttpGet]
        public ActionResult Index()
        {
            ViewBag.UserDate = HomeController.userDate.ToString("D", CultureInfo.CreateSpecificCulture("fr-FR"));
            return View();
        }


        [HttpPost]
        public ActionResult Index(PriceFormModel priceFormModel)
        {
            ViewBag.UserDate = HomeController.userDate.ToString("D", CultureInfo.CreateSpecificCulture("fr-FR"));
            int max = HomeController.hedging.market.PreviousFeeds(HomeController.hedging.market.feeds, HomeController.hedging.startdate).Count + 30;
            if (priceFormModel.Freq <= 0)
            {
                ViewBag.MessageErr = "La fréquence de rebalancement doit être superieur strictement à : 0";
            }
            else if (priceFormModel.EstimationWindow > max || priceFormModel.EstimationWindow <= 2)
            {
                ViewBag.MessageErr = "La fenêtre d'estimation doit être inferieur à : " + max + ", et superieur à : 3.";
            } else if (priceFormModel.CompositionDate > HomeController.hedging.market.feeds.Last().Date || priceFormModel.CompositionDate < HomeController.hedging.startdate)
            {
                ViewBag.MessageErr = "La date doit être entre : " + HomeController.hedging.startdate.ToString("d") + " et " + HomeController.hedging.market.feeds.Last().Date.ToString("d");
            } else {

                HomeController.hedging.estimationWindow = priceFormModel.EstimationWindow;
                HomeController.hedging.rebalancingFrequency = priceFormModel.Freq;
                Dictionary<DateTime, HedgeState> output = HomeController.hedging.HedgeKandji();
                Couverture(output);
                Dictionary<string, double> assetValues = output[priceFormModel.CompositionDate].getAssetValues();
                Portfolio(assetValues);      
            }
            
            return View();
        }


        public void Couverture(Dictionary<DateTime, HedgeState> output)
        {
            List<DataPoint> pricePoints = new List<DataPoint>();
            List<DataPoint> covePoints = new List<DataPoint>();
            List<DataPoint> deltasHG = new List<DataPoint>();
            List<DataPoint> deltasSP = new List<DataPoint>();
            List<DataPoint> deltasEur = new List<DataPoint>();
            foreach (var element in output) {
                pricePoints.Add(new DataPoint(element.Key.ToString("d"), element.Value.optionValue));
                covePoints.Add(new DataPoint(element.Key.ToString("d"), element.Value.portfolioValue));
                deltasHG.Add(new DataPoint(element.Key.ToString("d"), element.Value.composition["HANG SENG INDEX"]));
                deltasSP.Add(new DataPoint(element.Key.ToString("d"), element.Value.composition["S&P 500"]));
                deltasEur.Add(new DataPoint(element.Key.ToString("d"), element.Value.composition["ESTX 50"]));
            }

            ViewBag.PrixKanji = JsonConvert.SerializeObject(pricePoints);
            ViewBag.CouvertureKanji = JsonConvert.SerializeObject(covePoints);
            ViewBag.DeltasHG = JsonConvert.SerializeObject(deltasHG);
            ViewBag.DeltasSP = JsonConvert.SerializeObject(deltasSP);
            ViewBag.DeltasEur = JsonConvert.SerializeObject(deltasEur);
        }


        public void Portfolio(Dictionary<String, double> assetValues)
        {

            List<DataPoint> percentages = new List<DataPoint>();
            foreach (var element in assetValues)
            {
                percentages.Add(new DataPoint(element.Key, element.Value));
            }

            ViewBag.PercentageCoverage = JsonConvert.SerializeObject(percentages);
        }



      


       

    }
}