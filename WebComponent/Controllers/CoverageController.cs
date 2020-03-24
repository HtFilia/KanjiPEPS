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

namespace WebComponent.Controllers
{
    public class CoverageController : Controller
    {

        public Utilities.Path path = new Utilities.Path();

        // GET: Coverage
        [HttpGet]
        public ActionResult Index()
        {
            PrixKanji();
            CouvertureKanji();
            ViewBag.Posted = false;
            return View();
        }


        [HttpPost]
        public ActionResult Index(PriceFormModel priceFormModel)
        {
            DataReader reader = new DataReader();
            List<DataFeed> data = reader.ReadData();
            DateTime userDate = new DateTime(2013, 3, 20);
            Hedging hedging = new Hedging(priceFormModel.EstimationWindow, priceFormModel.Freq, userDate);
            Dictionary<DateTime, HedgeState> output = hedging.HedgeKandji();
            PrixKanji();
            CouvertureKanji();
            ViewBag.Posted = true;
            ViewBag.Freq = priceFormModel.Freq;
            ViewBag.EstWindow = priceFormModel.EstimationWindow;
            return View();
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