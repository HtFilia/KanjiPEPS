#include "VanillaCall.hpp"
#include <iostream>


double VanillaCall::payoff(double S_T) {


	return S_T > strike_ ? S_T - strike_: 0 ;
}

double VanillaCall::payoff(const PnlMat *path, double past_payoff, int past_len) {

	
	return 1;
}

double VanillaCall::payoff(const PnlMat *path) {
	double S_T = pnl_mat_get(path, path->m - 1, 0);
	return S_T > strike_ ? S_T - strike_ : 0;

}


double VanillaCall::price(double t, double spot, double r, double sigma, double maturity, double strike) {
	if (t == maturity) {
		return MAX(spot - strike, 0);
	}
	double d1 = (log(spot / strike) + (r + 0.5*sigma*sigma)*(maturity - t)) / (sigma * sqrt(maturity - t));
	double d2 = d1 - sigma * sqrt(maturity - t);
	double prix = spot *0.5* (1+pnl_sf_erf(d1/sqrt(2)) ) - strike * exp(-r * (maturity - t))*0.5* (1 + pnl_sf_erf(d2 / sqrt(2)));
	return prix;
}

double VanillaCall::delta(double t, double spot, double r, double sigma, double maturity, double strike) {
	double d1 = (log(spot / strike) + r + 0.5*sigma*sigma*(maturity - t)) / (sigma * sqrt(maturity - t));
	return 0.5* (1 + pnl_sf_erf(d1 / sqrt(2)));
}
