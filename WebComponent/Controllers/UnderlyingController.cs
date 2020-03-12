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
        // GET: Underlying
        public ActionResult Index()
        {
            List<string> dates = new List<string>();
            List<string> cours = new List<string>();
            using (var reader = new StreamReader(@"C:\Users\Idriss Afra\Source\Repos\KanjiPEPS2\WebComponent\Content\csv\Stoxx50.csv"))
            {

                int titres = 0;
                while (!reader.EndOfStream)
                {
                    var line = reader.ReadLine();

                    if (titres > 0)
                    {

                        var values = line.Split(',');
                        dates.Add(values[0]);
                        cours.Add(values[4]);
                    }
                    titres++;

                }
            }

            List<DataPoint> dataPoints = new List<DataPoint>();
            NumberFormatInfo provider = new NumberFormatInfo();
            provider.NumberGroupSeparator = ".";
            for (int i = 0; i < dates.Count; i++)
            {
                dataPoints.Add(new DataPoint(i, Convert.ToDouble(cours[i], provider)));
            }

            ViewBag.DataPoints = JsonConvert.SerializeObject(dataPoints);
            return View();
        }
    }
}