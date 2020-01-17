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
    double prix;
    double ic_prix;
    double capitalised_t_i;
    PnlMat *past = pnl_mat_create(1,1);;
    PnlVect *spots_at_t = pnl_vect_create(size);
    PnlVect *delta = pnl_vect_create_from_scalar(size,0);
    PnlVect *delta_prev = pnl_vect_create_from_scalar(size, 0);
    PnlVect *delta_difference = pnl_vect_create_from_scalar(size, 0);
    PnlVect *ic_delta = pnl_vect_create(size);
    PnlVect *tmpVect = pnl_vect_create(size);
    PnlVect *risk_free_part = pnl_vect_create(H + 1);
	int index=0;
	double portfolio_price = 0;
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
            pnl_vect_set(risk_free_part, 0, prix - pnl_vect_scalar_prod(delta, spots_at_t));
			portfolio_price = prix;
            pnl_vect_set(portfolio_values, 0, portfolio_price);
            ic0 = ic_prix;
			pnl_vect_clone(delta_prev, delta);
        }
        else {
			// calculation of the delta difference which is a vector of the ammount of each asset to buy at this time
			t = k * T / (N - 1);
			index = k * step;
			rebalance(spots_at_t, path, index, t, N, H, T, option_prices, delta_difference, delta_prev,
				delta, ic_delta, portfolio_values, risk_free_part);
		}
		// updating the last delta
        for (int i = 1; i < step && (k*step + i <= H); i++) {
			t = k * T / (N - 1) + i * T / H;
			index = k * step + i;
			rebalance(spots_at_t, path, index, t, N, H, T, option_prices, delta_difference, delta_prev,
				delta, ic_delta, portfolio_values, risk_free_part);
        }
    }
    error = pnl_vect_get(risk_free_part, H) + pnl_vect_scalar_prod(delta, spots_at_t) - pnl_vect_get(option_prices, H);
}

void Hedge::rebalance(PnlVect* spots_at_t, PnlMat* path, int index, double t, int N, int H, double T
	, PnlVect* option_prices, PnlVect* delta_difference, PnlVect* delta_prev, PnlVect* delta, PnlVect* ic_delta, PnlVect* portfolio_values, PnlVect* risk_free_part)
{
	PnlMat* past = pnl_mat_create(1, 1);
	double r = mc_->mod_->r_;
	double capitalised_t_i = 0;
	double portfolio_price = 0;
	double prix = 0;
	double ic_prix = 0;
	pnl_mat_get_row(spots_at_t, path, index);
	mc_->mod_->getPast(past, path, t, N - 1, H, T); // filling the past matrix with the discrete instants preceding t and t spots
	mc_->price_and_delta(past, t, prix, ic_prix, delta, ic_delta);
	pnl_vect_set(option_prices, index, prix);
	pnl_vect_clone(delta_difference, delta);
	pnl_vect_minus_vect(delta_difference, delta_prev);
	// calculation of the portolio cash
	capitalised_t_i = pnl_vect_get(risk_free_part, index - 1) * exp(r * T / H);
	pnl_vect_set(risk_free_part, index,
		capitalised_t_i - pnl_vect_scalar_prod(delta_difference, spots_at_t));
	// setting the hedging portfolio value for this time
	portfolio_price = capitalised_t_i + pnl_vect_scalar_prod(delta_prev, spots_at_t);
	pnl_vect_set(portfolio_values, index, portfolio_price);
	pnl_vect_clone(delta_prev, delta);
}