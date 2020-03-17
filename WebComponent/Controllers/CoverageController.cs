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
            }
            return View();
        }


        private void PrixKanji()
        {
            List<DataPoint> dataPoints = new List<DataPoint>();
            using (var reader = new StreamReader(@"C:\Users\Idriss Afra\Source\Repos\KanjiPEPS2\WebComponent\Content\csv\Kanji.csv"))
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
            using (var reader = new StreamReader(@"C:\Users\Idriss Afra\Source\Repos\KanjiPEPS2\WebComponent\Content\csv\Kanji.csv"))
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


            ViewBag.CouvertureKanji = JsonConvert.SerializeObject(dataPoints);
        }

    }
}