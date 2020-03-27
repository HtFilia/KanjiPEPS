#include "CallValidation.hpp"
#include "KanjiValidation.hpp"
#include "ValidateHedging.hpp"
#include "BasketValidation.hpp"
#include "QuantoValidation.hpp"


double test_fx() {
	double s0_[] = { 100, 0.1, 100,0.2, 100 };
	double trends_[] = { 0.01, 0.02, 0.03, 0.02, 0.04 };
	double sigmas_[] = { 0.2, 0.25, 0.3, 0.3, 0.2 };
	double correlation[] = { 0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1};
	for (size_t i = 0; i < 5; i++)
	{
		correlation[i + 5 * i] = 1;
	}
	int size_fx = 5;
	PnlMat* corr_mat = pnl_mat_create_from_ptr(size_fx, size_fx, correlation);
	double t = 0;
	double r = 0.01, r_US = 0.01, r_HK = 0.02;
	double T = 8;
	int n_time_steps = 16;
	double netAssetValue = 100;
	int nb_samples = 1000;
	double sigma_[] = { 0.2,0.15,0.05,0.2,0.04 };
	double S0_[] = { 1000,1500,0.9,2000,0.1 };
	PnlVect* S0 = pnl_vect_create_from_ptr(size_fx, S0_);
	PnlVect* vect_r = pnl_vect_create_from_double(size_fx, r);
	pnl_vect_set(vect_r, 1, r_US);
	pnl_vect_set(vect_r, 2, r_HK);
	PnlVect* sigma = pnl_vect_create_from_ptr(size_fx, sigma_);
	PnlVect* trend_vec = pnl_vect_create_from_double(size_fx, r);
	FXBlackScholes* model = new FXBlackScholes(size_fx, vect_r, sigma, S0, trend_vec, corr_mat);
	PnlVect* initial_values_ = pnl_vect_create_from_ptr(size_fx, S0_);
	KanjiOptionFX* kanji_fx = new KanjiOptionFX(T, n_time_steps, size_fx, initial_values_, netAssetValue, r_US, r_HK);
	PnlRng* rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));
	MonteCarlo* mc = new MonteCarlo(model, kanji_fx, rng, T / n_time_steps, nb_samples, 0.07);
	double prix = 0, ic = 0;
	mc->price(prix, ic);
	return prix;
}

double test() {
	int n_time_steps = 16;
	double T = 8;
	double r = 0.0001;
	double s0_[] = { 100, 0.1, 100,0.2, 100 };
	double sigma_[] = { 0.2,0.15,0.2};
	int size = 3;
	PnlVect *spot = pnl_vect_create_from_ptr(size, s0_);
	PnlVect *sigma = pnl_vect_create_from_ptr(size, sigma_);
	PnlVect *trend = pnl_vect_create_from_scalar(size, 0.0002);
	BlackScholesModel* model = new BlackScholesModel(size, r, 0.2, sigma, spot, trend);
	KanjiOption *kanji = new KanjiOption(T, n_time_steps, size, spot, 100);
	int n_samples = 1000;
	PnlRng* rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));
	MonteCarlo* mc = new MonteCarlo(model, kanji, rng, T / n_time_steps, n_samples, 0.07);
	double prix = 0, ic = 0;
	mc->price(prix, ic);
	return prix;
}
int main() {
	// initializing the random number generator 
	PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));
	//validate_call(rng);
	//validate_kanji(rng);
	//validate_quanto(rng);
	//validate_basket(rng);
	//validate_kanjiFX(rng);
	//validate_hedging();
	//double prix = test();
	//double prix_fx = test_fx();
	return 0;
}
