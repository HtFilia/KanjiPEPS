#include "KanjiValidation.hpp"
#include <chrono>

void validate_kanji(PnlRng* rng) {
	int n_time_steps = 16;
	double T = 8;
	double r = 0.0001;
	double spot_ = 1000;
	double sigma_ = 0.2;
	int size = 3;
	PnlVect *spot = pnl_vect_create_from_scalar(size, spot_);
	PnlVect *sigma = pnl_vect_create_from_scalar(size, sigma_);
	PnlVect *trend = pnl_vect_create_from_scalar(size, 0.0002);
	PnlVect *weights = pnl_vect_create_from_scalar(size, 1.0 / 3.0);
	BlackScholesModel* model = new BlackScholesModel(size, r, 0.2, sigma, spot, trend);
	KanjiOption *kanji = new KanjiOption(T, n_time_steps, size, spot, 100);
	int n_samples = 1000;
	MonteCarlo* mc = new MonteCarlo(model, kanji, rng, T / n_time_steps, n_samples, 0.0001);
	int M = T*252;
	int H = M;
	int n_scenarios = 50;
	PnlMat* simulated_path = pnl_mat_create(M+1, 1);
	//validate_price_kanji(simulated_path, model, mc, rng);
	//validate_delta_kanji(simulated_path, model, mc, rng);
	//validate_mean_error_kanji(mc, model, rng, M, H, n_scenarios);
	int n_freqs = 5;
	const double *freqs_ptr = new double[n_freqs] {1, 5, 10, 30, 50};
	PnlVect* freqs = pnl_vect_create_from_ptr(n_freqs, freqs_ptr);
	histogram_errors_kanji(mc, model, rng, M, freqs, n_scenarios);
	//pnl_mat_free(&simulated_path);
	//validateFX_Kanji();
	//validate_kanjiFX(rng);
}

void histogram_errors_kanji(MonteCarlo* mc, BlackScholesModel* model, PnlRng* rng, int M, PnlVect* freqs, int scenarios) {
	double r = model->r_;
	KanjiOptionFX* kanji = (KanjiOptionFX*)mc->opt_; 
	int n_time_steps = kanji->nbTimeSteps_;
	double T = kanji->T_;
	PnlMat* simulated_path = pnl_mat_create(M+1, model->size_);
	model->simul_market(simulated_path, T, M, rng);
	Hedge hedge(mc);
	double error = 0;
	std::ofstream myfile;
	PnlVect* option_values = pnl_vect_create(M + 1);
	PnlVect* portfolio_values = pnl_vect_create(M + 1);
	double freq = 0;
	std::string filename = "";
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	for (int k = 0; k < freqs->size; k++) {
		freq = GET(freqs, k);
		filename = "../Validation/fx_kanji_histogram_errors_M" + std::to_string(M) + "_freq_" + std::to_string((int)freq) + ".csv";
		myfile.open(filename);
		for (int i = 0; i < scenarios; i++) {
			model->simul_market(simulated_path, T, M, rng);
			hedge.PnLfreq(simulated_path, n_time_steps + 1, freq, portfolio_values, option_values, error); 
			//PnlMat* comp = pnl_mat_create(M + 1, 2);
			//pnl_mat_set_col(comp, portfolio_values, 0);
			//pnl_mat_set_col(comp, option_values, 1);
			//pnl_mat_print(comp);
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			//std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
			std::cout << error << std::endl;
			myfile << error << ";\n";
		}
		myfile.close();
	}
	pnl_mat_free(&simulated_path);
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
			std::cout << pnl_vect_get(delta, i) << "\n";
			myfile << pnl_vect_get(delta, i) << " ";
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


void validateFX_Kanji() {
	int size_fx = 5;
	PnlMat* correlation_matrix = pnl_mat_create_from_double(size_fx, size_fx, 0.1);
	for (int i = 0; i < size_fx; i++)
	{
		pnl_mat_set(correlation_matrix, i, i, 1);
	}
	double r = 0.01, r_US = 0.01, r_HK = 0.02;
	double T = 8;
	int n_time_steps = 16;
	double netAssetValue = 100;
	int nb_samples = 1000;
	double sigma_[] = { 0.2,0.15,0.05,0.2,0.04 };
	double past_[] = { 1000,1500,0.9,2000,0.1 };
	PnlMat* past = pnl_mat_create_from_ptr(1, size_fx, past_);
	PnlVect* vect_r = pnl_vect_create_from_double(size_fx, r);
	pnl_vect_set(vect_r, 1, r_US);
	pnl_vect_set(vect_r, 2, r_HK);
	PnlVect* S0 = pnl_vect_create(size_fx);
	pnl_mat_get_row(S0, past, 0);
	PnlVect* sigma = pnl_vect_create_from_ptr(size_fx, sigma_);
	PnlVect* trend_vec = pnl_vect_create_from_double(size_fx, r);
	FXBlackScholes* model = new FXBlackScholes(size_fx, vect_r, sigma, S0, trend_vec, correlation_matrix);
	PnlVect* initial_values_ = pnl_vect_create_from_ptr(size_fx, past_);
	KanjiOptionFX* kanji_fx = new KanjiOptionFX(T, n_time_steps, size_fx, initial_values_, netAssetValue, r_US, r_HK);
	PnlRng* rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));
	MonteCarlo* mc = new MonteCarlo(model, kanji_fx, rng, T / n_time_steps, nb_samples, 0.07);
	double prix = 0, ic = 0;
	PnlVect* delta = pnl_vect_create(size_fx);
	PnlVect* ic_delta = pnl_vect_create(size_fx);
	mc->price(past, 0, prix, ic);
	mc->delta(past, 0, delta, ic_delta);
	pnl_vect_print(delta);
}

void validate_kanjiFX(PnlRng* rng) {
	int n_time_steps = 16;
	double T = 8;
	int size = 5;
	PnlVect* r = pnl_vect_create_from_scalar(3, 0.0001);
	pnl_vect_set(r, 1, 0.0002);
	pnl_vect_set(r, 2, 0.0003);
	double rho = 0.2;
	PnlVect* sigma = pnl_vect_create_from_scalar(size, 0.2);
	pnl_vect_set(sigma, 1, 0.02);
	pnl_vect_set(sigma, 3, 0.02);
	PnlVect* spot = pnl_vect_create_from_scalar(size, 1000);
	pnl_vect_set(spot, 1, 0.89);
	pnl_vect_set(spot, 3, 0.11);
	PnlVect* trend = pnl_vect_create_from_scalar(size, 0.0002);
	PnlMat* corr = pnl_mat_create_from_scalar(size, size, rho);
	for (int diag = 0; diag < size; diag++)
		pnl_mat_set(corr, diag, diag, 1);
	FXBlackScholes* model = new FXBlackScholes(size, r, sigma, spot, trend, corr);
	KanjiOption* kanji = new KanjiOptionFX(T, n_time_steps, size,spot,100, pnl_vect_get(r, 1), pnl_vect_get(r, 2));
	int n_samples = 1000;
	double epsilon = 0.000001;
	double gamma = -1.0 / 4.0;
	double epsilon_n = epsilon * pow(n_samples, -gamma);
	MonteCarlo* mc = new MonteCarlo(model, kanji, rng, T / n_time_steps, n_samples, 0.0001);
	int M = T * 252; //4 ans * 360j
	int H = M;
	int n_scenarios = 50;
	PnlMat* simulated_path = pnl_mat_create(M + 1, size);
	//validate_price_kanji(simulated_path, model, mc, rng);
	//validate_delta_kanji(simulated_path, model, mc, rng);
	//validate_mean_error_kanji(mc, model, rng, M, H, n_scenarios);
	int n_freqs = 5;
	const double* freqs_ptr = new double[n_freqs] {1, 5, 10, 30, 50};
	PnlVect* freqs = pnl_vect_create_from_ptr(n_freqs, freqs_ptr);
	histogram_errors_kanji(mc, model, rng, M, freqs, n_scenarios);
	//pnl_mat_free(&simulated_path);
}