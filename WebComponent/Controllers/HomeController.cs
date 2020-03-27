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
using PricingKanji.Model;
using WebComponent.Utilities;
using PricingLibrary.Utilities.MarketDataFeed;

namespace WebComponent.Controllers
{
    public class HomeController : Controller
    {
        public static bool modele = false;

        public static DateTime userDate = new DateTime(2020, 1, 16);

        public static Hedging hedging = new Hedging(80, 1, userDate, false);

        public Utilities.Path path = new Utilities.Path();

        [HttpGet]
        public ActionResult Index()
        {
            ViewBag.UserDate = HomeController.userDate.ToString("D", CultureInfo.CreateSpecificCulture("fr-FR"));
            return View();
        }

        [HttpPost]
        public ActionResult Index(ConfigFormModel configFormModel)
        {
            DataReader data = new DataReader();
            List<DataFeed> list;

            if (configFormModel.TauxChange == "FX")
            {
                modele = true;
            } 
            else
            {
                modele = false;
            }
            hedging = new Hedging(60, 1, userDate, modele);
            list = data.ReadData();
            if (PricingKanji.Model.Utilities.ContainsDate(list, configFormModel.DesiredDate))
            {
                userDate = configFormModel.DesiredDate;
            }
            else
            {
                ViewBag.MessageEr = "Pas d'information disponible pour cette date. Choisissez une autre.";
            }
                            
            ViewBag.UserDate = userDate.ToString("D", CultureInfo.CreateSpecificCulture("fr-FR"));
            
            return View();
        }


    }
}