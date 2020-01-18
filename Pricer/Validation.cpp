#include "VanillaCall.hpp"
#include "MonteCarlo.hpp"
#include <ctime>
#include <iostream>
#include "Hedge.hpp"
#include "pnl/pnl_matrix.h"
#include <fstream>

void validatePrice(PnlMat* simulated_path, VanillaCall* call, double r, double sigma_, double T, double strike, MonteCarlo* mc, 
	_PnlMat* past, BlackScholesModel* model, int nbTimeSteps, int H ) {
	std::ofstream myfile;
	myfile.open("../Validation/price_vanilla.txt");
	double spot_at_t = 0;
	double prix_mc = 0;
	double ic = 0;
	double prix = 0;
	double t = 0;
	int count= 0;
	myfile << " Validation price" << std::endl;
	for (double i = 0.0; i <= double(H); i++)
	{
		t = i / (double)H;
		model->getPast(past, simulated_path, t, nbTimeSteps, H, T);
		spot_at_t = MGET(simulated_path, int(i), 0);
		prix = call->price(t, spot_at_t, r, sigma_, T, strike);
		mc->price(past, t, prix_mc, ic);
		myfile << "prix en t = " << t << " Monte-Carlo : " << prix_mc;
		myfile << " formule fermee: " << prix;
		if (prix_mc - ic / 2 <= prix && prix <= prix_mc+ ic/ 2)
		{
			myfile << " in ";
			count++;
		}
		else
		{
			myfile << " not in ";
		}
		myfile << "   [" << prix_mc - ic / 2 << ", " << prix_mc + ic / 2 << "] " << std::endl;
	}
	myfile << std::endl << count << " / " << (int)(H+1) << "sont dans l'intervalle de confiance" << std::endl;
	myfile.close();
}

void validateDelta(PnlMat* simulated_path, VanillaCall* call, double r, double sigma_, double T, double strike, MonteCarlo* mc,
	_PnlMat* past, BlackScholesModel* model, int nbTimeSteps, int H) {
	PnlVect* delta = pnl_vect_create(1);
	PnlVect* ic_delta = pnl_vect_create(1);
	std::ofstream myfile;
	myfile.open("../Validation/delta_vanilla.txt");
	double spot_at_t = 0;
	double delta_ferme= 0;
	double delta_ = 0;
	double ic_delta_=0;
	double t = 0;
	int count = 0;
	myfile << " Validation delta " << std::endl;
	for (double i = 0.0; i <= (double)H; i++)
	{
		t = i / (double)H;
		model->getPast(past, simulated_path, t, nbTimeSteps, H, T);
		spot_at_t = MGET(simulated_path, (int)i, 0);
		delta_ferme = call->delta(t, spot_at_t, r, sigma_, T, strike);
		mc->delta(past, t, delta, ic_delta);
		delta_ = pnl_vect_get(delta, 0);
		ic_delta_ = pnl_vect_get(ic_delta, 0);
		myfile << "delta en t = " << t << " Monte-Carlo : " << delta_;
		myfile <<" formule fermee: " << delta_ferme;
		if (delta_ - ic_delta_ / 2 <= delta_ferme && delta_ferme <= delta_ + ic_delta_ / 2)
		{
			myfile << " in ";
			count++;
		}
		else
		{
			myfile << " not in ";
		}
		myfile  << "   [" << delta_ - ic_delta_ / 2 << ", " << delta_ + ic_delta_ / 2 << "] " << std::endl;
	}
	myfile << std::endl << count << " / " << int(H+1) << "sont dans l'intervalle de confiance" << std::endl;
}

void validateHedging(MonteCarlo* mc, PnlMat* simulated_path, int nbTimeSteps, int H) {
	Hedge hedge(mc);
	double error = 0;
	double ic0 = 0;
	PnlVect* option_values = pnl_vect_create(H + 1);
	PnlVect* portfolio_values = pnl_vect_create(H + 1);
	hedge.PnL(simulated_path, nbTimeSteps + 1, portfolio_values, option_values, error, ic0);
	PnlMat *comparaison = pnl_mat_create(H + 1, 2);

}

int main() {
	// initializing the random number generator 
	PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));

	// initializing the model and the call option
	double r = 0.02;
	double spot_ = 109;
	double sigma_ = 0.2;
	PnlVect *spot = pnl_vect_create_from_scalar(1, spot_);
	PnlVect *sigma = pnl_vect_create_from_scalar(1, sigma_);
	PnlVect *trend = pnl_vect_create_from_scalar(1, r);
	double T = 1;
	double strike = 100;
	BlackScholesModel* model = new BlackScholesModel(1, r, 0, sigma, spot, trend);
	int nbTimeSteps = 10;
	VanillaCall* call = new VanillaCall(T, nbTimeSteps, 1, strike);

	// initializing the montecarlo tool
	int n_samples = 50000;
	double epsilon = 0.005;
	double gamma = -1.0 / 4.0;
	double epsilon_n = epsilon*pow(n_samples, -gamma);
	MonteCarlo* mc = new MonteCarlo(model, call, rng, T / nbTimeSteps, n_samples, epsilon_n);
	double ic = 0;

	// simulating the data
	int H = 4*nbTimeSteps;
	PnlMat* simulated_path = pnl_mat_create(H, 1);
	model->simul_market(simulated_path, T, H, rng);
	// computing the price 
	PnlMat *past = pnl_mat_create(1, 1);
	std::cout << std::endl;
	PnlMat *path = pnl_mat_create_from_scalar(nbTimeSteps + 1, 1, spot_);
	validatePrice(simulated_path, call, r, sigma_, T, strike, mc,past, model, nbTimeSteps, H);
	std::cout << std::endl;
	validateDelta(simulated_path, call, r, sigma_, T, strike, mc,past, model, nbTimeSteps, H);
	return 0;
}
