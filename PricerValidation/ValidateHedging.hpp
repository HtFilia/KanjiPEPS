#pragma once
#include "pnl/pnl_matrix.h"
#include "MonteCarlo.hpp"
#include "VanillaCall.hpp"
#include <ctime>
#include <iostream>

double rebalance(VanillaCall* call, MonteCarlo* mc_, PnlMat* path, int path_index, int hedging_index, double t, int n_time_steps, int H, double T, PnlVect* option_prices, PnlVect* portfolio_values, double delta_prev, double sigma);
void PnL(VanillaCall* call, MonteCarlo* mc_, PnlMat*  path, int n_time_steps, int H, PnlVect *portfolio_values, PnlVect* option_prices, double& error, double sigma);
void histogram_errors(int M, BlackScholesModel* model, PnlRng* rng, VanillaCall* call, MonteCarlo* mc, int n_time_steps, double sigma_);

void validate_hedging();
