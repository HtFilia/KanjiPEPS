#include "BasketValidation.hpp"
void validate_basket(PnlRng* rng) {
	int n_time_steps = 1;
	double T = 1;
	double r = 0.04879;
	double spot_ = 100;
	double sigma_ = 0.2;
	double strike = 100;
	int size = 5;
	PnlVect* spot = pnl_vect_create_from_scalar(size, spot_);
	PnlVect* sigma = pnl_vect_create_from_scalar(size, sigma_);
	PnlVect* trend = pnl_vect_create_from_scalar(size, 0.04);
	PnlVect* weights = pnl_vect_create_from_scalar(size, 1.0 / size);
	BlackScholesModel* model = new BlackScholesModel(size, r, 0.0, sigma, spot, trend);
	BasketOption* basket = new BasketOption(T, n_time_steps, size, weights, strike);
	int n_samples = 50000;
	MonteCarlo* mc = new MonteCarlo(model, basket, rng, T / n_time_steps, n_samples, 0.0001);
	int M = 365;
	int H = M;
	int n_scenarios = 50;
	PnlMat* simulated_path = pnl_mat_create(M + 1, 1);
	int n_freqs = 3;
	const double* freqs_ptr = new double[n_freqs] {1, 10, 30};
	PnlVect* freqs = pnl_vect_create_from_ptr(n_freqs, freqs_ptr);
	validate_delta_basket(simulated_path, model, mc, rng);
	//histogram_errors_basket(mc, model, rng, M, freqs, n_scenarios);
	//pnl_mat_free(&simulated_path);
}

void histogram_errors_basket(MonteCarlo* mc, BlackScholesModel* model, PnlRng* rng, int M, PnlVect* freqs, int scenarios) {
	double r = model->r_;
	BasketOption* kanji = (BasketOption*)mc->opt_;
	PnlMat* past = pnl_mat_create(1, 3);
	int n_time_steps = kanji->nbTimeSteps_;
	double T = kanji->T_;
	PnlMat* simulated_path = pnl_mat_create(M + 1, model->size_);
	model->simul_market(simulated_path, T, M, rng);
	Hedge hedge(mc);
	double error = 0;
	std::ofstream myfile;
	PnlVect* option_values = pnl_vect_create(M + 1);
	PnlVect* portfolio_values = pnl_vect_create(M + 1);
	double freq = 0;
	std::string filename = "";
	for (int k = 0; k < freqs->size; k++) {
		freq = GET(freqs, k);
		filename = "../Validation/basket_histogram_errors_M" + std::to_string(M) + "_freq_" + std::to_string((int)freq) + ".csv";
		myfile.open(filename);
		for (int i = 0; i < scenarios; i++) {
			model->simul_market(simulated_path, T, M, rng);
			hedge.PnLfreq(simulated_path, n_time_steps + 1, freq, portfolio_values, option_values, error); // amine
			PnlMat* comp = pnl_mat_create(M + 1, 2);
			pnl_mat_set_col(comp, portfolio_values, 0);
			pnl_mat_set_col(comp, option_values, 1);
			pnl_mat_print(comp);
			std::cout << error << std::endl;
			myfile << error << ";" << std::endl;
		}
		myfile.close();
	}
	pnl_mat_free(&simulated_path);
	pnl_mat_free(&past);
	pnl_vect_free(&option_values);
	pnl_vect_free(&portfolio_values);
}

void validate_delta_basket(PnlMat* simulated_path, BlackScholesModel* model, MonteCarlo* mc, PnlRng *rng) {
	std::ofstream myfile;
	myfile.open("../Validation/delta_basket.csv");
	BasketOption* basket = (BasketOption*)mc->opt_;
	int size = basket->size_;
	int n_time_steps = basket->nbTimeSteps_;
	double T = basket->T_;
	int M = simulated_path->m - 1;
	model->simul_market(simulated_path, T, M, rng);
	double prix_mc = 0, t = 0, error = 0;
	PnlMat* past = pnl_mat_create(1, 1);
	PnlVect* delta = pnl_vect_create(size), *ic = pnl_vect_create(size);
	myfile << "t; ESTX; USDEUR; SP500; HKDEUR; HSI\n";
	for (double i = 0.0; i < double(M); i++)
	{
		t = T * i / (double)M;
		model->getPast(past, simulated_path, t, n_time_steps, T);
		mc->delta(past, t, delta, ic);
		std::cout << t << "\t";
		myfile << t << ";";
		for (int k = 0; k < size; k++)
		{
			std::cout << pnl_vect_get(delta, k) << "\t";
			myfile << pnl_vect_get(delta, k) << "; ";
		}
		std::cout << std::endl;
		myfile << std::endl;
	}
	myfile.close();
	pnl_vect_free(&delta);
	pnl_vect_free(&ic);
	pnl_mat_free(&simulated_path);
	pnl_mat_free(&past);
}
