﻿using Newtonsoft.Json;
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
            ViewBag.UserDate = HomeController.userDate;

            data();
            ViewBag.Posted = false;
            return View();
        }


        [HttpPost]
        public ActionResult Index(PriceFormModel priceFormModel)
        {
            ViewBag.Posted = true;
            int max = HomeController.hedging.market.PreviousFeeds(HomeController.hedging.market.feeds, HomeController.hedging.startdate).Count + 30;
            if (priceFormModel.EstimationWindow > max || priceFormModel.EstimationWindow <= 2)
            {
                ViewBag.MessageErreur = "La fenêtre d'estimation doit être inferieur à : " + max + ", et superieur à : 3.";
            }
            else
            {
                HomeController.hedging.estimationWindow = priceFormModel.EstimationWindow;
                HomeController.hedging.market.completeMarket(HomeController.hedging.maturity_date, priceFormModel.EstimationWindow);
                
                data();
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