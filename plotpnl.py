

import pandas as pd
import seaborn as sns
from scipy import stats
import pandas as pd
import plotly.graph_objects as go
import plotly.express as px

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.path as mpath
from scipy.ndimage.filters import gaussian_filter1d
import csv

def plot_pnl():


      df = pd.read_csv('semihistoric-hedging.csv', delimiter=';')
      fig = go.Figure()
      fig.add_trace(go.Scatter(x = df['Date'], y = df['Kanji Price'],
                    mode='lines',
                    name='option price'))
      fig.add_trace(go.Scatter(x = df['Date'], y = df['Hedging Portfolio'],
              mode='lines',
              name='portfolio'))

      fig.update_layout(title='PnL',
                   plot_bgcolor='rgb(250, 250,250)',
                   showlegend=True)

      fig.show()

def plot_density(M, freqValues):
    colors = ['g', 'blue', 'r', 'black']
    for freq, color in zip(freqValues, colors):
        filename = 'kanji_histogram_errors_M' + str(M) + '_freq_' + str(freq) +'.csv'
        filename_fx = 'fx_kanji_histogram_errors_M' + str(M) + '_freq_' + str(freq) +'.csv'
        with open(filename) as csv_file:
          with open(filename_fx) as csv_file_fx:

            csv_reader = csv.reader(csv_file, delimiter=';')
            csv_reader_fx = csv.reader(csv_file_fx, delimiter=';')
            l = []
            lp = []
            for x in csv_reader:
                l.append(float(x[0]))
            for x in csv_reader_fx:
                lp.append(float(x[0]))
            sns.kdeplot(l, bw=.2, label= "freq:" + freq, shade=True)
            #sns.kdeplot(lp, bw = .2, label = "bs_fx_" + freq, shade = True)
    plt.legend();
    plt.title("Distribution de l'erreur de couverture")
    plt.xlim(-10, 10)
    plt.show()




def main():
    M = 2016
    # freqValues = [1]
    # freqValues = [str(x) for x in freqValues]
    # plot_density(M, freqValues)
    plot_pnl()
main()
