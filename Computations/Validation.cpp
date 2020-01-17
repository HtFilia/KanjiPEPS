#include "VanillaCall.hpp"
#include "MonteCarlo.hpp"
#include <ctime>
#include <iostream>
#include "Hedge.hpp"
#include "pnl/pnl_matrix.h"

void validatePrice(PnlMat* simulated_path, VanillaCall* call, double r, double sigma_, double T, double strike, MonteCarlo* mc, 
	_PnlMat* past,double prix_mc, double ic, BlackScholesModel* model, int nbTimeSteps, int H ) {
	PnlVect* delta = pnl_vect_create(1);
	PnlVect* ic_delta = pnl_vect_create(1);
	double spot_at_t = 0;
	double prix = 0;
	double delta_ = 0;
	double t = 0;
	int count= 0;
	std::cout << " Validation price" << std::endl;
	for (double i = 0.0; i <= 12; i++)
	{
		t = i / 12.0;
		model->getPast(past, simulated_path, t, nbTimeSteps, H, T);
		spot_at_t = MGET(simulated_path, int(i), 0);
		prix = call->price(t, spot_at_t, r, sigma_, T, strike);
		mc->price_and_delta(past, t, prix_mc, ic, delta, ic_delta);
		std::cout << "prix en t = " << t << " formule fermee: " << prix;
		std::cout << " Monte-Carlo : " << prix_mc;
		std::cout << "   [" << prix_mc - ic / 2 << ", " << prix_mc + ic / 2 << "] " << std::endl;
		if (prix_mc - ic / 2 <= prix && prix <= prix_mc+ ic/ 2)
		{
			count++;
		}
	}
	std::cout << count << " / " << 13 << "sont dans l'intervalle de confiance" << std::endl;


}

void validateDelta(PnlMat* simulated_path, VanillaCall* call, double r, double sigma_, double T, double strike, MonteCarlo* mc,
	_PnlMat* past, double prix_mc, double ic, BlackScholesModel* model, int nbTimeSteps, int H) {
	PnlVect* delta = pnl_vect_create(1);
	PnlVect* ic_delta = pnl_vect_create(1);
	double spot_at_t = 0;
	double delta_ferme= 0;
	double delta_ = 0;
	double ic_delta_=0;
	double t = 0;
	int count = 0;
	std::cout << " Validation delta " << std::endl;
	for (double i = 0.0; i <= 12; i++)
	{
		t = i / 12.0;
		model->getPast(past, simulated_path, t, nbTimeSteps, H, T);
		spot_at_t = MGET(simulated_path, int(i), 0);
		delta_ferme = call->delta(t, spot_at_t, r, sigma_, T, strike);
		//mc->price_and_delta(past, t, prix_mc, ic, delta, ic_delta);
		mc->delta(past, t, delta, ic_delta);
		delta_ = pnl_vect_get(delta, 0);
		ic_delta_ = pnl_vect_get(ic_delta, 0);
		std::cout << "delta en t = " << t << " formule fermee: " << delta_ferme;
		std::cout << " Monte-Carlo : " << delta_;
		std::cout << "   [" << delta_ - ic_delta_ / 2 << ", " << delta_ + ic_delta_ / 2 << "] " << std::endl;
		if (delta_ - ic_delta_ / 2 <= delta_ferme && delta_ferme <= delta_ + ic_delta_ / 2)
		{
			count++;
		}
	}
	std::cout << count << " / " << 13 << "sont dans l'intervalle de confiance" << std::endl;
}


void main() {
	// initializing the random number generator 
	PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));

	// initializing the model and the call option
	double r = 0.02;
	double spot_ = 100;
	double sigma_ = 0.2;
	PnlVect *spot = pnl_vect_create_from_scalar(1, spot_);
	PnlVect *sigma = pnl_vect_create_from_scalar(1, sigma_);
	PnlVect *trend = pnl_vect_create_from_scalar(1, r);
	double T = 1;
	double strike = 110;
	BlackScholesModel* model = new BlackScholesModel(1, r, 0, sigma, spot, trend);
	int nbTimeSteps = 3;
	VanillaCall* call = new VanillaCall(T, nbTimeSteps, 1, strike);

	// initializing the montecarlo tool
	int n_samples = 50000;
	double epsilon = 0.005;
	double gamma = -1.0 / 4.0;
	double epsilon_n = epsilon*pow(n_samples, -gamma);
	MonteCarlo* mc = new MonteCarlo(model, call, rng, T / nbTimeSteps, n_samples, epsilon_n);
	double prix_mc = 0;
	double ic = 0;

	// simulating the data
	int H = 12;
	PnlMat* simulated_path = pnl_mat_create(H, 1);
	model->simul_market(simulated_path, T, H, rng);
	std::cout << "\n simulated path \n";
	pnl_mat_print(simulated_path);

	// computing the price 
	PnlMat *past = pnl_mat_create(1, 1);
	std::cout << std::endl;
	PnlMat *path = pnl_mat_create_from_scalar(nbTimeSteps + 1, 1, spot_);
	validatePrice(simulated_path, call, r, sigma_, T, strike, mc,
		past, prix_mc, ic, model, nbTimeSteps, H);
	std::cout << std::endl;
	validateDelta(simulated_path, call, r, sigma_, T, strike, mc,
		past, prix_mc, ic, model, nbTimeSteps, H);
}
