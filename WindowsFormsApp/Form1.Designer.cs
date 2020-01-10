namespace WindowsFormsApp
{
    partial class Form1
    {
        /// <summary>
        /// Variable nécessaire au concepteur.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Nettoyage des ressources utilisées.
        /// </summary>
        /// <param name="disposing">true si les ressources managées doivent être supprimées ; sinon, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Code généré par le Concepteur Windows Form

        /// <summary>
        /// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
        /// le contenu de cette méthode avec l'éditeur de code.
        /// </summary>
        private void InitializeComponent()
        {
            this.prixLabel = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.textBox4 = new System.Windows.Forms.TextBox();
            this.textBox5 = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.vol1 = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.IC = new System.Windows.Forms.TextBox();
            this.vol2 = new System.Windows.Forms.TextBox();
            this.vol3 = new System.Windows.Forms.TextBox();
            this.delta1 = new System.Windows.Forms.TextBox();
            this.delta2 = new System.Windows.Forms.TextBox();
            this.delta3 = new System.Windows.Forms.TextBox();
            this.Correlation = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.icdelta1 = new System.Windows.Forms.TextBox();
            this.icdelta2 = new System.Windows.Forms.TextBox();
            this.icdelta3 = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // prixLabel
            // 
            this.prixLabel.Location = new System.Drawing.Point(104, 113);
            this.prixLabel.Name = "prixLabel";
            this.prixLabel.Size = new System.Drawing.Size(100, 20);
            this.prixLabel.TabIndex = 0;
            this.prixLabel.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(121, 97);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(56, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "priceLabel";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(646, 183);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(40, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "icDelta";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(199, 233);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 4;
            this.button1.Text = "goBtn";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.Form1_Click);
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(39, 39);
            this.textBox3.Name = "textBox3";
            this.textBox3.Size = new System.Drawing.Size(100, 20);
            this.textBox3.TabIndex = 5;
            // 
            // textBox4
            // 
            this.textBox4.Location = new System.Drawing.Point(174, 39);
            this.textBox4.Name = "textBox4";
            this.textBox4.Size = new System.Drawing.Size(100, 20);
            this.textBox4.TabIndex = 6;
            // 
            // textBox5
            // 
            this.textBox5.Location = new System.Drawing.Point(312, 39);
            this.textBox5.Name = "textBox5";
            this.textBox5.Size = new System.Drawing.Size(100, 20);
            this.textBox5.TabIndex = 7;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(57, 23);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(59, 13);
            this.label3.TabIndex = 8;
            this.label3.Text = "nbSamples";
            this.label3.Click += new System.EventHandler(this.label3_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(336, 23);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(44, 13);
            this.label4.TabIndex = 9;
            this.label4.Text = "Maturity";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(208, 23);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(34, 13);
            this.label5.TabIndex = 10;
            this.label5.Text = "Strike";
            // 
            // vol1
            // 
            this.vol1.Location = new System.Drawing.Point(440, 39);
            this.vol1.Name = "vol1";
            this.vol1.Size = new System.Drawing.Size(100, 20);
            this.vol1.TabIndex = 11;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(463, 23);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(53, 13);
            this.label6.TabIndex = 12;
            this.label6.Text = "Volatilities";
            this.label6.Click += new System.EventHandler(this.label6_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(546, 39);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(100, 20);
            this.textBox1.TabIndex = 13;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(575, 23);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(63, 13);
            this.label7.TabIndex = 14;
            this.label7.Text = "Interest rate";
            this.label7.Click += new System.EventHandler(this.label7_Click);
            // 
            // IC
            // 
            this.IC.Location = new System.Drawing.Point(260, 113);
            this.IC.Name = "IC";
            this.IC.Size = new System.Drawing.Size(100, 20);
            this.IC.TabIndex = 16;
            // 
            // vol2
            // 
            this.vol2.Location = new System.Drawing.Point(440, 76);
            this.vol2.Name = "vol2";
            this.vol2.Size = new System.Drawing.Size(100, 20);
            this.vol2.TabIndex = 17;
            // 
            // vol3
            // 
            this.vol3.Location = new System.Drawing.Point(440, 113);
            this.vol3.Name = "vol3";
            this.vol3.Size = new System.Drawing.Size(100, 20);
            this.vol3.TabIndex = 18;
            // 
            // delta1
            // 
            this.delta1.Location = new System.Drawing.Point(440, 215);
            this.delta1.Name = "delta1";
            this.delta1.Size = new System.Drawing.Size(100, 20);
            this.delta1.TabIndex = 19;
            // 
            // delta2
            // 
            this.delta2.Location = new System.Drawing.Point(440, 264);
            this.delta2.Name = "delta2";
            this.delta2.Size = new System.Drawing.Size(100, 20);
            this.delta2.TabIndex = 20;
            // 
            // delta3
            // 
            this.delta3.Location = new System.Drawing.Point(440, 320);
            this.delta3.Name = "delta3";
            this.delta3.Size = new System.Drawing.Size(100, 20);
            this.delta3.TabIndex = 21;
            // 
            // Correlation
            // 
            this.Correlation.Location = new System.Drawing.Point(674, 39);
            this.Correlation.Name = "Correlation";
            this.Correlation.Size = new System.Drawing.Size(100, 20);
            this.Correlation.TabIndex = 22;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(466, 182);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(37, 13);
            this.label8.TabIndex = 23;
            this.label8.Text = "Deltas";
            this.label8.Click += new System.EventHandler(this.label8_Click);
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(702, 23);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(57, 13);
            this.label9.TabIndex = 24;
            this.label9.Text = "Correlation";
            // 
            // icdelta1
            // 
            this.icdelta1.Location = new System.Drawing.Point(616, 215);
            this.icdelta1.Name = "icdelta1";
            this.icdelta1.Size = new System.Drawing.Size(100, 20);
            this.icdelta1.TabIndex = 25;
            // 
            // icdelta2
            // 
            this.icdelta2.Location = new System.Drawing.Point(616, 264);
            this.icdelta2.Name = "icdelta2";
            this.icdelta2.Size = new System.Drawing.Size(100, 20);
            this.icdelta2.TabIndex = 26;
            // 
            // icdelta3
            // 
            this.icdelta3.Location = new System.Drawing.Point(616, 320);
            this.icdelta3.Name = "icdelta3";
            this.icdelta3.Size = new System.Drawing.Size(100, 20);
            this.icdelta3.TabIndex = 27;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(283, 97);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(15, 13);
            this.label10.TabIndex = 28;
            this.label10.Text = "ic";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.icdelta3);
            this.Controls.Add(this.icdelta2);
            this.Controls.Add(this.icdelta1);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.Correlation);
            this.Controls.Add(this.delta3);
            this.Controls.Add(this.delta2);
            this.Controls.Add(this.delta1);
            this.Controls.Add(this.vol3);
            this.Controls.Add(this.vol2);
            this.Controls.Add(this.IC);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.vol1);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.textBox5);
            this.Controls.Add(this.textBox4);
            this.Controls.Add(this.textBox3);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.prixLabel);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Click += new System.EventHandler(this.Form1_Click);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox prixLabel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.TextBox textBox4;
        private System.Windows.Forms.TextBox textBox5;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox vol1;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox IC;
        private System.Windows.Forms.TextBox vol2;
        private System.Windows.Forms.TextBox vol3;
        private System.Windows.Forms.TextBox delta1;
        private System.Windows.Forms.TextBox delta2;
        private System.Windows.Forms.TextBox delta3;
        private System.Windows.Forms.TextBox Correlation;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox icdelta1;
        private System.Windows.Forms.TextBox icdelta2;
        private System.Windows.Forms.TextBox icdelta3;
        private System.Windows.Forms.Label label10;
    }
}

