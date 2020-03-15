#include "BasketOption.hpp"

#include <iostream>
#include "BasketOption.hpp"

double BasketOption::payoff(const PnlMat* path) {

    PnlVect* V = pnl_vect_create(size_);
    pnl_mat_get_row(V, path, path->m - 1);
    double payoff = pnl_vect_scalar_prod(payOffCoeffs_, V) - strike_;
    pnl_vect_free(&V);

    if (payoff > 0)
        return payoff;
    return 0;
}
double BasketOption::payoff(const PnlMat* path, double prev_payoff, int past_length) {
    PnlVect* V = pnl_vect_create(size_);
    pnl_mat_get_row(V, path, path->m - 1);
    double payoff = pnl_vect_scalar_prod(payOffCoeffs_, V) - strike_;
    pnl_vect_free(&V);

    pnl_vect_free(&V);
    if (payoff > 0)
        return payoff;

    return 0;
}
