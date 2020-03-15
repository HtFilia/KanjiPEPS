using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using Wrapper;
using WebComponent.Models;
using Newtonsoft.Json;
using System.IO;
using System.Globalization;

namespace WebComponent.Controllers
{
    public class HomeController : Controller
    {

       

        public ActionResult Index()
        {
            KanjiQuot();
            KanjiMens();
            KanjiHebdo();

            


            if (Request.HttpMethod == "GET")
            {
                WrapperClass wrapper = new WrapperClass();
                wrapper.getPriceDeltaPerft(2, 0, new double[] { 100, 100, 100 }, 16, new double[] { 0.2, 0.2, 0.2 }, new double[] { 1, 0.1, 0.1, 0.1, 1, 0.1, 0.1, 0.1, 1}, 0.07);
                ViewBag.Price = wrapper.getPrice();
            }
            
            if (Request.HttpMethod == "POST")
            {
                int SampleNb = Convert.ToInt32(Request.Form["SampleNb"]);
                double Maturity = Convert.ToDouble(Request.Form["Maturity"]);
                double InitialPrice = Convert.ToDouble(Request.Form["InitialPrice"]);
                double Strike = Convert.ToDouble(Request.Form["Strike"]);
                double Volatility = Convert.ToDouble(Request.Form["Volatility"]);
                double RiskFreeRate = Convert.ToDouble(Request.Form["RiskFreeRate"]);
                WrapperClass wrapper = new WrapperClass();
                wrapper.getPriceCallEuro(Maturity, InitialPrice, Strike, Volatility, RiskFreeRate);
                ViewBag.Price = wrapper.getPrice();
            }
            return View();
        }


      

        // On test avec le cours Stoxx50 pour le moment, à changer dès qu'on a le csv des prix !!!!!!!!!!!!

        private void KanjiQuot()
        {
            List<DataPoint> dataPoints = new List<DataPoint>();
            using (var reader = new StreamReader(@"C:\Users\Fibo\source\repos\HtFilia\KanjiPEPS\WebComponent\Content\csv\Stoxx50.csv"))
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
                        dataPoints.Add(new DataPoint((Double)compteur - 1, Convert.ToDouble(values[1], provider)));
                    }
                    compteur++;
                }
            }


            ViewBag.KanjiQuot = JsonConvert.SerializeObject(dataPoints);
        }

        private void KanjiMens()
        {

            List<DataPoint> dataPoints = new List<DataPoint>();
            using (var reader = new StreamReader(@"C:\Users\Fibo\source\repos\HtFilia\KanjiPEPS\WebComponent\Content\csv\Stoxx50.csv"))
            {
                NumberFormatInfo provider = new NumberFormatInfo();
                provider.NumberDecimalSeparator = ".";

                int compteur = 0;
                while (!reader.EndOfStream)
                {
                    var line = reader.ReadLine();

                    if (compteur > 0)
                    {
                        if ((compteur-1) % 30 == 0)
                        {
                            var values = line.Split(',');
                            dataPoints.Add(new DataPoint((Double)compteur - 1, Convert.ToDouble(values[1], provider)));
                        }
                    }
                    compteur++;
                }
            }


            ViewBag.KanjiMens = JsonConvert.SerializeObject(dataPoints);
        }

        private void KanjiHebdo()
        {

            List<DataPoint> dataPoints = new List<DataPoint>();
            using (var reader = new StreamReader(@"C:\Users\Fibo\source\repos\HtFilia\KanjiPEPS\WebComponent\Content\csv\Stoxx50.csv"))
            {
                NumberFormatInfo provider = new NumberFormatInfo();
                provider.NumberDecimalSeparator = ".";

                int compteur = 0;
                while (!reader.EndOfStream)
                {
                    var line = reader.ReadLine();

                    if (compteur > 0)
                    {
                        if ((compteur-1) % 7 == 0)
                        {
                            var values = line.Split(',');
                            dataPoints.Add(new DataPoint((Double)compteur - 1, Convert.ToDouble(values[1], provider)));
                        }
                    }
                    compteur++;
                }
            }


            ViewBag.KanjiHebdo = JsonConvert.SerializeObject(dataPoints);
        }



    }
}