#include "Quanto.hpp"
using namespace std;

double Quanto::payoff(double S_T, double Z_eur, double r_usd) {

	return S_T > strike_ ? (S_T - strike_) * Z_eur * exp(-r_usd * T_): 0;

}
double Quanto::payoff(const PnlMat* path) {
	double S_T = pnl_mat_get(path, path->m - 1, 0);
	double Z_eur = pnl_mat_get(path, path->m - 1, 1);
	double r_usd = pnl_vect_get(vect_r_, 1);

	return payoff(S_T, Z_eur, r_usd);
}

double Quanto::price(double t, double spot_S, double spot_Z, PnlVect* r, PnlVect* sigmas, double corr) {
	if (t == T_) {
		return payoff(spot_S, spot_Z, pnl_vect_get(r, 1));
	}
	double sigma_s = pnl_vect_get(sigmas, 0);
	double sigma_x = pnl_vect_get(sigmas, 1);
	double r_eur = pnl_vect_get(r, 0);
	double r_usd = pnl_vect_get(r, 1);
	
	double spot_X = spot_Z * exp(-r_usd * t);
	double d1 = (log(spot_S / strike_) + (r_eur + sigma_s * sigma_x * corr + 0.5*sigma_s*sigma_s)*(T_ - t))/(pnl_vect_get(sigmas, 0) * sqrt(T_ - t));
	double d2 = d1 - pnl_vect_get(sigmas, 0) * sqrt(T_ - t);
	double prix = spot_S * spot_X * exp((-r_usd + r_eur + sigma_s*sigma_x*corr) * (T_-t)) * 0.5 * (1 + pnl_sf_erf(d1 / sqrt(2))) - strike_ * spot_Z * exp(-r_usd * T_) * 0.5 * (1 + pnl_sf_erf(d2 / sqrt(2)));
	return prix;
}

double Quanto::delta(double t, double spot_S, double spot_Z, PnlVect* r, PnlVect* sigmas, double corr) {
	double sigma_s = pnl_vect_get(sigmas, 0);
	double sigma_x = pnl_vect_get(sigmas, 1);
	double r_eur = pnl_vect_get(r, 0);
	double r_usd = pnl_vect_get(r, 1);

	double spot_X = spot_Z * exp(-r_usd * t);
	double d1 = (log(spot_S / strike_) + (r_eur + sigma_s * sigma_x * corr + 0.5 * sigma_s * sigma_s) * (T_ - t)) / (pnl_vect_get(sigmas, 0) * sqrt(T_ - t));
	return spot_X * exp((-r_usd + r_eur + sigma_s * sigma_x * corr) * (T_ - t)) * 0.5 * (1 + pnl_sf_erf(d1 / sqrt(2)));
}




