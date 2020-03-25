#ifndef COMPUTATIONS
#define COMPUTATIONS

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
#include "FXBlackScholes.hpp"
#include "KanjiOptionFX.hpp"
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

void Computations::performance_price_t(double netAssetValue, double &ic, double &prix, int nb_samples, double T,
	double t, double past_[], double inital_values[], int nb_dates, double sigma_[], double correlation[], double r)
{
	PnlMat *past = pnl_mat_create_from_ptr(nb_dates, size, past_);
	MonteCarlo *mc = initialize_mc(nb_samples, T, past_, netAssetValue, inital_values, nb_dates, sigma_, correlation, r);
	mc->price(past, t, prix, ic);
}

void Computations::performance_price_t_fx(double netAssetValue, double& ic, double& prix, int nb_samples, double T,
	double t, double past_[], double initial_values[], int nb_dates, double sigma_[], double correlation[], double r, double r_US, double r_HK)
{
	double size_fx = 5;
	PnlMat* past = pnl_mat_create_from_ptr(nb_dates, size_fx, past_);
	MonteCarlo* mc = init_mc_fx(nb_samples, T, past_, netAssetValue, initial_values, nb_dates, sigma_, correlation, r, r_US, r_HK);
	mc->price(past, t, prix, ic);
}



void Computations::performance_delta_t(double netAssetValue, double ic_deltas[], double deltas[], int nb_samples, double T,
	double t, double past_[], double inital_values[], int nb_dates, double sigma_[], double correlation[], double r)
{
	PnlVect* ic_delta = pnl_vect_create_from_ptr(size, ic_deltas);
	PnlVect* delta = pnl_vect_create_from_ptr(size, deltas);
	PnlMat *past = pnl_mat_create_from_ptr(nb_dates, size, past_);
	MonteCarlo *mc = initialize_mc(nb_samples, T, past_, netAssetValue, inital_values, nb_dates, sigma_, correlation, r);
	mc->delta(past, t, delta, ic_delta);
	for (int i = 0; i < delta->size; i++) {
		deltas[i] = pnl_vect_get(delta, i);
		ic_deltas[i] = pnl_vect_get(ic_delta, i);
	}
}

void Computations::performance_delta_t_fx(double netAssetValue, double ic_deltas[], double deltas[], int nb_samples, double T,
	double t, double past_[], double initial_values[], int nb_dates, double sigma_[], double correlation[], double r, double r_US, double r_HK) {
	int size_fx = 5;
	PnlVect* ic_delta = pnl_vect_create_from_ptr(size_fx, ic_deltas);
	PnlVect* delta = pnl_vect_create_from_ptr(size_fx, deltas);
	PnlMat* past = pnl_mat_create_from_ptr(nb_dates, size_fx, past_);
	MonteCarlo* mc = init_mc_fx(nb_samples, T, past_, netAssetValue, initial_values, nb_dates, sigma_, correlation, r, r_US, r_HK);
	mc->delta(past, t, delta, ic_delta);
	for (int i = 0; i < delta->size; i++) {
		deltas[i] = pnl_vect_get(delta, i);
		ic_deltas[i] = pnl_vect_get(ic_delta, i);
	}
}


void Computations::simul_market(double path_[], double t, double maturity, int nb_dates, double s0_[], double trends_[], double sigmas_[], double correlation[], double r)
{
	PnlMat* path = pnl_mat_create(nb_dates + 1, size);
	PnlVect* s0 = pnl_vect_create_from_ptr(size, s0_);
	PnlVect* sigma = pnl_vect_create_from_ptr(size, sigmas_);
	PnlVect* trend_vec = pnl_vect_create_from_ptr(size, trends_);
	PnlMat* corr_mat = pnl_mat_create_from_ptr(size, size, correlation);
	BlackScholesModel *model = new BlackScholesModel(size, r, sigma, s0, trend_vec, corr_mat);

	PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));

	model->simul_market(path, maturity - t, nb_dates, rng);
	for (int index_row = 0; index_row < path->m; index_row++) {
		for (int index_col = 0; index_col < path->n; index_col++) {
			path_[index_row * path->n + index_col] = pnl_mat_get(path, index_row, index_col);
		}
	}
}

void Computations::simul_market_fx(double path_[], double t, double maturity, int nb_dates, double s0_[], double trends_[], double sigmas_[], double correlation[],
	double r, double r_US, double r_HK) {
	int size_fx = 5;
	PnlMat* path = pnl_mat_create(nb_dates + 1, size_fx);
	PnlVect* s0 = pnl_vect_create_from_ptr(size_fx, s0_);
	PnlVect* sigma = pnl_vect_create_from_ptr(size_fx, sigmas_);
	PnlVect* trend_vec = pnl_vect_create_from_ptr(size_fx, trends_);
	PnlMat* corr_mat = pnl_mat_create_from_ptr(size_fx, size_fx, correlation);
	PnlVect* vect_r = pnl_vect_create_from_double(size, r);
	pnl_vect_set(vect_r, 1, r_US);
	pnl_vect_set(vect_r, 2, r_HK);
	FXBlackScholes* model = new FXBlackScholes(size_fx, vect_r, sigma, s0, trend_vec, corr_mat);
	PnlRng* rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));


	model->simul_market(path, maturity - t, nb_dates, rng);
	for (int index_row = 0; index_row < path->m; index_row++) {
		for (int index_col = 0; index_col < path->n; index_col++) {
			path_[index_row * path->n + index_col] = pnl_mat_get(path, index_row, index_col);
		}
	}
}

MonteCarlo* Computations::initialize_mc(int nb_samples, double T, double past_[], double netAssetValue, double inital_values[], int nb_dates, double sigma_[], double correlation[], double r) {
	PnlMat* correlation_matrix = pnl_mat_create_from_ptr(size, size, correlation);
	PnlMat *past = pnl_mat_create_from_ptr(nb_dates, size, past_);
	PnlVect* S0 = pnl_vect_create(3);
	pnl_mat_get_row(S0, past, 0);
	PnlVect* sigma = pnl_vect_create_from_ptr(size, sigma_);
	PnlVect* trend_vec = pnl_vect_create_from_double(1, r);
	BlackScholesModel *model = new BlackScholesModel(size, r, sigma, S0, trend_vec, correlation_matrix);
	PnlVect *inital_values_ = pnl_vect_create_from_ptr(3, inital_values);
	KanjiOption *perf_option = new KanjiOption(T, n_time_steps, size, inital_values_, netAssetValue);
	PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));
	MonteCarlo *mc = new MonteCarlo(model, perf_option, rng, T / n_time_steps, nb_samples, 0.07);
	return mc;
}

MonteCarlo* Computations::init_mc_fx(int nb_samples, double T, double past_[], double netAssetValue, double initial_values[], int nb_dates, double sigma_[], double correlation[], double r, double r_US, double r_HK) {
	int size_fx = 5;
	PnlMat* correlation_matrix = pnl_mat_create_from_ptr(size_fx, size_fx, correlation);
	PnlMat* past = pnl_mat_create_from_ptr(nb_dates, size_fx, past_);
	PnlVect* vect_r = pnl_vect_create_from_double(size, r);
	pnl_vect_set(vect_r, 1, r_US);
	pnl_vect_set(vect_r, 2, r_HK);
	PnlVect* S0 = pnl_vect_create(size_fx);
	pnl_mat_get_row(S0, past, 0);
	PnlVect* sigma = pnl_vect_create_from_ptr(size_fx, sigma_);
	PnlVect* trend_vec = pnl_vect_create_from_double(size_fx, r);
	FXBlackScholes* model = new FXBlackScholes(size_fx, vect_r, sigma, S0, trend_vec, correlation_matrix);
	PnlVect* initial_values_ = pnl_vect_create_from_ptr(size_fx, initial_values);
	KanjiOptionFX* kanji_fx = new KanjiOptionFX(T, n_time_steps, size_fx, initial_values_, netAssetValue, r_US, r_HK);
	PnlRng* rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));
	MonteCarlo* mc = new MonteCarlo(model, kanji_fx, rng, T / n_time_steps, nb_samples, 0.07);

	return mc;
}
#endif