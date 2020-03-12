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
            if (Request.HttpMethod == "GET")
            {
                WrapperClass wrapper = new WrapperClass();
                wrapper.getPriceDeltaPerft(2, 0, new double[] { 100, 100, 100 }, 16, new double[] { 0.2, 0.2, 0.2 }, new double[] { 1, 0.1, 0.1, 0.1, 1, 0.1, 0.1, 0.1, 1}, 0.07);
                ViewBag.Price = wrapper.getPrice();

                List<string> dates = new List<string>();
                List<string> cours = new List<string>();
                using (var reader = new StreamReader(@"C:\Users\Idriss Afra\Source\Repos\KanjiPEPS2\WebComponent\Content\csv\HangSeng.csv"))
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
            return View("Index");
        } 

       
    }
}