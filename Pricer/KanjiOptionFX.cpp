#include "KanjiOptionFX.hpp"

KanjiOptionFX::~KanjiOptionFX() {
	pnl_vect_free(&initial_values);
}

double KanjiOptionFX::payoff(const PnlMat* path) {

	double payoff = 0;
	double perf = 0;
	double timestep = T_ / (double)nbTimeSteps_;
	double mean = 0, t = 0;
	for (int i = 1; i <= nbTimeSteps_; i++) {
		for (int d = 0; d < 5; d++)
		{
			if (d == 0)
				perf += (pnl_mat_get(path, i, d) / pnl_vect_get(initial_values, d)) - 1;
			else if (d == 2)
				perf += ((pnl_mat_get(path, i, d) * (pnl_expm1(rUSD_ * i * timestep) + 1) / pnl_mat_get(path, 0, d - 1)) / (pnl_mat_get(path, 0, d) * (pnl_expm1(rUSD_ * 0 * timestep) + 1) / pnl_mat_get(path, 0, d - 1))) - 1;
			else if (d == 4)
				perf += ((pnl_mat_get(path, i, d) * (pnl_expm1(rHKD_ * i * timestep) + 1) / pnl_mat_get(path, i, d - 1)) / (pnl_mat_get(path, 0, d) * (pnl_expm1(rHKD_ * 0 * timestep) + 1) / pnl_mat_get(path, 0, d - 1))) - 1;

		}
		payoff += MAX(perf, 0);
		perf = 0;
	}

	return netAssetValue_ * (1 + (double)(60.0 / (300.0 * 16.0)) * payoff);
}