using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using Wrapper;
using WebComponent.Models;

namespace WebComponent.Controllers
{
    public class HomeController : Controller
    {
        [HttpGet]
        public ActionResult Index()
        {
            WrapperClass wrapper = new WrapperClass();
            wrapper.getPriceCallEuro(3000, 2, 100, 100, 0.2, 0.07);
            ViewBag.Price = 0;
            return View();
        }

        [HttpPost]
        public ActionResult Index(PriceFormModel priceFormModel)
        {
            WrapperClass wrapper = new WrapperClass();
            wrapper.getPriceCallEuro(priceFormModel.SampleNb, priceFormModel.Maturity, priceFormModel.InitialPrice,
                priceFormModel.Strike, priceFormModel.Volatility, priceFormModel.RiskFreeRate);
            ViewBag.Price = wrapper.getPrice();
            return View();
        }
    }
}