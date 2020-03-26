using Newtonsoft.Json;
using PricingLibrary.Utilities.MarketDataFeed;
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
        [HttpGet]
        public ActionResult Index()
        {
            ViewBag.Modele = HomeController.modele;
            ViewBag.UserDate = HomeController.userDate.ToString("D", CultureInfo.CreateSpecificCulture("fr-FR"));
            if (HomeController.modele == false)
            {
                data();
            } else
            {
                dataFX();
            }
            
            return View();
        }


        [HttpPost]
        public ActionResult Index(PriceFormModel priceFormModel)
        {
            ViewBag.Modele = HomeController.modele;
            ViewBag.UserDate = HomeController.userDate.ToString("D", CultureInfo.CreateSpecificCulture("fr-FR"));
            int max = HomeController.hedging.market.PreviousFeeds(HomeController.hedging.market.feeds, HomeController.hedging.startdate).Count + 30;
            if (priceFormModel.EstimationWindow > max || priceFormModel.EstimationWindow <= 2)
            {
                ViewBag.MessageErreur = "La fenêtre d'estimation doit être inferieur à : " + max + ", et superieur à : 3.";
            }
            else
            {
                HomeController.hedging.estimationWindow = priceFormModel.EstimationWindow;
                HomeController.hedging.market.completeMarket(HomeController.hedging.maturity_date, priceFormModel.EstimationWindow);

                if (HomeController.modele == false)
                {
                    data();
                }
                else
                {
                    dataFX();
                }
            }
            return View();
        }

        private void data()
        {

            List<DataFeed> data = HomeController.hedging.market.feeds;
            List<DataPoint> HGPoints = new List<DataPoint>();
            List<DataPoint> SPPoints = new List<DataPoint>();
            List<DataPoint> EURPoints = new List<DataPoint>();
            foreach (var feed in data)
            {
                HGPoints.Add(new DataPoint(feed.Date.ToString("d"), (double) feed.PriceList["HANG SENG INDEX"]));
                SPPoints.Add(new DataPoint(feed.Date.ToString("d"), (double)feed.PriceList["S&P 500"]));
                EURPoints.Add(new DataPoint(feed.Date.ToString("d"), (double)feed.PriceList["ESTX 50"]));
            }
            ViewBag.HangSeng = JsonConvert.SerializeObject(HGPoints);
            ViewBag.Stoxx50 = JsonConvert.SerializeObject(EURPoints);
            ViewBag.SP500 = JsonConvert.SerializeObject(SPPoints);
        }


        private void dataFX()
        {

            List<DataFeed> data = HomeController.hedging.market.feeds;
            List<DataPoint> HGPoints = new List<DataPoint>();
            List<DataPoint> SPPoints = new List<DataPoint>();
            List<DataPoint> EURPoints = new List<DataPoint>();
            List<DataPoint> USDPoints = new List<DataPoint>();
            List<DataPoint> HKDPoints = new List<DataPoint>();
            foreach (var feed in data)
            {
                HGPoints.Add(new DataPoint(feed.Date.ToString("d"), (double)feed.PriceList["HANG SENG INDEX"]));
                SPPoints.Add(new DataPoint(feed.Date.ToString("d"), (double)feed.PriceList["S&P 500"]));
                EURPoints.Add(new DataPoint(feed.Date.ToString("d"), (double)feed.PriceList["ESTX 50"]));
                USDPoints.Add(new DataPoint(feed.Date.ToString("d"), (double)feed.PriceList["USDEUR"]));
                HKDPoints.Add(new DataPoint(feed.Date.ToString("d"), (double)feed.PriceList["HKDEUR"]));
            }


            ViewBag.HangSeng = JsonConvert.SerializeObject(HGPoints);
            ViewBag.Stoxx50 = JsonConvert.SerializeObject(EURPoints);
            ViewBag.SP500 = JsonConvert.SerializeObject(SPPoints);
            ViewBag.USD = JsonConvert.SerializeObject(USDPoints);
            ViewBag.HKD = JsonConvert.SerializeObject(HKDPoints);
        }



    }
}