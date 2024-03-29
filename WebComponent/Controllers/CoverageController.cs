﻿using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using WebComponent.Models;
using PricingLibrary.FinancialProducts;
using PricingKanji.Model;
using PricingLibrary.Utilities.MarketDataFeed;
using System.Diagnostics;
using Microsoft.Ajax.Utilities;

namespace WebComponent.Controllers
{
    public class CoverageController : Controller
    {

        public Utilities.Path path = new Utilities.Path();
        static double FixedFreq = 0;
        static double FixedEstimWindow = 0;
        static DateTime FixedDate;
        static double test = 0;
        static double vlr = 0;
        static Dictionary<DateTime, HedgeState> output;

        // GET: Coverage
        [HttpGet]
        public ActionResult Index()
        {
            ViewBag.UserDate = HomeController.userDate.ToString("D", CultureInfo.CreateSpecificCulture("fr-FR"));
            ViewBag.Modele = HomeController.modele;
            return View();
        }


        [HttpPost]
        public ActionResult Index(PriceFormModel priceFormModel)
        {
            ViewBag.Modele = HomeController.modele;
            ViewBag.UserDate = HomeController.userDate.ToString("D", CultureInfo.CreateSpecificCulture("fr-FR"));
            List<DataFeed> VLR_feeds =  HomeController.hedging.kanji.netassetValueFeeds(HomeController.hedging.market.feeds);
            int max = HomeController.hedging.market.PreviousFeeds(HomeController.hedging.market.feeds, HomeController.hedging.startdate).IndexOf(VLR_feeds.First());
                if (!PricingKanji.Model.Utilities.ContainsDate(HomeController.hedging.market.KanjiFeeds(HomeController.hedging.market.feeds, HomeController.hedging.startdate, HomeController.hedging.maturity_date, HomeController.hedging.FX), priceFormModel.CompositionDate))
                {
                    ViewBag.MessageErr = "Pas d'information disponible pour cette date. Choisissez une autre.";
                }
                else if (priceFormModel.Freq <= 0)
                {
                    ViewBag.MessageErr = "La fréquence de rebalancement doit être superieur strictement à : 0";
                }
                else if (priceFormModel.EstimationWindow > max || priceFormModel.EstimationWindow <= 2)
                {
                    ViewBag.MessageErr = "La fenêtre d'estimation doit être inferieur à : " + max + ", et superieur à : 3.";
                }
                else
                {
                    
                    
                    if (test == 0)
                    {
                        FixedFreq = priceFormModel.Freq;
                        FixedEstimWindow = priceFormModel.EstimationWindow;
                        FixedDate = priceFormModel.CompositionDate;
                        HomeController.hedging.estimationWindow = priceFormModel.EstimationWindow;
                        HomeController.hedging.rebalancingFrequency = priceFormModel.Freq;
                        output = HomeController.hedging.HedgeKandji();
                        vlr = HomeController.hedging.kanji.NetAssetValue;
                        ViewBag.Vlr = vlr;
                        Dictionary<string, double> assetValues = output[priceFormModel.CompositionDate].getAssetValues();
                        Portfolio(assetValues);
                        test++;
                    }
                    else
                    {
                        if (((FixedFreq != priceFormModel.Freq || FixedEstimWindow != priceFormModel.EstimationWindow) && FixedDate == priceFormModel.CompositionDate) || ((FixedFreq != priceFormModel.Freq || FixedEstimWindow != priceFormModel.EstimationWindow) && FixedDate != priceFormModel.CompositionDate))
                        {
                            FixedFreq = priceFormModel.Freq;
                            FixedEstimWindow = priceFormModel.EstimationWindow;
                            FixedDate = priceFormModel.CompositionDate;
                            HomeController.hedging.estimationWindow = priceFormModel.EstimationWindow;
                            HomeController.hedging.rebalancingFrequency = priceFormModel.Freq;
                            output = HomeController.hedging.HedgeKandji();
                            vlr = HomeController.hedging.kanji.NetAssetValue;
                            ViewBag.Vlr = vlr;
                            Dictionary<string, double> assetValues = output[priceFormModel.CompositionDate].getAssetValues();
                            Portfolio(assetValues);
                        }
                        else if ((FixedFreq == priceFormModel.Freq && FixedEstimWindow == priceFormModel.EstimationWindow) || FixedDate != priceFormModel.CompositionDate)
                        {
                            ViewBag.Vlr = vlr;
                            Dictionary<string, double> assetValues = output[priceFormModel.CompositionDate].getAssetValues();
                            Portfolio(assetValues);
                        }
                    }

                    if (HomeController.modele == false)
                    {
                        Couverture(output);
                    }
                    else
                    {
                        CouvertureFX(output);
                    }
                }
            ViewBag.TrErr = (output.Values.Last().optionValue - output.Values.Last().portfolioValue) / output.Values.Last().portfolioValue * 100;
                return View();
        }


        public void Couverture(Dictionary<DateTime, HedgeState> output)
        {
            List<DataPoint> pricePoints = new List<DataPoint>();
            List<DataPoint> covePoints = new List<DataPoint>();
            List<DataPoint> deltasHG = new List<DataPoint>();
            List<DataPoint> deltasSP = new List<DataPoint>();
            List<DataPoint> deltasEur = new List<DataPoint>();
            foreach (var element in output) {
                pricePoints.Add(new DataPoint(element.Key.ToString("d"), element.Value.optionValue));
                covePoints.Add(new DataPoint(element.Key.ToString("d"), element.Value.portfolioValue));
                deltasHG.Add(new DataPoint(element.Key.ToString("d"), element.Value.composition["HANG SENG INDEX"]));
                deltasSP.Add(new DataPoint(element.Key.ToString("d"), element.Value.composition["S&P 500"]));
                deltasEur.Add(new DataPoint(element.Key.ToString("d"), element.Value.composition["ESTX 50"]));
            }

            ViewBag.PrixKanji = JsonConvert.SerializeObject(pricePoints);
            ViewBag.CouvertureKanji = JsonConvert.SerializeObject(covePoints);
            ViewBag.DeltasHG = JsonConvert.SerializeObject(deltasHG);
            ViewBag.DeltasSP = JsonConvert.SerializeObject(deltasSP);
            ViewBag.DeltasEur = JsonConvert.SerializeObject(deltasEur);
        }


        public void CouvertureFX(Dictionary<DateTime, HedgeState> output)
        {
            List<DataPoint> pricePoints = new List<DataPoint>();
            List<DataPoint> covePoints = new List<DataPoint>();
            List<DataPoint> deltasHG = new List<DataPoint>();
            List<DataPoint> deltasSP = new List<DataPoint>();
            List<DataPoint> deltasEur = new List<DataPoint>();
            List<DataPoint> deltasUSD = new List<DataPoint>();
            List<DataPoint> deltasHKD = new List<DataPoint>();
            foreach (var element in output)
            {
                pricePoints.Add(new DataPoint(element.Key.ToString("d"), element.Value.optionValue));
                covePoints.Add(new DataPoint(element.Key.ToString("d"), element.Value.portfolioValue));
                deltasHG.Add(new DataPoint(element.Key.ToString("d"), element.Value.composition["HANG SENG INDEX"]));
                deltasSP.Add(new DataPoint(element.Key.ToString("d"), element.Value.composition["S&P 500"]));
                deltasEur.Add(new DataPoint(element.Key.ToString("d"), element.Value.composition["ESTX 50"]));
                deltasUSD.Add(new DataPoint(element.Key.ToString("d"), element.Value.composition["USDEUR"]));
                deltasHKD.Add(new DataPoint(element.Key.ToString("d"), element.Value.composition["HKDEUR"]));
            }

            ViewBag.PrixKanji = JsonConvert.SerializeObject(pricePoints);
            ViewBag.CouvertureKanji = JsonConvert.SerializeObject(covePoints);
            ViewBag.DeltasHG = JsonConvert.SerializeObject(deltasHG);
            ViewBag.DeltasSP = JsonConvert.SerializeObject(deltasSP);
            ViewBag.DeltasEur = JsonConvert.SerializeObject(deltasEur);
            ViewBag.DeltasUSD = JsonConvert.SerializeObject(deltasUSD);
            ViewBag.DeltasHKD = JsonConvert.SerializeObject(deltasHKD);
        }


        public void Portfolio(Dictionary<String, double> assetValues)
        {

            List<DataPoint> percentages = new List<DataPoint>();
            foreach (var element in assetValues)
            {
                percentages.Add(new DataPoint(element.Key, element.Value));
            }

            ViewBag.PercentageCoverage = JsonConvert.SerializeObject(percentages);
        }



      


       

    }
}