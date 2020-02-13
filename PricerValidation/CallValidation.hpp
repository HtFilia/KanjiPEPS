#pragma once
#include "VanillaCall.hpp"
#include "MonteCarlo.hpp"
#include <ctime>
#include <iostream>
#include "Hedge.hpp"
#include "pnl/pnl_matrix.h"
#include <fstream>
#include "pnl/pnl_random.h"


void validate_call(PnlRng* rng);
void validate_price_call(PnlMat* simulated_path, MonteCarlo* mc, BlackScholesModel* model, int n_scenarios);
void validate_delta_call(PnlMat* simulated_path, MonteCarlo* mc, BlackScholesModel* model, int n_scenarios);
void validate_hedging_frequency_call(MonteCarlo* mc, BlackScholesModel* model, PnlRng* rng, int M);
void validate_mean_error_call(MonteCarlo* mc, BlackScholesModel* model, PnlRng *rng, int M, int n_scenarios);
void histogram_errors_call(MonteCarlo* mc, BlackScholesModel* model, PnlRng* rng, int M, int H, int scenarios);