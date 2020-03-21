#include "KanjiValidation.hpp"

void validate_kanji(PnlRng* rng) {
	int n_time_steps = 16;
	double T = 8;
	double r = 0.001;
	int size = 3;
	double* spot_ = new double[size]{ 2641, 1563, 22311 };
	double* sigma_ = new double[size] { 0.16, 0.12, 0.14 };
	double* initial_values_ = new double[size] {2657, 1557, 22225};
	PnlVect* initial_values = pnl_vect_create_from_ptr(size, initial_values_);
	PnlVect *spot = pnl_vect_create_from_ptr(size, spot_);
	PnlVect *sigma = pnl_vect_create_from_ptr(size, sigma_);
	PnlVect *trend = pnl_vect_create_from_scalar(size, 0.0002);
	PnlVect *weights = pnl_vect_create_from_scalar(size, 1.0 / 3.0);
	double rho = 0.2;
	BlackScholesModel* model = new BlackScholesModel(size, r, rho, sigma, spot, trend);
	KanjiOption *kanji = new KanjiOption(T, n_time_steps, size, initial_values);
	int n_samples = 1000;
	MonteCarlo* mc = new MonteCarlo(model, kanji, rng, T / n_time_steps, n_samples, 0.0001);
	int M = T * 252;
	int H = M;
	int n_scenarios = 50;
	PnlMat* simulated_path = pnl_mat_create(M + 1, 1);
	//validate_price_kanji(simulated_path, model, mc, rng);
	//validate_delta_kanji(simulated_path, model, mc, rng);
	//validate_mean_error_kanji(mc, model, rng, M, H, n_scenarios);
	//int n_freqs = 1;
	//const double *freqs_ptr = new double[n_freqs] {1};
	int n_freqs = 4;
	const double *freqs_ptr = new double[n_freqs] {1, 5, 10, 30};
	PnlVect* freqs = pnl_vect_create_from_ptr(n_freqs, freqs_ptr);
	histogram_errors_kanji(mc, model, rng, M, freqs, n_scenarios);
	//pnl_mat_free(&simulated_path);
}

void histogram_errors_kanji(MonteCarlo* mc, BlackScholesModel* model, PnlRng* rng, int M, PnlVect* freqs, int scenarios) {
	double r = model->r_;
	KanjiOption* kanji = (KanjiOption*)mc->opt_;
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
		filename = "../Validation/kanji_histogram_errors_M" + std::to_string(M) + "_freq_" + std::to_string((int)freq) + ".csv";
		myfile.open(filename);
		for (int i = 0; i < scenarios; i++) {
			model->simul_market(simulated_path, T, M, rng);
			hedge.PnLfreq(simulated_path, n_time_steps + 1, freq, portfolio_values, option_values, error); // amine
			//PnlMat* comp = pnl_mat_create(M + 1, 2);
			//pnl_mat_set_col(comp, portfolio_values, 0);
			//pnl_mat_set_col(comp, option_values, 1);
			//pnl_mat_print(comp);
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
//void validate_kanjiFX(PnlRng* rng) {
//	int n_time_steps = 16;
//	double T = 8;
//	int size = 5;
//	PnlVect* r = pnl_vect_create_from_scalar(3, 0.0001);
//	pnl_vect_set(r, 1, 0.0002);
//	pnl_vect_set(r, 2, 0.0003);
//	double rho = 0.2;
//	PnlVect* sigma = pnl_vect_create_from_scalar(size, 0.2);
//	pnl_vect_set(sigma, 1, 0.02);
//	pnl_vect_set(sigma, 3, 0.02);
//	PnlVect* spot = pnl_vect_create_from_scalar(size, 1000);
//	pnl_vect_set(spot, 1, 0.89);
//	pnl_vect_set(spot, 3, 0.11);
//	PnlVect* trend = pnl_vect_create_from_scalar(size, 0.0002);
//	PnlMat* corr = pnl_mat_create_from_scalar(size, size, rho);
//	for (int diag = 0; diag < size; diag++)
//		pnl_mat_set(corr, diag, diag, 1);
//	FXBlackScholes* model = new FXBlackScholes(size, r, sigma, spot, trend, corr);
//	KanjiOption* kanji = new KanjiOption(T, n_time_steps, size, pnl_vect_get(r, 1), pnl_vect_get(r, 2));
//	int n_samples = 1000;
//	double epsilon = 0.000001;
//	double gamma = -1.0 / 4.0;
//	double epsilon_n = epsilon * pow(n_samples, -gamma);
//	MonteCarlo* mc = new MonteCarlo(model, kanji, rng, T / n_time_steps, n_samples, 0.0001);
//	int M = T * 252; //4 ans * 360j
//	int H = M;
//	int n_scenarios = 50;
//	PnlMat* simulated_path = pnl_mat_create(M + 1, size);
//	//validate_price_kanji(simulated_path, model, mc, rng);
//	//validate_delta_kanji(simulated_path, model, mc, rng);
//	//validate_mean_error_kanji(mc, model, rng, M, H, n_scenarios);
//	int n_freqs = 3;
//	const double* freqs_ptr = new double[n_freqs] {1, 10, 30};
//	PnlVect* freqs = pnl_vect_create_from_ptr(n_freqs, freqs_ptr);
//	histogram_errors_kanji(mc, model, rng, M, freqs, n_scenarios);
//	//pnl_mat_free(&simulated_path);
//}