#pragma once

#pragma once
#include "BasketOption.hpp"
#include "MonteCarlo.hpp"
#include "BlackScholesModel.hpp"
#include <ctime>
#include <iostream>
#include "Hedge.hpp"
#include "pnl/pnl_matrix.h"
#include <fstream>
#include <string>
void validate_basket(PnlRng* rng);
void histogram_errors_basket(MonteCarlo* mc, BlackScholesModel* model, PnlRng* rng, int M, PnlVect* freqs, int scenarios);
