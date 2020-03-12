#include "CallValidation.hpp"
#include "KanjiValidation.hpp"
#include "ValidateHedging.hpp"
#include "QuantoValidation.hpp"

int main() {
	// initializing the random number generator 
	PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));
	//validate_call(rng);
	validate_kanji(rng);
	//validate_kanjiFX(rng);
	//validate_hedging();
	//validate_quanto(rng);
	return 0;
}
