#include "Computations.hpp"
#include <iostream>
#include <time.h>
#include "pnl/pnl_random.h"
#include "MonteCarlo.hpp"
#include "BlackScholesModel.hpp"
#include "VanillaCall.hpp"
#include "PerformanceOption.hpp"
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
	pnl_rng_sseed(rng, time(NULL));

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
	pnl_rng_sseed(rng, time(NULL));
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
void  Computations::performance_price_hedge(double &ic, double &prix, double ic_deltas[], double deltas[], int nb_samples, double T,
	double S0_[], double sigma_[], double correlation, double r) {
	int size = 3;

	PnlVect* ic_delta = pnl_vect_create_from_ptr(size, ic_deltas);
	PnlVect* delta = pnl_vect_create_from_ptr(size, deltas);
	PnlVect* S0 = pnl_vect_create_from_ptr(size, S0_);
	PnlVect* sigma = pnl_vect_create_from_ptr(size, sigma_);
	PnlVect* trend_vec = pnl_vect_create_from_double(1, r);
	BlackScholesModel *model = new BlackScholesModel(size, r, correlation, sigma, S0, trend_vec);
	PnlVect* payOffCoeffs = pnl_vect_create_from_scalar(3, 1.0 / 3.0);
	int timesteps = 16; //voir boucle dans perf payoff 
	Option *perf_option = new PerformanceOption(T, timesteps, size, payOffCoeffs);
	PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));

	MonteCarlo *mc = new MonteCarlo(model, perf_option, rng, T / nb_samples, nb_samples, 0.1);
	PnlMat *past = pnl_mat_create_from_double(1, 3, S0_[0]);
	pnl_mat_set_row(past, S0, 0);
	mc->price_and_delta(past, 0, prix, ic, delta, ic_delta);
	pnl_vect_print(delta);
	for (int i = 0; i < delta->size; i++) {
		deltas[i] = pnl_vect_get(delta, i);
		//std::cout << deltas[i] << std::endl;
		//std::cout << pnl_vect_get(delta, i) << std::endl;

	}
}

void Computations::performance_price_hedge_t(double &ic, double &prix, double ic_deltas[], double deltas[], int nb_samples, double T,
	double t, double path_[], double nb_dates, double sigma_[], double correlation, double r)
{
	int size = 3;
	PnlVect* ic_delta = pnl_vect_create_from_ptr(size, ic_deltas);
	PnlVect* delta = pnl_vect_create_from_ptr(size, deltas);
	PnlMat *path = pnl_mat_create_from_ptr(nb_dates, size, path_);
	PnlMat *past = pnl_mat_create(nb_dates, size);
	PnlVect* S0 = pnl_vect_create(3);
	pnl_mat_get_row(S0, path, 0);
	PnlVect* sigma = pnl_vect_create_from_ptr(size, sigma_);
	PnlVect* trend_vec = pnl_vect_create_from_double(1, r);
	BlackScholesModel *model = new BlackScholesModel(size, r, correlation, sigma, S0, trend_vec);
	PnlVect* payOffCoeffs = pnl_vect_create_from_scalar(3, 1.0 / 3.0);
	int timesteps = 16; //voir boucle dans perf payoff
	model->getPast(past, path, t, timesteps, T);

	Option *perf_option = new PerformanceOption(T, timesteps, size, payOffCoeffs);
	PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));

	MonteCarlo *mc = new MonteCarlo(model, perf_option, rng, T / nb_samples, nb_samples, 0.07);
	mc->price_and_delta(past, t, prix, ic, delta, ic_delta);
	for (int i = 0; i < delta->size; i++) {
		deltas[i] = pnl_vect_get(delta, i);

	}
}
void Computations::simul_market(double path_[], double t, double maturity, int nbHedging_dates, double s0_[], double trends_[], double sigmas_[], double correlation, double r)
{
	int size_path = (t * nbHedging_dates) / maturity;
	int size = 3;
	PnlMat* path = pnl_mat_create(size_path, size);
	PnlVect* s0 = pnl_vect_create_from_ptr(size, s0_);
	PnlVect* sigma = pnl_vect_create_from_ptr(size, sigmas_);
	PnlVect* trend_vec = pnl_vect_create_from_ptr(size, trends_);
	BlackScholesModel *model = new BlackScholesModel(size, r, correlation, sigma, s0, trend_vec);

	PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));

	model->simul_market(path, maturity, nbHedging_dates, rng);
	for (int index_row = 0; index_row < path->m; index_row++) {
		for (int index_col = 0; index_col < path->n; index_col++) {
			path_[index_row * path->n + index_col] = pnl_mat_get(path, index_row, index_col);
		}

	}
}