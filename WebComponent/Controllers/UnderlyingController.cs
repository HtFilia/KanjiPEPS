using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using WebComponent.Models;

namespace WebComponent.Controllers
{
    public class UnderlyingController : Controller
    {

        public Utilities.Path path = new Utilities.Path();

        // GET: Underlying
        public ActionResult Index()
        {
            HangSeng();
            Stoxx50();
            SP500();

            return View();
        }


        private void HangSeng()
        {

            List<DataPoint> dataPoints = new List<DataPoint>();
            using (var reader = new StreamReader(path.Chemin + @"\WebComponent\Content\csv\HangSeng.csv"))
            {
                NumberFormatInfo provider = new NumberFormatInfo();
                provider.NumberDecimalSeparator = ".";

                int compteur = 0;
                while (!reader.EndOfStream)
                {
                    var line = reader.ReadLine();

                    if (compteur > 0)
                    {
                        var values = line.Split(',');
                        dataPoints.Add(new DataPoint(values[0], Convert.ToDouble(values[1], provider)));
                    }
                    compteur++;
                }
            }


            ViewBag.HangSeng = JsonConvert.SerializeObject(dataPoints);
        }

        private void Stoxx50()
        {
            List<DataPoint> dataPoints = new List<DataPoint>();
            using (var reader = new StreamReader(path.Chemin + @"\WebComponent\Content\csv\Stoxx50.csv"))
            {
                NumberFormatInfo provider = new NumberFormatInfo();
                provider.NumberDecimalSeparator = ".";

                int compteur = 0;
                while (!reader.EndOfStream)
                {
                    var line = reader.ReadLine();

                    if (compteur > 0)
                    {
                        var values = line.Split(',');
                        dataPoints.Add(new DataPoint(values[0], Convert.ToDouble(values[1], provider)));
                    }
                    compteur++;
                }
            }


            ViewBag.Stoxx50 = JsonConvert.SerializeObject(dataPoints);
        }

        private void SP500()
        {
            List<DataPoint> dataPoints = new List<DataPoint>();
            using (var reader = new StreamReader(path.Chemin + @"\WebComponent\Content\csv\S&P500.csv"))
            {
                NumberFormatInfo provider = new NumberFormatInfo();
                provider.NumberDecimalSeparator = ".";

                int compteur = 0;
                while (!reader.EndOfStream)
                {
                    var line = reader.ReadLine();

                    if (compteur > 0)
                    {
                        var values = line.Split(',');
                        dataPoints.Add(new DataPoint(values[0], Convert.ToDouble(values[1], provider)));
                    }
                    compteur++;
                }
            }


            ViewBag.SP500 = JsonConvert.SerializeObject(dataPoints);
        }

    }
}