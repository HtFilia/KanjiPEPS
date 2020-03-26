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
        public ActionResult Index(PriceFormModel priceFormModel)
        {
            DataReader data = new DataReader();
            List<DataFeed> list;
            if (priceFormModel.Modele != "FX" && priceFormModel.Modele != null)
            {
                ViewBag.MessageEr = "Tapez FX ou laisser le vide.";
            }
            else if (priceFormModel.Modele == "FX")
            {
                hedging = new Hedging(60, 1, userDate, true);
                modele = true;
                list = data.ReadDataFX();
                if(PricingKanji.Model.Utilities.containsDate(list, priceFormModel.DesiredDate))
                {
                    userDate = priceFormModel.DesiredDate;
                } else
                {
                    ViewBag.MessageEr = "Pas d'information disponible pour cette date. Choisissez une autre.";
                }
            } 
            else
            {
                hedging = new Hedging(60, 1, userDate, false);
                modele = false;
                list = data.ReadData();
                if (PricingKanji.Model.Utilities.containsDate(list, priceFormModel.DesiredDate))
                {
                    userDate = priceFormModel.DesiredDate;
                }
                else
                {
                    ViewBag.MessageEr = "Pas d'information disponible pour cette date. Choisissez une autre."; 
                }
            }
                            
            ViewBag.UserDate = priceFormModel.DesiredDate.ToString("D", CultureInfo.CreateSpecificCulture("fr-FR"));
            
            return View();
        }


    }
}