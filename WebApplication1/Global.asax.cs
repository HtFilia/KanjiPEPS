using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.Web.Optimization;
using System.Web.Routing;

namespace WebApplication1
{
    public class MvcApplication : System.Web.HttpApplication
    {
        protected void Application_Start()
        {
            
                String _path = String.Concat(System.Environment.GetEnvironmentVariable("PATH"), ";", System.AppDomain.CurrentDomain.RelativeSearchPath);
                System.Environment.SetEnvironmentVariable("PATH", _path, EnvironmentVariableTarget.Process);
            
            AreaRegistration.RegisterAllAreas();
            FilterConfig.RegisterGlobalFilters(GlobalFilters.Filters);
            RouteConfig.RegisterRoutes(RouteTable.Routes);
            BundleConfig.RegisterBundles(BundleTable.Bundles);
        }
    }
}
