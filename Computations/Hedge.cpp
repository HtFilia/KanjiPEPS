//
// Created by boutzatz on 9/17/19.
//

#include <iostream>
#include <pnl/pnl_finance.h>
#include "Hedge.hpp"
void Hedge::PnL(const PnlMat *path, double N, PnlVect *portfolio_values, PnlVect* option_prices, double& error, double ic0) {
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
	double time = 0;
	int index=0;
	double portfolio_price = 0;
    int step = int(H/(N-1));
    for (int t = 0; t <= N - 1 ; t++) {
        past = pnl_mat_create(t + 1, size);
        spots_at_t = pnl_vect_create(size);
        pnl_mat_get_row(spots_at_t, path, t * step); //get spots at t*H/(N-1)
        //fill past with appropriate market values
        for (int factor = 0; factor <= t; factor++) {
            pnl_mat_get_row(tmpVect, path, factor * H / (N - 1));
            pnl_mat_set_row(past, tmpVect, factor);
        }
		// computing the price and delta for the discretization time
		time = t * T / (N - 1);
        mc_->price_and_delta(past, time , prix, ic_prix, delta,ic_delta);
		index = t * step;
        pnl_vect_set(option_prices, index, prix);
        if (t == 0) {
            pnl_vect_set(risk_free_part, 0, prix - pnl_vect_scalar_prod(delta, spots_at_t));
			portfolio_price = prix;
            pnl_vect_set(portfolio_values, 0, portfolio_price);
            ic0 = ic_prix;
        }
        else {
			// calculation of the delta difference which is a vector of the ammount of each asset to buy at this time
            pnl_vect_clone(delta_difference, delta);
            pnl_vect_minus_vect(delta_difference, delta_prev);
			// calculation of the portolio cash
            capitalised_t_i = pnl_vect_get(risk_free_part, t * step - 1) * (pnl_expm1(r * T / H) + 1);
            pnl_vect_set(risk_free_part, t * step,
                         capitalised_t_i - pnl_vect_scalar_prod(delta_difference, spots_at_t));
			// setting the hedging portfolio value for this time
			portfolio_price = capitalised_t_i + pnl_vect_scalar_prod(delta_prev, spots_at_t);
			index = t * step;
            pnl_vect_set(portfolio_values, index, portfolio_price);
        }
		// updating the last delta
        pnl_vect_clone(delta_prev, delta);
        for (int i = 1; i < step && (t*step + i <= H); i++) {

            pnl_mat_get_row(spots_at_t, path, t * step + i); //get spots at t*H/(N-1) + i
		// the first discretization time of this loop is not a discretization time for the monte carl algorithm
		// we need to resize the past matrix with an extra row corresponding to the the spots at the dates t*H/(N-1) + i
			if (i == 1) {
				// we resize the past matrix at the first iteration
				pnl_mat_free(&past);
				past = pnl_mat_create(t + 2, size);
				//start filling past with market data
				for (int factor = 0; factor <= t; factor++) {
					pnl_mat_get_row(tmpVect, path, factor * step );
					pnl_mat_set_row(past, tmpVect, factor);
				}
			}
            pnl_mat_get_row(tmpVect, path, t * step  + i);
            pnl_mat_set_row(past, tmpVect, t + 1);
            //end filling past
			time = t * T / (N - 1) + i * T / H;
            mc_->price_and_delta(past, time, prix, ic_prix, delta,ic_delta);
            pnl_vect_set(option_prices, t * step + i, prix);  //add to option_prices
            //risk free part calculation start
            pnl_vect_clone(delta_difference, delta);
            pnl_vect_minus_vect(delta_difference, delta_prev);
            capitalised_t_i = pnl_vect_get(risk_free_part, t * step + i - 1) * (pnl_expm1(r * T / H) + 1);
            pnl_vect_set(risk_free_part, t * step + i, capitalised_t_i - pnl_vect_scalar_prod(delta_difference, spots_at_t));
            //risk free part calculation end
            //portfolio value
			portfolio_price = capitalised_t_i + pnl_vect_scalar_prod(delta_prev, spots_at_t);
			index = t * step + i;
            pnl_vect_set(portfolio_values, index, portfolio_price);
            pnl_vect_clone(delta_prev, delta);
        }
        if (t != N-1)
            pnl_mat_free(&past);
    }
    error = pnl_vect_get(risk_free_part, H) + pnl_vect_scalar_prod(delta, spots_at_t) -mc_->opt_->payoff(past);
}
