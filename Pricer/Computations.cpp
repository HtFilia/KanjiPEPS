#include "Computations.hpp"
#include <iostream>
#include "pnl/pnl_random.h"
#include "MonteCarlo.hpp"
#include "BlackScholesModel.hpp"
#include "VanillaCall.hpp"
#include "PerformanceOption.hpp"
#include <ctime>
#include "pnl/pnl_vector.h"
using namespace std;

void Computations::callMC(double &ic, double &prix, int nb_samples, double T,
	double S0, double K, double sigma, double r)
{
	PnlVect* sigma_vec = pnl_vect_create_from_scalar(1, sigma);
	PnlVect* spot_vec = pnl_vect_create_from_scalar(1, S0);
	PnlVect* trend_vec = pnl_vect_create_from_double(1, r);
	BlackScholesModel *model = new BlackScholesModel(1, r, 0, sigma_vec, spot_vec, trend_vec);


	Option *vanilla_call = new VanillaCall(T, 1, 1, K);
	PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, 0);

	MonteCarlo *mc = new MonteCarlo(model, vanilla_call, rng, T, nb_samples, 0.05);

	mc->price(prix, ic);
}
void Computations::calleuro(double &ic, double &prix, int nb_samples, double T,
	double S0, double K, double sigma, double r)
{
	double drift = (r - sigma * sigma / 2.) * T;
	double sqrt_T = sqrt(T);
	double sum = 0;
	double var = 0;
	PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, 0);
	double payoff;
	for (int i = 0; i < nb_samples; i++)
	{
		payoff = S0 * exp(drift + sigma * sqrt_T * pnl_rng_normal(rng));
		payoff = MAX(payoff - K, 0.);
		sum += payoff;
		var += payoff * payoff;
	}

	prix = exp(-r * T) * sum / nb_samples;
	var = exp(-2.*r*T) * var / nb_samples - prix * prix;
	ic = 1.96 * sqrt(var / nb_samples);
	pnl_rng_free(&rng);
}

void Computations::performance_price_hedge(double &ic, double &prix, double ic_deltas[], double deltas[], int nb_samples, double T,
	double S0_[], double K, double sigma_[], double correlation, double r) {
	int size = 3;

	PnlVect* ic_delta = pnl_vect_create_from_ptr(size, ic_deltas);
	PnlVect* delta = pnl_vect_create_from_ptr(size, deltas);
	PnlVect* S0 = pnl_vect_create_from_ptr(size, S0_);
	PnlVect* sigma = pnl_vect_create_from_ptr(size, sigma_);
	PnlVect* trend_vec = pnl_vect_create_from_double(1, r);
	BlackScholesModel *model = new BlackScholesModel(size, r, correlation, sigma, S0, trend_vec);
	PnlVect* payOffCoeffs = pnl_vect_create_from_scalar(3,1/3);
	int timesteps = 100; //à revoir
	Option *perf_option = new PerformanceOption(T, timesteps, size, payOffCoeffs);
	PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));

	MonteCarlo *mc = new MonteCarlo(model, perf_option, rng, T/nb_samples, nb_samples, 0.05);
	PnlMat *past = pnl_mat_create_from_double(1, 3, 0);
	pnl_mat_set_row(past, S0, 0);
	mc->price_and_delta(past, 0, prix, ic, delta, ic_delta);
}