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
        // GET: Coverage
        public ActionResult Index()
        {
            PrixKanji();
            CouvertureKanji();

            if (Request.HttpMethod == "POST" && Request.Form["Freq"] != null && Request.Form["EstimationWindow"] != null)
            {
                int freq = Convert.ToInt32(Request.Form["Freq"]);
                int estimationWidow = Convert.ToInt32(Request.Form["EstimationWindow"]);
                DataReader reader = new DataReader();
                List<DataFeed> data = reader.ReadData();
                int estimation_window = 80;
                double r = 0.01;
                Hedging hedging = new Hedging(estimation_window, freq, data);
                Dictionary<DateTime, HedgeOutput> output = hedging.HedgeKandji();

            }
            return View();
        }


        private void PrixKanji()
        {
            List<DataPoint> dataPoints = new List<DataPoint>();
            using (var reader = new StreamReader(@"C:\Users\anas\source\repos\PEPS\WebComponent\Content\csv\Kanji.csv"))
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
            using (var reader = new StreamReader(@"C:\Users\anas\source\repos\PEPS\WebComponent\Content\csv\Kanji.csv"))
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