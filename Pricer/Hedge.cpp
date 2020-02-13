	//
// Created by boutzatz on 9/17/19.
//

#include <iostream>
#include <pnl/pnl_finance.h>
#include "Hedge.hpp"
void Hedge::PnL(PnlMat *path, int n_time_steps, int H, PnlVect *portfolio_values, PnlVect* option_prices, double& error) {
    int size = mc_->opt_->size_;
    double r = mc_->mod_->r_;
    double T = mc_->opt_->T_;
    double prix = 0;
    double ic_prix = 0;
	pnl_vect_resize(portfolio_values, H + 1);
	pnl_vect_resize(option_prices, H + 1);
    PnlMat *past = pnl_mat_create(1,1);;
    PnlVect *delta_prev = pnl_vect_create_from_scalar(size, 0);
    PnlVect *ic_delta = pnl_vect_create(size);
	int path_index=0;
	int hedging_index;
	double t= 0;
	int M = path->m - 1;
    int step = int(M/H);
	mc_->mod_->getPast(past, path, 0, n_time_steps, T); // filling the past matrix with the discrete instants preceding t and t spots
	mc_->price(prix, ic_prix);
	mc_->delta(past, 0, delta_prev, ic_delta);
	pnl_vect_set(option_prices, 0, prix);
	pnl_vect_set(portfolio_values, 0, prix);
	for (int i = 1; i < H; i++)
	{
		path_index = i * step;
		t = i * T / H;
		delta_prev = rebalance(path, path_index, i, t, n_time_steps, H, T, option_prices, portfolio_values, delta_prev);
    }

	pnl_vect_set(option_prices, H, mc_->opt_->payoff(path));
	PnlVect * prev_spots = pnl_vect_create(size);
	PnlVect * spots = pnl_vect_create(size);
	pnl_mat_get_row(prev_spots, path, M-M/H);
	double riskless_part = pnl_vect_get(portfolio_values, H - 1) - pnl_vect_scalar_prod(delta_prev, prev_spots) * exp(r * T / H);
	// (V_{i-1} - delta_{i-1} * S_{i-1}) * e^(t_i - t_{i-1})
	pnl_mat_get_row(spots, path, M);
	double risky_part = pnl_vect_scalar_prod(delta_prev, spots);
	double portfolio_price = risky_part + riskless_part;
	pnl_vect_set(portfolio_values, H, portfolio_price);
    error = pnl_vect_get(portfolio_values,H) - pnl_vect_get(option_prices, H);
	pnl_mat_free(&past);
	pnl_vect_free(&delta_prev);
	pnl_vect_free(&ic_delta);
	pnl_vect_free(&prev_spots);
	pnl_vect_free(&spots);
}

void Hedge::PnL(PnlMat *path, double N, PnlVect *portfolio_values, PnlVect* option_prices, double& error, double ic0) {
	int size = mc_->opt_->size_;
	double r = mc_->mod_->r_;
	double T = mc_->opt_->T_;
	int H = path->m - 1;
	double prix;
	double ic_prix;
	double capitalised_t_i;
	PnlMat *past;
	PnlVect *spots_at_t = pnl_vect_create(size);
	PnlVect *delta = pnl_vect_create_from_scalar(size, 0);
	PnlVect *delta_prev = pnl_vect_create_from_scalar(size, 0);
	PnlVect *delta_difference = pnl_vect_create_from_scalar(size, 0);
	PnlVect *ic_delta = pnl_vect_create(size);
	PnlVect *tmpVect = pnl_vect_create(size);
	PnlVect *risk_free_part = pnl_vect_create(H + 1);

	int step = int(H / (N - 1));

	for (int t = 0; t <= N - 1; t++) {

		past = pnl_mat_create(t + 1, size);

		spots_at_t = pnl_vect_create(size);
		pnl_mat_get_row(spots_at_t, path, t * step); //get spots at t*H/(N-1)
		//fill past with appropriate market values
		for (int factor = 0; factor <= t; factor++) {
			pnl_mat_get_row(tmpVect, path, factor * H / (N - 1));
			pnl_mat_set_row(past, tmpVect, factor);
		}
		mc_->price_and_delta(past, t * T / (N - 1), prix, ic_prix, delta, ic_delta);
		pnl_vect_set(option_prices, t *step, prix);
		if (t == 0) {
			pnl_vect_set(risk_free_part, 0, prix - pnl_vect_scalar_prod(delta, spots_at_t));

			pnl_vect_set(portfolio_values, 0, prix);
			ic0 = ic_prix;
		}
		else {
			pnl_vect_clone(delta_difference, delta);
			pnl_vect_minus_vect(delta_difference, delta_prev);
			capitalised_t_i = pnl_vect_get(risk_free_part, t * step - 1) * (pnl_expm1(r * T / H) + 1);
			pnl_vect_set(risk_free_part, t * step,
				capitalised_t_i - pnl_vect_scalar_prod(delta_difference, spots_at_t));
			pnl_vect_set(portfolio_values, t * step, capitalised_t_i + pnl_vect_scalar_prod(delta_prev, spots_at_t));
		}

		pnl_vect_clone(delta_prev, delta);
		for (int i = 1; i < step && (t*step + i <= H); i++) {

			pnl_mat_get_row(spots_at_t, path, t * step + i); //get spots at t*H/(N-1) + i

			if (i == 1) {
				pnl_mat_free(&past);
				past = pnl_mat_create(t + 2, size);
				//start filling past with market data
				for (int factor = 0; factor <= t; factor++) {
					pnl_mat_get_row(tmpVect, path, factor * step);
					pnl_mat_set_row(past, tmpVect, factor);
				}
			}
			pnl_mat_get_row(tmpVect, path, t * step + i);
			pnl_mat_set_row(past, tmpVect, t + 1);
			//end filling past
			mc_->price_and_delta(past, t * T / (N - 1) + i * T / H, prix, ic_prix, delta, ic_delta);
			pnl_vect_set(option_prices, t * step + i, prix);  //add to option_prices
			//risk free part calculation start
			pnl_vect_clone(delta_difference, delta);
			pnl_vect_minus_vect(delta_difference, delta_prev);
			capitalised_t_i = pnl_vect_get(risk_free_part, t * step + i - 1) * (pnl_expm1(r * T / H) + 1);
			pnl_vect_set(risk_free_part, t * step + i, capitalised_t_i - pnl_vect_scalar_prod(delta_difference, spots_at_t));
			//risk free part calculation end
			//portfolio value
			pnl_vect_set(portfolio_values, t * step + i, capitalised_t_i + pnl_vect_scalar_prod(delta_prev, spots_at_t));
			pnl_vect_clone(delta_prev, delta);
			// std::cout << "error at " << (t*T/(N-1) + i*T/H)<<"\t" << (pnl_vect_get(portfolio_values, step* t + i) - prix)<<"\n";
		}
		if (t != N - 1)
			pnl_mat_free(&past);
	}


	error = pnl_vect_get(risk_free_part, H) + pnl_vect_scalar_prod(delta, spots_at_t) - mc_->opt_->payoff(path);
}



PnlVect* Hedge::rebalance(PnlMat* path, int path_index, int hedging_index, double t, int n_time_steps, int H, double T, PnlVect* option_prices, PnlVect* portfolio_values, PnlVect *delta_prev)
{
	//initializing necessary variables
	int size = mc_->opt_->size_;
	PnlMat* past = pnl_mat_create(1, 1);
	PnlVect *delta = pnl_vect_create_from_scalar(size, 0);
	PnlVect *ic_delta = pnl_vect_create(size);
	double r = mc_->mod_->r_;
	double riskless_part = 0;
	double risky_part = 0;
	double portfolio_price = 0;
	double prix = 0;
	double ic_prix = 0;
	PnlVect * prev_spots = pnl_vect_create(size);
	PnlVect * spots_at_t = pnl_vect_create(size);
	int M = path->m - 1;

	pnl_mat_get_row(spots_at_t, path, path_index);// recovering the spots at t 
	int prev_spot_global_index = path_index - M / H;
	pnl_mat_get_row(prev_spots, path, prev_spot_global_index); // the previous hedging date spots
	mc_->mod_->getPast(past, path, t, n_time_steps, T); // and the correct past data
	// calculating the price and delta at t
	// mc_->price_and_delta(past, t, prix, ic_prix, delta, ic_delta);
	mc_->price(past, t, prix, ic_prix);
	mc_->delta(past, t, delta, ic_delta);
	pnl_vect_set(option_prices, hedging_index, prix); //setting the option price

	// (V_{i-1} - delta_{i-1} * S_{i-1}) * e^(t_i - t_{i-1})
	riskless_part = pnl_vect_get(portfolio_values, hedging_index-1) - pnl_vect_scalar_prod(delta_prev, prev_spots) * exp(r * T / H);
	risky_part = pnl_vect_scalar_prod(delta_prev, spots_at_t);
	portfolio_price = risky_part + riskless_part;
	pnl_vect_set(portfolio_values, hedging_index, portfolio_price);
	// returning actual delta to update the delta_prev value

	pnl_mat_free(&past);
	pnl_vect_free(&ic_delta);
	pnl_vect_free(&prev_spots);
	pnl_vect_free(&spots_at_t);
	return delta;
}