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
void validate_kanji_market(PnlRng* rng);
