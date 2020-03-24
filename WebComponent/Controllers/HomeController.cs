﻿using System;
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
        public static DateTime userDate = DateTime.Today;

        public static Hedging hedging = new Hedging(80, 1, userDate);

        public Utilities.Path path = new Utilities.Path();

        [HttpGet]
        public ActionResult Index()
        {
            KanjiQuot();
            KanjiMens();
            KanjiHebdo();
            ViewBag.UserDate = HomeController.userDate;
            ViewBag.Posted = false;
            /*if (Request.HttpMethod == "GET")
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
            }*/
            return View();
        }

        [HttpPost]
        public ActionResult Index(DateFormModel requestForm)
        {
            KanjiQuot();
            KanjiMens();
            KanjiHebdo();
            ViewBag.Posted = true;
            userDate = requestForm.DesiredDate;
            ViewBag.UserDate = requestForm.DesiredDate;
            return View();
        }


        // On test avec le cours Stoxx50 pour le moment, à changer dès qu'on a le csv des prix !!!!!!!!!!!!

        private void KanjiQuot()
        {
            List<DataPoint> dataPoints = new List<DataPoint>();
            using (var reader = new StreamReader(path.Chemin + @"\WebComponent\Content\csv\Kanji.csv"))
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


            ViewBag.KanjiQuot = JsonConvert.SerializeObject(dataPoints);
        }

        private void KanjiMens()
        {

            List<DataPoint> dataPoints = new List<DataPoint>();
            using (var reader = new StreamReader(path.Chemin + @"\WebComponent\Content\csv\Kanji.csv"))
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
                            var values = line.Split(';');
                            dataPoints.Add(new DataPoint(values[0], Convert.ToDouble(values[1], provider)));
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
            using (var reader = new StreamReader(path.Chemin + @"\WebComponent\Content\csv\Kanji.csv"))
            {
                NumberFormatInfo provider = new NumberFormatInfo
                {
                    NumberDecimalSeparator = "."
                };

                int compteur = 0;
                while (!reader.EndOfStream)
                {
                    var line = reader.ReadLine();
                    if (compteur > 0)
                    {
                        if ((compteur-1) % 7 == 0)
                        {
                            var values = line.Split(';');
                            dataPoints.Add(new DataPoint(values[0], Convert.ToDouble(values[1], provider)));
                        }
                    }
                    compteur++;
                }
            }


            ViewBag.KanjiHebdo = JsonConvert.SerializeObject(dataPoints);
        }



    }
}