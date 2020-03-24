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
            //PrixKanji();
            //CouvertureKanji();
            ViewBag.Posted = false;
            return View();
        }


        [HttpPost]
        public ActionResult Index(PriceFormModel priceFormModel)
        {
            ViewBag.Posted = true;
            int max = HomeController.hedging.market.PreviousFeeds(HomeController.hedging.market.feeds, HomeController.hedging.startdate).Count + 30;
            if (priceFormModel.Freq <= 0)
            {
                ViewBag.MessageErr = "La fréquence de rebalancement doit être superieur strictement à : 0";
            }
            else if (priceFormModel.EstimationWindow > max || priceFormModel.EstimationWindow <= 2)
            {
                ViewBag.MessageErr = "La fenêtre d'estimation doit être inferieur à : " + max + ", et superieur à : 3.";
            }  else {

                HomeController.hedging.estimationWindow = priceFormModel.EstimationWindow;
                HomeController.hedging.rebalancingFrequency = priceFormModel.Freq;
                Dictionary<DateTime, HedgeState> output = HomeController.hedging.HedgeKandji();
                Couverture(output);
                Dictionary<string, double> assetValues = output[HomeController.hedging.market.feeds.Last().Date].getAssetValues();
                Portfolio(assetValues);
                
                ViewBag.Freq = priceFormModel.Freq;
                ViewBag.EstWindow = priceFormModel.EstimationWindow;
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



        private void PrixKanji()
        {
            List<DataPoint> dataPoints = new List<DataPoint>();
            using (var reader = new StreamReader(path.Chemin + @"\WebComponent\Content\csv\Kanji.csv"))
            {
                NumberFormatInfo provider = new NumberFormatInfo();
                provider.NumberDecimalSeparator = ".";

                int compteur = 0;
                while (!reader.EndOfStream)
                {
                    var line = reader.ReadLine();

                    if (compteur > 0)
                    {
                        var values = line.Split(';');
                        dataPoints.Add(new DataPoint(values[0], Convert.ToDouble(values[1], provider)));
                    }
                    compteur++;
                }
            }


            ViewBag.PrixKanji = JsonConvert.SerializeObject(dataPoints);
        }


        private void CouvertureKanji()
        {
            List<DataPoint> dataPoints = new List<DataPoint>();
            List<DataPoint> percentages = new List<DataPoint>();
            string filePath = System.IO.Path.GetFullPath("Kanji.csv");
            string folder = Path.GetDirectoryName(Process.GetCurrentProcess().MainModule.FileName);
            string dir = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
            string file = dir + @"\TestDir\TestFile.txt";
            using (var reader = new StreamReader(path.Chemin + @"\WebComponent\Content\csv\Kanji.csv"))
            {
                NumberFormatInfo provider = new NumberFormatInfo();
                provider.NumberDecimalSeparator = ".";

                int compteur = 0;
                while (!reader.EndOfStream)
                {
                    var line = reader.ReadLine();

                    if (compteur > 0)
                    {
                        var values = line.Split(';');
                        dataPoints.Add(new DataPoint(values[0], Convert.ToDouble(values[2], provider)));
                    }
                    compteur++;
                }
            }

            /* TODO: Actual percentages in portfolio */
            percentages.Add(new DataPoint("Hang Seng", 60));
            percentages.Add(new DataPoint("Euronext", 30));
            percentages.Add(new DataPoint("S&P 500", 10));

            ViewBag.CouvertureKanji = JsonConvert.SerializeObject(dataPoints);
            ViewBag.PercentageCoverage = JsonConvert.SerializeObject(percentages);
        }

    }
}