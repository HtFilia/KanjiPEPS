#pragma once
#include "Quanto.hpp"
#include "MonteCarlo.hpp"
#include "FXBlackScholes.hpp"
#include <iostream>
#include "Hedge.hpp"
#include "pnl/pnl_matrix.h"
#include <fstream>
#include <string>
void validate_quanto(PnlRng* rng);
void validate_price_quanto(PnlMat* simulated_path, FXBlackScholes* model, MonteCarlo* mc, int n_scenarios, double rho);


