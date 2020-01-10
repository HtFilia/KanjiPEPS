using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Wrapper;


namespace WindowsFormsApp
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Click(object sender, EventArgs e)
        {
            WrapperClass wc = new WrapperClass();
            int nbSamples = Int32.Parse(textBox3.Text);
            double strike = Double.Parse(textBox4.Text);
            double maturity = Double.Parse(textBox5.Text);
            double sigma1 = Double.Parse(vol1.Text);
            double sigma2 = Double.Parse(vol2.Text);
            double sigma3 = Double.Parse(vol3.Text);
            double r = Double.Parse(textBox1.Text);
            double corr = Double.Parse(Correlation.Text);
            double[] S0 = new double[] { strike, strike, strike} ;
            double[] sigmas = new double[] { sigma1, sigma2, sigma3};

            wc.getPriceDeltaPerf(nbSamples, maturity, S0, strike, sigmas, corr, r);
            prixLabel.Text = wc.getPrice().ToString();
            IC.Text = wc.getIC().ToString();
            

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void label7_Click(object sender, EventArgs e)
        {

        }

        private void label6_Click(object sender, EventArgs e)
        {

        }

        private void label8_Click(object sender, EventArgs e)
        {

        }
    }
}
