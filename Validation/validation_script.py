# -*- coding: utf-8 -*-
"""
Éditeur de Spyder

Ceci est un script temporaire.
"""

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.path as mpath
from scipy.ndimage.filters import gaussian_filter1d
import csv

def plot_histogram():
    HValues = ['365']
    colors = ['g', 'blue', 'r', 'black']
    for H, color in zip(HValues, colors):
        filename = 'histogram_errors_M365_H'+H+'.csv'
        with open(filename) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=';')
            l = []
            for x in csv_reader:
                l.append(float(x[0]))
            n, bins, patches = plt.hist(l, 10, density=True,label = H,\
                                        facecolor=color, alpha=0.75)
    bin_centers = 0.5*(bins[1:]+bins[:-1])
    plt.plot(bin_centers,n) 
    plt.xlabel('Erreur de couverture')
    plt.ylabel('Probabilité')
    plt.legend()
    plt.title("Histogramme de l'erreur de couverture")
    plt.grid(True)
    plt.gcf().savefig("call_error_histogram")
    plt.clf()
    print(sum(l)/len(l))

def plot_error_H():
    with open("variation_frequence_call_M64.txt") as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=';')
        x = []
        y = []
        for val in csv_reader:
            x.append(float(val[0]))
            y.append(float(val[1]))
    
    ysmoothed = gaussian_filter1d(y, sigma=2)
    
    star = mpath.Path.unit_regular_star(6)
    circle = mpath.Path.unit_circle()
    # concatenate the circle with an internal cutout of the star
    verts = np.concatenate([circle.vertices, star.vertices[::-1, ...]])
    codes = np.concatenate([circle.codes, star.codes])
    cut_star = mpath.Path(verts, codes)
    
    
    plt.plot(x, ysmoothed, '--r', marker=cut_star, markersize=8 )
    plt.ylabel('Erreur de couverture')
    plt.xlabel('Nombre de dates de rebalancement')
    plt.title("Erreur de couverture en fonction de la fréquence de rebalancement")
    plt.grid(True)
    plt.savefig("hedging_error_f(H)")

plot_histogram()
#plot_error_H()