#pragma once
#include "KanjiOption.hpp"
#include "MonteCarlo.hpp"
#include "FXBlackScholes.hpp"
#include <ctime>
#include <iostream>
#include "Hedge.hpp"
#include "pnl/pnl_matrix.h"
#include <fstream>
#include <string>

void validate_kanji(PnlRng* rng);
void validate_kanjiFX(PnlRng* rng);
void validate_price_kanji(PnlMat* simulated_path, BlackScholesModel* model, MonteCarlo* mc, PnlRng *rng);
void validate_delta_kanji(PnlMat* simulated_path, BlackScholesModel* model, MonteCarlo* mc, PnlRng *rng);
void validate_mean_error_kanji(MonteCarlo* mc, BlackScholesModel* model, PnlRng *rng, int M, int H, int n_scenarios);
void histogram_errors_kanji(MonteCarlo* mc, BlackScholesModel* model, PnlRng* rng, int M, PnlVect* freqs, int scenarios);
void validate_kanjiFX(PnlRng* rng);