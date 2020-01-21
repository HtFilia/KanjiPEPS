#include "VanillaCall.hpp"
#include "KanjiOption.hpp"
#include "MonteCarlo.hpp"
#include <ctime>
#include <iostream>
#include "Hedge.hpp"
#include "pnl/pnl_matrix.h"
#include <fstream>

void validate_price_call(PnlMat* simulated_path, VanillaCall* call, double r, double sigma_, double T, double strike, MonteCarlo* mc,
	_PnlMat* past, BlackScholesModel* model, int n_time_steps) {
	std::ofstream myfile;
	myfile.open("../Validation/price_vanilla.txt");
	double spot_at_t = 0;
	double prix_mc = 0;
	double ic = 0;
	double prix = 0;
	double t = 0;
	int count = 0;
	int M = simulated_path->m - 1;
	myfile << " Validation price" << std::endl;
	for (double i = 0.0; i <= double(M); i++)
	{
		t = T * i / (double)M;
		model->getPast(past, simulated_path, t, n_time_steps, T);
		spot_at_t = MGET(simulated_path, int(i), 0);
		prix = call->price(t, spot_at_t, r, sigma_, T, strike);
		mc->price(past, t, prix_mc, ic);
		myfile << "prix en t = " << t << " Monte-Carlo : " << prix_mc;
		myfile << " formule fermee: " << prix;
		if (prix_mc - ic / 2 <= prix && prix <= prix_mc + ic / 2)
		{
			myfile << " in ";
			count++;
		}
		else
		{
			myfile << " not in ";
		}
		myfile << "   [" << prix_mc - ic / 2 << ", " << prix_mc + ic / 2 << "] " << std::endl;
	}
	myfile << std::endl << count << " / " << (int)(M + 1) << "sont dans l'intervalle de confiance" << std::endl;
	myfile.close();
}

void validate_delta_call(PnlMat* simulated_path, VanillaCall* call, double r, double sigma_, double T, double strike, MonteCarlo* mc,
	PnlMat* past, BlackScholesModel* model, int n_time_steps) {
	PnlVect* delta = pnl_vect_create(1);
	PnlVect* ic_delta = pnl_vect_create(1);
	std::ofstream myfile;
	myfile.open("../Validation/delta_vanilla.txt");
	double spot_at_t = 0;
	double delta_ferme = 0;
	double delta_ = 0;
	double ic_delta_ = 0;
	double t = 0;
	int count = 0;
	int M = simulated_path->m - 1;
	myfile << " Validation delta " << std::endl;
	for (double i = 0.0; i <= (double)M; i++)
	{
		t = T * i / (double)M;
		model->getPast(past, simulated_path, t, n_time_steps, T);
		spot_at_t = MGET(simulated_path, (int)i, 0);
		delta_ferme = call->delta(t, spot_at_t, r, sigma_, T, strike);
		mc->delta(past, t, delta, ic_delta);
		delta_ = pnl_vect_get(delta, 0);
		ic_delta_ = pnl_vect_get(ic_delta, 0);
		myfile << "delta en t = " << t << " Monte-Carlo : " << delta_;
		myfile << " formule fermee: " << delta_ferme;
		if (delta_ - ic_delta_ / 2 <= delta_ferme && delta_ferme <= delta_ + ic_delta_ / 2)
		{
			myfile << " in ";
			count++;
		}
		else
		{
			myfile << " not in ";
		}
		myfile << "   [" << delta_ - ic_delta_ / 2 << ", " << delta_ + ic_delta_ / 2 << "] " << std::endl;
	}
	myfile << std::endl << count << " / " << int(M + 1) << "sont dans l'intervalle de confiance" << std::endl;
}

void validate_hedging_frequency_call(MonteCarlo* mc, PnlMat* simulated_path, int nbTimeSteps, double strike) {
	Hedge hedge(mc);
	int M = simulated_path->m - 1;
	double error = 0;
	std::ofstream myfile;
	myfile.open("../Validation/variation_frequence.txt");
	myfile << "Variation de la fréquence de rebalancement pour " << M << " dates fournies" << std::endl;
	myfile << "Option vanille de spot " << MGET(simulated_path, 0, 0) << " de strike " << strike << " de maturité " << mc->opt_->T_ << std::endl;
	PnlVect* option_values = pnl_vect_create(1);
	PnlVect* portfolio_values = pnl_vect_create(1);
	for (int H = M / 2; H >= 2; H = H / 2) {
		hedge.PnL(simulated_path, nbTimeSteps, H, portfolio_values, option_values, error);
		myfile << "Erreur pour " << H << " dates de rebalancement : " << error << std::endl;
	}
	myfile.close();
}

void validate_mean_error_kanji(PnlRng *rng) {
	std::ofstream myfile;
	myfile.open("../Validation/mean_error_kanji.txt");
	int M = 160;
	int n_time_steps = 16;
	double T = 1.0;
	double r = 0.02;
	double spot_ = 109;
	double sigma_ = 0.2;
	int size = 3;
	PnlVect *spot = pnl_vect_create_from_scalar(size, spot_);
	PnlVect *sigma = pnl_vect_create_from_scalar(size, sigma_);
	PnlVect *trend = pnl_vect_create_from_scalar(size, r);
	BlackScholesModel* model = new BlackScholesModel(size, r, 0, sigma, spot, trend);
	KanjiOption *kanji = new KanjiOption(T, n_time_steps, size);
	int n_samples = 50000;
	double epsilon = 0.000001;
	double gamma = -1.0 / 4.0;
	double epsilon_n = epsilon * pow(n_samples, -gamma);
	MonteCarlo* mc = new MonteCarlo(model, kanji, rng, T / n_time_steps, n_samples, epsilon_n);
	int H = 32;
	PnlMat* simulated_path = pnl_mat_create(M, 1);
	double error = 0;
	int n_scenarios = 10;
	PnlVect* option_values = pnl_vect_create(1);
	PnlVect* portfolio_values = pnl_vect_create(1);
	Hedge hedge(mc);
	double mean_error = 0;
	myfile << " Calcul de l'erreur moyenne sur la couverture d'un produit kanji de maturité " << T << " ans sur des indices de spot initials " << spot_ << std::endl;
	myfile << " Avec " << M + 1 << " dates de constatations et " << H << " dates de rebalancement" << std::endl;
	for (int i = 0; i < n_scenarios; i++) {
		mc->mod_->simul_market(simulated_path, T, M, rng);
		hedge.PnL(simulated_path, n_time_steps, H, portfolio_values, option_values, error);
		myfile << " Erreur sur le scénario " << i << " : " << error << std::endl;
		mean_error += error;
	}
	mean_error = mean_error / n_scenarios;
	myfile << "Erreur moyenne sur " << n_scenarios << " scénarios : " << mean_error;
	myfile.close();
}


void validate_price_kanji(PnlRng *rng) {
	std::ofstream myfile;
	myfile.open("../Validation/price_kanji.txt");
	int M = 160;
	int n_time_steps = 16;
	double T = 345;
	double r = 0.02;
	double spot_ = 100;
	double sigma_ = 0.2;
	int size = 3;
	PnlVect *spot = pnl_vect_create_from_scalar(size, spot_);
	PnlVect *sigma = pnl_vect_create_from_scalar(size, sigma_);
	PnlVect *trend = pnl_vect_create_from_scalar(size, r);
	PnlVect *weights = pnl_vect_create_from_scalar(size, 1.0 / 3.0);
	BlackScholesModel* model = new BlackScholesModel(size, r, 0, sigma, spot, trend);
	KanjiOption *kanji = new KanjiOption(T, n_time_steps, size);
	int n_samples = 50000;
	double epsilon = 0.000001;
	double gamma = -1.0 / 4.0;
	double epsilon_n = epsilon * pow(n_samples, -gamma);
	MonteCarlo* mc = new MonteCarlo(model, kanji, rng, T / n_time_steps, n_samples, epsilon_n);
	PnlMat* simulated_path = pnl_mat_create(M + 1, 1);
	model->simul_market(simulated_path, T, M, rng);
	double prix_mc = 0, ic = 0, t = 0, error = 0;
	PnlMat* past = pnl_mat_create(1, 1);
	for (double i = 0.0; i <= double(M); i++)
	{
		t = T * i / (double)M;
		model->getPast(past, simulated_path, t, n_time_steps, T);
		mc->price(past, t, prix_mc, ic);
		myfile << "prix en t = " << t << " Monte-Carlo : " << prix_mc << "\n";
	}
	myfile.close();
}



void validate_delta_kanji(PnlRng *rng) {
	std::ofstream myfile;
	myfile.open("../Validation/delta_kanji.txt");
	int M = 160;
	int n_time_steps = 16;
	double T = 1.0;
	double r = 0.02;
	double spot_ = 100;
	double sigma_ = 0.2;
	int size = 3;
	PnlVect *spot = pnl_vect_create_from_scalar(size, spot_);
	PnlVect *sigma = pnl_vect_create_from_scalar(size, sigma_);
	PnlVect *trend = pnl_vect_create_from_scalar(size, r);
	PnlVect *weights = pnl_vect_create_from_scalar(size, 1.0 / 3.0);
	BlackScholesModel* model = new BlackScholesModel(size, r, 0.5, sigma, spot, trend);
	KanjiOption *kanji = new KanjiOption(T, n_time_steps, size);
	int n_samples = 50000;
	double epsilon = 0.01;
	double gamma = -1.0 / 4.0;
	double epsilon_n = epsilon * pow(n_samples, -gamma);
	MonteCarlo* mc = new MonteCarlo(model, kanji, rng, T / n_time_steps, n_samples, epsilon_n);
	int H = 50;
	PnlMat* simulated_path = pnl_mat_create(M + 1, 3);
	model->simul_market(simulated_path, T, M, rng);
	int n_scenarios = 10;
	double t = 0, error = 0;
	PnlMat* past = pnl_mat_create(1, 1);
	PnlVect* delta = pnl_vect_create(size), *ic = pnl_vect_create(size);
	for (double i = 0.0; i <= double(M); i++)
	{
		t = T * i / (double)M;
		model->getPast(past, simulated_path, t, n_time_steps, T);
		mc->delta(past, t, delta, ic);
		myfile << "delta en t = " << t << " Monte-Carlo : ";
		for (int i = 0; i < size; i++)
		{
			myfile << pnl_vect_get(delta, i) << " ";
		}
		myfile << std::endl;
	}
	myfile.close();
}


int main() {
	// initializing the random number generator 
	PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));

	// initializing the model and the call option
	double r = 0.02;
	double spot_ = 109;
	double sigma_ = 0.2;
	PnlVect *spot = pnl_vect_create_from_scalar(1, spot_);
	PnlVect *sigma = pnl_vect_create_from_scalar(1, sigma_);
	PnlVect *trend = pnl_vect_create_from_scalar(1, r);
	double T = 1;
	double strike = 100;
	BlackScholesModel* model = new BlackScholesModel(1, r, 0, sigma, spot, trend);
	int n_time_steps = 10;
	VanillaCall* call = new VanillaCall(T, n_time_steps, 1, strike);

	// initializing the montecarlo tool
	int n_samples = 50000;
	double epsilon = 0.000001;
	double gamma = -1.0 / 4.0;
	double epsilon_n = epsilon * pow(n_samples, -gamma);
	MonteCarlo* mc = new MonteCarlo(model, call, rng, T / n_time_steps, n_samples, epsilon_n);
	double ic = 0;

	// simulating the data
	int M = 100;
	PnlMat* simulated_path = pnl_mat_create(M, 1);
	model->simul_market(simulated_path, T, M, rng);
	// computing the price 
	PnlMat *past = pnl_mat_create(1, 1);
	PnlMat *path = pnl_mat_create_from_scalar(n_time_steps + 1, 1, spot_);


	//validate_price_call(simulated_path, call, r, sigma_, T, strike, mc, past, model, n_time_steps);

	//validate_delta_call(simulated_path, call, r, sigma_, T, strike, mc,past, model, n_time_steps);

	//validate_hedging_frequency_call(mc, simulated_path, n_time_steps, strike);

	validate_price_kanji(rng);

	validate_delta_kanji(rng);

	validate_mean_error_kanji(rng);

	return 0;
}
