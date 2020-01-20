//
// Created by afrai on 9/13/19.
//

#include "PerformanceOption.hpp"

double PerformanceOption::payoff(const PnlMat *path) {

    double payoff = 0;
    PnlVect *values = pnl_vect_create(size_);
    pnl_mat_get_row(values, path, 0);
    double prev_mean = pnl_vect_scalar_prod(weights_, values);
    double mean;
    for (int i = 1; i <= path->m - 1; i++) {
        pnl_mat_get_row(values, path, i);
        mean = pnl_vect_scalar_prod(weights_, values);
        payoff += (mean < prev_mean) ? 0 : (mean/prev_mean - 1);
        prev_mean = mean;
    }
    pnl_vect_free(&values);
    return payoff;
}
