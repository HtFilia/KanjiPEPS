#include "pnl/pnl_matrix.h"
#include "MonteCarlo.hpp"
#include "VanillaCall.hpp"
#include <ctime>

void PnL(VanillaCall* call, MonteCarlo* mc_, PnlMat*  path, int n_time_steps, int H, PnlVect *portfolio_values, PnlVect* option_prices, double& error, double sigma, double r) {
	int size = mc_->opt_->size_;
	double r = mc_->mod_->r_;
	double T = mc_->opt_->T_;
	double prix = 0;
	pnl_vect_resize(portfolio_values, H + 1);
	pnl_vect_resize(option_prices, H + 1);
	PnlMat *past = pnl_mat_create(1, 1);;
	double delta_prev = 0;
	int path_index = 0;
	int hedging_index;
	double t = 0;
	int M = path->m - 1;
	int step = int(M / H);
	mc_->mod_->getPast(past, path, 0, n_time_steps, T); // filling the past matrix with the discrete instants preceding t and t spots
	double spot = pnl_mat_get(past, past->m - 1, 0);
	prix = call->price(t, spot, r, sigma, call->T_, call->strike_);
	delta_prev = call->delta(t, spot, r, sigma, call->T_, call->strike_);
	pnl_vect_set(option_prices, 0, prix);
	pnl_vect_set(portfolio_values, 0, prix);
	for (int i = 1; i < H; i++)
	{
		path_index = i * step;
		t = i * T / H;
		delta_prev = rebalance(call,mc_, path, path_index, i, t, n_time_steps, H, T, option_prices, portfolio_values, delta_prev, sigma);
	}

	pnl_vect_set(option_prices, H, mc_->opt_->payoff(path));
	double prev_spots = 0;
	prev_spots= pnl_mat_get(path, M - M / H, 0);
	double riskless_part = pnl_vect_get(portfolio_values, H - 1) - delta_prev*prev_spots * exp(r * T / H);
	// (V_{i-1} - delta_{i-1} * S_{i-1}) * e^(t_i - t_{i-1})
	spot = pnl_mat_get(path,M,0);
	double risky_part = delta_prev * spot;
	double portfolio_price = risky_part + riskless_part;
	pnl_vect_set(portfolio_values, H, portfolio_price);
	error = pnl_vect_get(portfolio_values, H) - pnl_vect_get(option_prices, H);
	pnl_mat_free(&past);
}

double rebalance(VanillaCall* call, MonteCarlo* mc_, PnlMat* path, int path_index, int hedging_index, double t, int n_time_steps, int H, double T, PnlVect* option_prices, PnlVect* portfolio_values, double delta_prev, double sigma)
{
	//initializing necessary variables
	int size = mc_->opt_->size_;
	PnlMat* past = pnl_mat_create(1, 1);
	double r = mc_->mod_->r_;
	double riskless_part = 0;
	double risky_part = 0;
	double portfolio_price = 0;
	double prix = 0;
	double ic_prix = 0;
	int M = path->m - 1;

	double spots_at_t = pnl_mat_get(path, path_index, 0);
	int prev_spot_global_index = path_index - M / H;
	double prev_spots = pnl_mat_get(path, prev_spot_global_index, 0);
	prix = call->price(t, spots_at_t, r, sigma, call->T_, call->strike_);
	double delta = call->delta(t, spots_at_t, r, sigma, call->T_, call->strike_);

	pnl_vect_set(option_prices, hedging_index, prix); //setting the option price

	// (V_{i-1} - delta_{i-1} * S_{i-1}) * e^(t_i - t_{i-1})
	riskless_part = pnl_vect_get(portfolio_values, hedging_index - 1) - delta_prev*prev_spots* exp(r * T / H);
	risky_part = delta_prev*spots_at_t;
	portfolio_price = risky_part + riskless_part;
	pnl_vect_set(portfolio_values, hedging_index, portfolio_price);
	// returning actual delta to update the delta_prev value
	return delta;
}

int main() {
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
	int n_time_steps = 20;
	VanillaCall* call = new VanillaCall(T, n_time_steps, 1, strike);

	// initializing the montecarlo tool
	int n_samples = 50000;
	double epsilon = 0.000001;
	double gamma = -1.0 / 4.0;
	double epsilon_n = epsilon * pow(n_samples, -gamma);
	MonteCarlo* mc = new MonteCarlo(model, call, rng, T / n_time_steps, n_samples, epsilon_n);
	double ic = 0;

	// simulating the data
	int M = 100;
	PnlMat* simulated_path = pnl_mat_create(M, 1);
	model->simul_market(simulated_path, T, M, rng);
	//	pnl_mat_print(simulated_path);
		// computing the price 
	PnlMat *past = pnl_mat_create(1, 1);
	PnlMat *path = pnl_mat_create_from_scalar(n_time_steps + 1, 1, spot_);
	int H = 50;
	PnlVect* option_values = pnl_vect_create(1);
	PnlVect* portfolio_values = pnl_vect_create(1);
	double error = 0;
	PnL(call, mc, simulated_path, n_time_steps, H, portfolio_values, option_values, error, sigma_, r);
	return 0;
}