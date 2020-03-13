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
            ViewBag.HangSeng = HangSeng();
            ViewBag.Stoxx50 = Stoxx50();
            ViewBag.SP500 = SP500();

            return View();
        }


        private string HangSeng()
        {

            string json = "{\"data\":{\"datasets\":[{\"data\":[";

            using (var reader = new StreamReader(@"C:\Users\Idriss Afra\Source\Repos\KanjiPEPS2\WebComponent\Content\csv\HangSeng.csv"))
            {

                int compteur = 0;
                while (!reader.EndOfStream)
                {
                    var line = reader.ReadLine();

                    if (compteur > 0)
                    {

                        var values = line.Split(',');
                        if (compteur == 1)
                            json = json + values[1];
                        else
                            json = json + ", " + values[1];
                    }
                    compteur++;

                }
                json = json + "]}]}}";
            }
            return json;
        }

        private string Stoxx50()
        {
            string json = "{\"data\":{\"datasets\":[{\"data\":[";

            using (var reader = new StreamReader(@"C:\Users\Idriss Afra\Source\Repos\KanjiPEPS2\WebComponent\Content\csv\Stoxx50.csv"))
            {

                int compteur = 0;
                while (!reader.EndOfStream)
                {
                    var line = reader.ReadLine();

                    if (compteur > 0)
                    {

                        var values = line.Split(',');
                        if (compteur == 1)
                            json = json + values[1];
                        else
                            json = json + ", " + values[1];
                    }
                    compteur++;

                }
                json = json + "]}]}}";
            }
            return json;
        }

        private string SP500()
        {
            string json = "{\"data\":{\"datasets\":[{\"data\":[";

            using (var reader = new StreamReader(@"C:\Users\Idriss Afra\Source\Repos\KanjiPEPS2\WebComponent\Content\csv\S&P500.csv"))
            {

                int compteur = 0;
                while (!reader.EndOfStream)
                {
                    var line = reader.ReadLine();

                    if (compteur > 0)
                    {

                        var values = line.Split(',');
                        if (compteur == 1)
                            json = json + values[1];
                        else
                            json = json + ", " + values[1];
                    }
                    compteur++;

                }
                json = json + "]}]}}";
            }
            return json;
        }

    }
}