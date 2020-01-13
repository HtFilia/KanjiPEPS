#pragma once
#define DLLEXP   __declspec( dllexport )
namespace Computations {
	DLLEXP void calleuro(double& ic, double& prix, int nb_samples, double T,
		double S0, double K, double sigma, double r);
	DLLEXP void callMC(double& ic, double& prix, int nb_samples, double T,
		double S0, double K, double sigma, double r);
	DLLEXP void performance_price_hedge(double& ic, double& prix, double ic_deltas[], double deltas[], int nb_samples, double T,
		double S0_[], double K, double sigma_[], double correlation, double r);
}