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
		perf += (pnl_mat_get(path, i, 0) / pnl_vect_get(initial_values, 0)) - 1;
		perf += ((pnl_mat_get(path, i, 2) * (pnl_expm1(rUSD_ * i * timestep) + 1) / pnl_mat_get(path, i, 1)) / (pnl_vect_get(initial_values, 2) * (pnl_expm1(rUSD_ * 0 * timestep) + 1) / pnl_vect_get(initial_values, 1))) - 1;
		perf += ((pnl_mat_get(path, i, 4) * (pnl_expm1(rHKD_ * i * timestep) + 1) / pnl_mat_get(path, i, 3)) / (pnl_vect_get(initial_values, 4) * (pnl_expm1(rHKD_ * 0 * timestep) + 1) / pnl_vect_get(initial_values, 3))) - 1;
		payoff += MAX(perf, 0);
		perf = 0;
	}
	return netAssetValue_ * (1 + (double)(60.0 / (300.0 * 16.0)) * payoff);
}