#include "CallValidation.hpp"
#include "KanjiValidation.hpp"
#include "ValidateHedging.hpp"
#include "BasketValidation.hpp"
#include "QuantoValidation.hpp"


void test_wrapper() {
	int size = 3;
	double r = 0.01;
	int nb_dates = 1046;
	double s0_[] = { 100, 0.1, 100,0.2, 100 };
	double trends_[] = { 0.01, 0.02, 0.03, 0.02, 0.04 };
	double sigmas_[] = { 0.2, 0.25, 0.3, 0.3, 0.2 };
	double correlation[] = { 0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1};
	for (size_t i = 0; i < 5; i++)
	{
		correlation[i + 5 * i] = 1;
	}
	int size_fx = 5;
	PnlMat* path = pnl_mat_create(nb_dates + 1, size_fx);
	PnlVect* s0 = pnl_vect_create_from_ptr(size_fx, s0_);
	PnlVect* sigma = pnl_vect_create_from_ptr(size_fx, sigmas_);
	PnlVect* trend_vec = pnl_vect_create_from_ptr(size_fx, trends_);
	PnlMat* corr_mat = pnl_mat_create_from_ptr(size_fx, size_fx, correlation);
	PnlVect* vect_r = pnl_vect_create_from_double(size, r);
	pnl_vect_set(vect_r, 1, r);
	pnl_vect_set(vect_r, 2, r);
	FXBlackScholes* model = new FXBlackScholes(size_fx, vect_r, sigma, s0, trend_vec, corr_mat);
	PnlRng* rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));
	double maturity = 8.7896825396825395;
	double t = 4.6428571428571432;
	model->simul_market(path, maturity - t, nb_dates, rng);
	pnl_mat_print(path);
	double *path_ = new double[size_fx*nb_dates];
 	for (int index_row = 0; index_row < path->m; index_row++) {
		for (int index_col = 0; index_col < path->n; index_col++) {
			path_[index_row * path->n + index_col] = pnl_mat_get(path, index_row, index_col);
		}
	}
}
int main() {
	// initializing the random number generator 
	PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));
	//validate_call(rng);
	//validate_kanji(rng);
	//validate_quanto(rng);
	//validate_basket(rng);
	validate_kanjiFX(rng);
	//validate_hedging();
	//test_wrapper();


	return 0;
}
