#include "CallValidation.hpp"
#include "KanjiValidation.hpp"
#include "ValidateHedging.hpp"
#include "BasketValidation.hpp"
#include "QuantoValidation.hpp"


void test_wrapper() {
	int size = 3;
	double r = 0.01;
	int nb_dates = 1440;
	double spots[] = { 100, 100, 100 };
	double trends[] = { 0.01, 0.02, 0.03 };
	double sigma_[] = { 0.2, 0.25, 0.3 };
	double correlation[] = { 1, 0.1, 0.2, 0.1, 1,-0.3, 0.2, -0.3, 1 };

	PnlVect* ic_delta = pnl_vect_create(size);
	PnlVect* delta = pnl_vect_create(size);
	PnlMat* correlation_matrix = pnl_mat_create_from_ptr(size, size, correlation);
	PnlMat *past = pnl_mat_create(nb_dates, size);
	PnlVect* S0 = pnl_vect_create(3);
	pnl_mat_get_row(S0, past, 0);
	PnlVect* sigma = pnl_vect_create_from_ptr(size, sigma_);
	PnlVect* trend_vec = pnl_vect_create_from_ptr(size, trends);
	BlackScholesModel *model = new BlackScholesModel(size, r, sigma, S0, trend_vec, correlation_matrix);
	int timesteps = 16; //voir boucle dans perf payoff

	double T = 8;
	int nb_samples = 1000;
	KanjiOption *perf_option = new KanjiOption(T, timesteps, size);
	PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));

	MonteCarlo *mc = new MonteCarlo(model, perf_option, rng, T / timesteps, nb_samples, 0.07);
	//pnl_mat_print(past);
	double prix = 0, ic = 0;
	double t = 0;
	mc->price(past, t, prix, ic);
	mc->delta(past, t, delta, ic_delta);
	//for (int i = 0; i < delta->size; i++) {
	//	deltas[i] = pnl_vect_get(delta, i);
	//	ic_deltas[i] = pnl_vect_get(ic_delta, i);
	//}

}
int main() {
	// initializing the random number generator 
	PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));
	//validate_call(rng);
	validate_kanji(rng);
	//validate_quanto(rng);
	//validate_basket(rng);
	//validate_kanjiFX(rng);
	//validate_hedging();
	//test_wrapper();


	return 0;
}
