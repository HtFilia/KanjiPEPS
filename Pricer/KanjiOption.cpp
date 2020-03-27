//
// Created by afrai on 9/13/19.
//

#include "KanjiOption.hpp"
#include <iostream>
KanjiOption::~KanjiOption()
{
	pnl_vect_free(&initial_values);
}
double KanjiOption::payoff(const PnlMat *path) {

	double payoff = 0;
	double perf = 0;
	double mean = 0;
	for (int i = 1; i <= nbTimeSteps_; i++) {
		for (int d = 0; d < size_; d++)
		{
			perf += (pnl_mat_get(path, i, d) / pnl_vect_get(initial_values, d)) - 1;
		}
		payoff += MAX(perf, 0);
		perf = 0;
	}
	return netAssetValue_ * (1 + (double)(60.0 / (300.0*16.0))* payoff);
}