
#pragma once
#include "MonteCarlo.hpp"
#define DLLEXP   __declspec( dllexport )
namespace Computations {
	DLLEXP void calleuro(double &ic, double &prix, int nb_samples, double T,
		double S0, double K, double sigma, double r);
	DLLEXP void callMC(double &ic, double &prix, int nb_samples, double T,
		double S0, double K, double sigma, double r);
	//DLLEXP void performance_price_hedge(double &ic, double &prix, double ic_deltas[], double deltas[], int nb_samples, double T,double S0_[], double sigma_[], double correlation, double r);
	DLLEXP void performance_price_t(double netAssetValue, double & ic, double & prix, int nb_samples, double T, double t, double past_[], double inital_values[], int nb_dates, double sigma_[], double correlation[], double r);
	DLLEXP void performance_delta_t(double netAssetValue, double ic_deltas[], double deltas[], int nb_samples, double T, double t, double past_[], double inital_values[], int nb_dates, double sigma_[], double correlation[], double r);
	//DLLEXP void performance_price_hedge_t(double &ic, double &prix, double ic_deltas[], double deltas[], int nb_samples, double T,double t, double path_[], double nb_dates, double sigma_[], double correlation[], double r);
	// simulate the 3 underlyings from 0 to the date t, here we assume that t is a constatation date.
	DLLEXP void simul_market(double path_[], double t, double maturity, int nbHedging_dates, double s0_[], double trends_[], double sigmas_[], double correlation[], double r);

	MonteCarlo * initialize_mc(int nb_samples, double T, double past_[], double netAssetValue, double inital_values[], int nb_dates, double sigma_[], double correlation[], double r);

	int n_time_steps = 16;
	int size = 3;

}
