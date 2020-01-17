//
// Created by boutzatz on 9/17/19.
//

#include <iostream>
#include <pnl/pnl_finance.h>
#include "Hedge.hpp"
void Hedge::PnL(PnlMat *path, double N, PnlVect *portfolio_values, PnlVect* option_prices, double& error, double ic0) {
    int size = mc_->opt_->size_;
    double r = mc_->mod_->r_;
    double T = mc_->opt_->T_;
    int H = path->m - 1;
    double prix = 0;
    double ic_prix = 0;
    PnlMat *past = pnl_mat_create(1,1);;
    PnlVect *spots_at_t = pnl_vect_create(size);
    PnlVect *delta = pnl_vect_create_from_scalar(size,0);
    PnlVect *delta_prev = pnl_vect_create_from_scalar(size, 0);
    PnlVect *ic_delta = pnl_vect_create(size);
	int index=0;
	double t= 0;
    int step = int(H/(N-1));
    for (int k = 0; k <= N - 1 ; k++) {
		//get spots at k*H/(N-1)
		// computing the price and delta for the discretization time
        if (k == 0) {
			t = k * step;
			mc_->mod_->getPast(past, path, t, N - 1, H, T); // filling the past matrix with the discrete instants preceding t and t spots
			pnl_mat_get_row(spots_at_t, path, t);
			t = k * T / (N - 1);
			mc_->price_and_delta(past, t, prix, ic_prix, delta, ic_delta);
			index = k * step;
			pnl_vect_set(option_prices, index, prix);
            pnl_vect_set(portfolio_values, 0, prix);
            ic0 = ic_prix;
			pnl_vect_clone(delta_prev, delta);
        }
        else {
			t = k * T / (N - 1);
			index = k * step;
			delta_prev = rebalance(path, index, t, N, H, T, option_prices, portfolio_values,  delta_prev);
		}
        for (int i = 1; i < step && (k*step + i <= H); i++) {
			t = k * T / (N - 1) + i * T / H;
			index = k * step + i;
			delta_prev = rebalance(path, index, t, N, H, T, option_prices, portfolio_values, delta_prev);
        }
    }
    error = pnl_vect_get(portfolio_values,H) - pnl_vect_get(option_prices, H);
}

PnlVect* Hedge::rebalance(PnlMat* path, int index, double t, int N, int H, double T, PnlVect* option_prices, PnlVect* portfolio_values, PnlVect *delta_prev)
{
	//initializing necessary variables
	int size = mc_->opt_->size_;
	PnlMat* past = pnl_mat_create(1, 1);
	PnlVect *delta = pnl_vect_create_from_scalar(size, 0);
	PnlVect *ic_delta = pnl_vect_create(size);
	double r = mc_->mod_->r_;
	double capitalised_t_i = 0;
	double portfolio_price = 0;
	double prix = 0;
	double ic_prix = 0;
	PnlVect * spots_before_t = pnl_vect_create(size);
	PnlVect * spots_at_t = pnl_vect_create(size);

	// recovering the spots at t , the previous spots and the correct past data
	pnl_mat_get_row(spots_at_t, path, index);
	pnl_mat_get_row(spots_before_t, path, index - 1);
	mc_->mod_->getPast(past, path, t, N - 1, H, T);
	// calculating the price and delta at t
	mc_->price_and_delta(past, t, prix, ic_prix, delta, ic_delta);
	pnl_vect_set(option_prices, index, prix);

	// (V_{i-1} - delta_{i-1} * S_{i-1}) * e^(t_i - t_{i-1})
	capitalised_t_i = pnl_vect_get(portfolio_values, index-1) - pnl_vect_scalar_prod(delta_prev, spots_before_t) * exp(r * T / H);
	portfolio_price = pnl_vect_scalar_prod(delta_prev, spots_at_t) + capitalised_t_i;
	pnl_vect_set(portfolio_values, index, portfolio_price);

	// returning actual delta to update the delta_prev value
	return delta;
}