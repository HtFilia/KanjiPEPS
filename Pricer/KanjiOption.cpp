//
// Created by afrai on 9/13/19.
//

#include "KanjiOption.hpp"

double KanjiOption::payoff(const PnlMat *path) {

    double payoff = 0;
	double perf = 0;
    PnlVect *initial_values = pnl_vect_create(size_);
	pnl_mat_get_row(initial_values, path, 0);
	double timestep = T_ / (double)nbTimeSteps_;
    double mean = 0, t = 0;
    for (int i = 1; i <= nbTimeSteps_; i++) {
		for (int d = 0; d < size_; d++)
		{
			perf = perf + pnl_mat_get(path, i, d)/pnl_vect_get(initial_values,d) - 1;
		}
		payoff = MAX(perf, 0);
		perf = 0;
    }
    pnl_vect_free(&initial_values);
    return payoff;
}
