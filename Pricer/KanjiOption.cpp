//
// Created by afrai on 9/13/19.
//

#include "KanjiOption.hpp"

double KanjiOption::payoff(const PnlMat *path) {

    double payoff = 0;
	double perf = 0;
    PnlVect *values = pnl_vect_create(size_);
	int N = 16;
	double timestep = T_ / (double)N;
    pnl_mat_get_row(values, path, 0);
    double mean = 0, t = 0;
	int index;
    for (int i = 1; i <= 16; i++) {
		t = i * timestep;
		index = 
		for (int d = 0; d < size_; d++)
		{
			perf = perf + pnl_mat_get(path, index, d);
		}
        pnl_mat_get_row(values, path, i);
        mean = pnl_vect_scalar_prod(weights_, values);
        payoff += (mean < prev_mean) ? 0 : (mean/prev_mean - 1);
        prev_mean = mean;
    }
    pnl_vect_free(&values);
    return payoff;
}
