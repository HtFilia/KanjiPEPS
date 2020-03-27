using System.Web;
using System.Web.Optimization;

namespace WebComponent
{
    public class BundleConfig
    {
        // Pour plus d'informations sur le regroupement, visitez https://go.microsoft.com/fwlink/?LinkId=301862
        public static void RegisterBundles(BundleCollection bundles)
        {
            bundles.Add(new ScriptBundle("~/bundles/jquery").Include(
                        "~/Content/js/jquery/jquery-3.4.1.min.js"));

            bundles.Add(new ScriptBundle("~/bundles/jqueryval").Include(
                        "~/Content/js/jquery/jquery.validate.min.js",
                        "~/Content/js/jquery/additional-methods.min.js"));

            // Utilisez la version de développement de Modernizr pour le développement et l'apprentissage. Puis, une fois
            // prêt pour la production, utilisez l'outil de génération à l'adresse https://modernizr.com pour sélectionner uniquement les tests dont vous avez besoin.
            bundles.Add(new ScriptBundle("~/bundles/modernizr").Include(
                        "~/Content/js/modernizr/modernizr-2.8.3.js"));

            bundles.Add(new ScriptBundle("~/bundles/bootstrap").Include(
                      "~/Content/js/bootstrap/bootstrap.min.js"));

            bundles.Add(new ScriptBundle("~/bundles/argon").Include(
                      "~/Content/vendor/js-cookie/js.cookie.js",
                      "~/Content/vendor/chart.js/Chart.min.js",
                      "~/Content/vendor/chart.js/Chart.extension.js",
                      "~/Content/js/argon/argon.js"));

            bundles.Add(new ScriptBundle("~/bundles/font-awesome").Include(
                      "~/Content/js/font-awesome/all.min.js"));

            bundles.Add(new StyleBundle("~/bundles/css").Include(
                      "~/Content/css/bootstrap/bootstrap.min.css",
                      "~/Content/css/site.css",
                      "~/Content/css/argon/argon.css"));
        }
    }
}
