using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp
{
    static class Program
    {
        /// <summary>
        /// Point d'entrée principal de l'application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            string newPath = string.Concat(Environment.GetEnvironmentVariable("PATH"), ";", Properties.Settings.Default.AdditionalPaths);
            Environment.SetEnvironmentVariable("PATH", newPath, EnvironmentVariableTarget.Process);
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}
