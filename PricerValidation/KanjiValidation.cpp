#include "KanjiValidation.hpp"

void validate_kanji(PnlRng* rng) {
	int n_time_steps = 4;
	double T = 1;
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
	int M = 64;
	int H = M;
	int n_scenarios = 20;
	PnlMat* simulated_path = pnl_mat_create(1, 1);
	//validate_price_kanji(simulated_path, model, mc, rng);
	//validate_delta_kanji(simulated_path, model, mc, rng);
	validate_mean_error_kanji(mc, model, rng, M, H, n_scenarios);
	pnl_mat_free(&simulated_path);


}

void validate_price_kanji(PnlMat* simulated_path, BlackScholesModel* model, MonteCarlo* mc, PnlRng *rng) {
	std::ofstream myfile;
	myfile.open("../Validation/price_kanji.txt");
	KanjiOption* kanji = (KanjiOption*)mc->opt_;
	PnlMat* past = pnl_mat_create(1, 1);
	int n_time_steps = kanji->nbTimeSteps_;
	double T = kanji->T_;
	int M = simulated_path->m - 1;
	model->simul_market(simulated_path, T, M, rng);
	double prix_mc = 0, ic = 0, t = 0, error = 0;
	for (double i = 0.0; i <= double(M); i++)
	{
		t = T * i / (double)M;
		model->getPast(past, simulated_path, t, n_time_steps, T);
		mc->price(past, t, prix_mc, ic);

		myfile << "prix en t = " << t << " Monte-Carlo : " << prix_mc << "\n";
	}
	myfile.close();
	pnl_mat_free(&simulated_path);
	pnl_mat_free(&past);
}


void validate_delta_kanji(PnlMat* simulated_path, BlackScholesModel* model, MonteCarlo* mc, PnlRng *rng) {
	std::ofstream myfile;
	myfile.open("../Validation/delta_kanji.txt");
	KanjiOption* kanji = (KanjiOption*)mc->opt_;
	int size = kanji->size_;
	int n_time_steps = kanji->nbTimeSteps_;
	double T = kanji->T_;
	int M = simulated_path->m - 1;
	model->simul_market(simulated_path, T, M, rng);
	double prix_mc = 0, t = 0, error = 0;
	PnlMat* past = pnl_mat_create(1, 1);
	PnlVect* delta = pnl_vect_create(size), *ic = pnl_vect_create(size);
	for (double i = 0.0; i < double(M); i++)
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
	pnl_vect_free(&delta);
	pnl_vect_free(&ic);
	pnl_mat_free(&simulated_path);
	pnl_mat_free(&past);
}

void validate_mean_error_kanji(MonteCarlo* mc, BlackScholesModel* model, PnlRng *rng, int M, int H, int n_scenarios) {
	std::ofstream myfile;
	myfile.open("../Validation/mean_error_kanji.txt");
	KanjiOption* kanji = (KanjiOption*)mc->opt_;
	int size = kanji->size_;
	int n_time_steps = kanji->nbTimeSteps_;
	double T = kanji->T_;
	PnlMat* simulated_path = pnl_mat_create(1, 1);
	model->simul_market(simulated_path, T, M, rng);
	model->simul_market(simulated_path, T, M, rng);
	PnlMat* past = pnl_mat_create(1, 1);
	PnlVect* delta = pnl_vect_create(size), *ic = pnl_vect_create(size);
	PnlVect* option_values = pnl_vect_create(1);
	PnlVect* portfolio_values = pnl_vect_create(1);
	Hedge hedge(mc);
	double mean_error = 0, error = 0;
	for (int H = M; H >= 2; H /= 2) {
		for (int i = 0; i < n_scenarios; i++) {
			mc->mod_->simul_market(simulated_path, T, M, rng);
			hedge.PnL(simulated_path, n_time_steps, H, portfolio_values, option_values, error);
			myfile << H << ";" << error;
			mean_error += error;
		}
		myfile << std::endl;
		mean_error = mean_error / n_scenarios;
		myfile << H << ";" << mean_error << std::endl;
	}
	myfile.close();
	pnl_vect_free(&portfolio_values);
	pnl_vect_free(&portfolio_values);
	pnl_mat_free(&simulated_path);
}