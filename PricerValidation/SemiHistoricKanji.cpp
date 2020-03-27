#include "SemiHistoricKanji.hpp"
#include "DataMarketReader.h"
#include <iostream>
#include <cstring>
#include <string>

void validate_kanji_market(PnlRng* rng){
	int n_time_steps = 16;
	double T = 8;
	double r = 0.001;
	int size = 3;
	double* spot_ = new double[size] { 2641, 1563, 22311 };
	double* sigma_ = new double[size] { 0.16, 0.12, 0.14 };
	double* initial_values_ = new double[size] {2657, 1557, 22225};
	PnlVect* initial_values = pnl_vect_create_from_ptr(size, initial_values_);
	PnlVect* spot = pnl_vect_create_from_ptr(size, spot_);
	PnlVect* sigma = pnl_vect_create_from_ptr(size, sigma_);
	PnlVect* trend = pnl_vect_create_from_scalar(size, 0.0002);
	PnlVect* weights = pnl_vect_create_from_scalar(size, 1.0 / 3.0);
	double rho = 0.2;
	BlackScholesModel* model = new BlackScholesModel(size, r, rho, sigma, spot, trend);
	KanjiOption* kanji = new KanjiOption(T, n_time_steps, size, initial_values);
	int n_samples = 1000;
	MonteCarlo* mc = new MonteCarlo(model, kanji, rng, T / n_time_steps, n_samples, 0.0001);
	int M = T * 252;
	int H = M;

	std::string marketfile =  "../kanji_market.dat";
	DataMarketReader* m = new DataMarketReader;
	char* cstr = &marketfile[0];
	PnlMat* path = m->readMarketData(cstr);
	int n_freqs = 4;
	const double* freqs_ptr = new double[n_freqs] {1, 2, 4, 6};
	PnlVect* freqs = pnl_vect_create_from_ptr(n_freqs, freqs_ptr);

	Hedge hedge(mc);
	double error = 0;
	std::ofstream myfile;
	PnlVect* option_values = pnl_vect_create(path->m);
	PnlVect* portfolio_values = pnl_vect_create(path->m);
	double freq = 0;
	std::string filename = "";
	filename = "../Validation/kanji_historic_errors_freqs.csv";
	myfile.open(filename);
	for (int k = 0; k < freqs->size; k++) {
		freq = GET(freqs, k);

		hedge.PnLfreq(path, n_time_steps + 1, freq, portfolio_values, option_values, error); // amine
		PnlMat* comp = pnl_mat_create(path->m +  1, 2);
		std::cout << error << std::endl;
		myfile << "freq: chaque " << freq << "jours, PnL: " << error  << std::endl;
		
	}
	myfile.close();
	pnl_mat_free(&path);
	pnl_vect_free(&option_values);
	pnl_vect_free(&portfolio_values);
}