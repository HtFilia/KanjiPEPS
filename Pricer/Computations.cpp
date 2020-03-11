#include "Computations.hpp"
#include <iostream>
#include <time.h>
#include "pnl/pnl_random.h"
#include "MonteCarlo.hpp"
#include "BlackScholesModel.hpp"
#include "VanillaCall.hpp"
#include "pnl/pnl_vector.h"
#include "KanjiOption.hpp"
#include "KanjiOption.hpp"
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
	/*int size = 3;
	PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));

	PnlVect * deltass = pnl_vect_create(3);
	PnlVect * ic_deltass = pnl_vect_create(3);

	int M = 50000;
	int n_time_steps = 16;


	double spot_ = 2681;

	PnlVect *spot = pnl_vect_create_from_ptr(size, S0_);
	PnlVect* sigma = pnl_vect_create_from_ptr(size, sigma_);
	PnlVect *trend = pnl_vect_create_from_scalar(size, r);
	BlackScholesModel* model = new BlackScholesModel(size, r, correlation, sigma, spot, trend);
	KanjiOption *kanji = new KanjiOption(T, n_time_steps, size);
	MonteCarlo* mc = new MonteCarlo(model, kanji, rng, T / n_time_steps, M, 0.1);
	PnlMat *past = pnl_mat_create_from_double(1, 3, 0);
	pnl_mat_set_row(past, spot, 0);
	for (int i = 0; i < 3; i++) {
		deltas[i] = pnl_vect_get(deltass, i);
	}*/
}

void Computations::performance_price_hedge_t(double &ic, double &prix, double ic_deltas[], double deltas[], int nb_samples, double T,
	double t, double past_[], double nb_dates, double sigma_[], double correlation[], double r)
{
	/*int size = 3;
	PnlVect* ic_delta = pnl_vect_create_from_ptr(size, ic_deltas);
	PnlVect* delta = pnl_vect_create_from_ptr(size, deltas);
	PnlMat* correlation_matrix = pnl_mat_create_from_ptr(size, size, correlation);
	PnlMat *past = pnl_mat_create_from_ptr(nb_dates, size, past_);
	PnlVect* S0 = pnl_vect_create(3);
	pnl_mat_get_row(S0, past, 0);
	PnlVect* sigma = pnl_vect_create_from_ptr(size, sigma_);
	PnlVect* trend_vec = pnl_vect_create_from_double(1, r);
	BlackScholesModel *model = new BlackScholesModel(size, r, sigma, S0, trend_vec, correlation_matrix);
	int timesteps = 16; //voir boucle dans perf payoff


	KanjiOption *perf_option = new KanjiOption(T, timesteps, size);
	PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));

	MonteCarlo *mc = new MonteCarlo(model, perf_option, rng, T / timesteps, nb_samples, 0.07);
	//pnl_mat_print(past);
	mc->price(past, t, prix, ic);
	mc->delta(past, t, delta, ic_delta);
	for (int i = 0; i < delta->size; i++) {
		deltas[i] = pnl_vect_get(delta, i);
		ic_deltas[i] = pnl_vect_get(ic_delta, i);
	}*/
}
void Computations::simul_market(double path_[], double t, double maturity, int nbHedging_dates, double s0_[], double trends_[], double sigmas_[], double correlation[], double r)
{
	int size_path = (t * nbHedging_dates) / maturity;
	int size = 3;
	PnlMat* path = pnl_mat_create(size_path, size);
	PnlVect* s0 = pnl_vect_create_from_ptr(size, s0_);
	PnlVect* sigma = pnl_vect_create_from_ptr(size, sigmas_);
	PnlVect* trend_vec = pnl_vect_create_from_ptr(size, trends_);
	PnlMat* corr_mat = pnl_mat_create_from_ptr(size, size, correlation);
	BlackScholesModel *model = new BlackScholesModel(size, r, sigma, s0, trend_vec, corr_mat);

	PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));

	model->simul_market(path, maturity, nbHedging_dates, rng);
	for (int index_row = 0; index_row < path->m; index_row++) {
		for (int index_col = 0; index_col < path->n; index_col++) {
			path_[index_row * path->n + index_col] = pnl_mat_get(path, index_row, index_col);
		}

	}
}
