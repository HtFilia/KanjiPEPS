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

namespace WebComponent.Controllers
{
    public class HomeController : Controller
    {
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
        public ActionResult Index(DateFormModel requestForm)
        {
            if (requestForm.DesiredDate > new DateTime(2020, 1, 16) || requestForm.DesiredDate < new DateTime(2012, 1, 1))
            {
                ViewBag.MessageEr = "La date devra être entre l'année 2012 et le 16 Janvier de l'année 2020.";
            }
            else
            {
                userDate = requestForm.DesiredDate;
                ViewBag.UserDate = requestForm.DesiredDate.ToString("D", CultureInfo.CreateSpecificCulture("fr-FR"));
            }
            return View();
        }


    }
}