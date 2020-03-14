#include "QuantoValidation.hpp"

void validate_quanto(PnlRng* rng) {
	int n_time_steps = 1;
	double T = 1;
	int size = 2;

	const double* list_r = new double[size] {0.02, 0.05};
	PnlVect* r = pnl_vect_create_from_ptr(size, list_r);
	const double* list_sigmas = new double[size] { 0.2, 0.01};
	PnlVect* sigmas = pnl_vect_create_from_ptr(size, list_sigmas);
	double rho = 0.2;
	PnlMat* corr = pnl_mat_create_from_scalar(size, size, rho);
	for (int diag = 0; diag < size; diag++)
		pnl_mat_set(corr, diag, diag, 1);
	const double* list_spots = new double[size] { 100, 0.89 };
	PnlVect* spots = pnl_vect_create_from_ptr(size, list_spots);

	const double* list_trends = new double[size] {0.02, -0.03};
	PnlVect* trends = pnl_vect_create_from_ptr(size, list_trends);
	Quanto* quanto = new Quanto(T, n_time_steps, size, 90, r);
	FXBlackScholes* model = new FXBlackScholes(size, r, sigmas, spots, trends, corr);
	int n_samples = 50000;
	double epsilon = 0.000001;
	double gamma = -1.0 / 4.0;
	double epsilon_n = epsilon * pow(n_samples, -gamma);
	MonteCarlo* mc = new MonteCarlo(model, quanto, rng, T / n_time_steps, n_samples, 0.0001);
	int M = 365;
	PnlMat* simulated_path = pnl_mat_create(M + 1, 2);
	int n_scenarios = 50;
	double mc_price = 0;
	double mc_ic = 0;
	model->simul_market(simulated_path, T, M, rng);
	int n_freqs = 4;
	const double* freqs_ptr = new double[n_freqs] {1, 5, 10, 30};
	PnlVect* freqs = pnl_vect_create_from_ptr(n_freqs, freqs_ptr);
	//histogram_erorrs_quanto(mc, model, rng, M, freqs, n_scenarios);
	//validate_price_quanto(simulated_path, model, mc, n_scenarios, rho);
	validate_delta_quanto(simulated_path, model, mc, n_scenarios, rho);

}

void validate_price_quanto(PnlMat* simulated_path, FXBlackScholes* model, MonteCarlo* mc, int n_scenarios, double rho) {
	std::ofstream myfile;
	Quanto* quanto = (Quanto*)mc->opt_;
	double T = quanto->T_;
	myfile.open("../Validation/price_quanto.txt");
	int count = 0;
	double t = 0, S_t = 0, Z_eur_t = 0, closed_price = 0, mc_price = 0, ic = 0;
	int n_time_steps = quanto->nbTimeSteps_;
	int M = simulated_path->m - 1;
	PnlMat* past = pnl_mat_create(1, 1);
	myfile << "Validation price" << std::endl;
	for (double i = 0.0; i <= double(M); i++) {
		t = T * i / (double)M;
		model->getPast(past, simulated_path, t, n_time_steps, T);
		S_t = MGET(simulated_path, int(i), 0);
		Z_eur_t = MGET(simulated_path, int(i), 1);
		closed_price = quanto->price(t, S_t, Z_eur_t, model->vect_r, model->sigma_, rho);
		std::cout << "closed price" << closed_price << std::endl;
		for (int k = 0; k < n_scenarios; k++) {
			mc->price(past, t, mc_price, ic);
			std::cout << mc_price << std::endl;
			if (mc_price - ic / 2 <= closed_price && closed_price <= mc_price + ic / 2)
			{
				//myfile << " in ";
				count++;
			}
			else
			{
				//myfile << " not in ";
			}
			//myfile << "   [" << prix_mc - ic / 2 << ", " << prix_mc + ic / 2 << "] " << " ic = " << ic << std::endl;
		}
		myfile << std::endl << count << " / " << n_scenarios << "sont dans l'intervalle de confiance" << std::endl;
		count = 0;
	}
	pnl_mat_free(&past);
	myfile.close();
}

void validate_delta_quanto(PnlMat* simulated_path, FXBlackScholes* model, MonteCarlo* mc, int n_scenarios, double rho) {
	std::ofstream myfile;
	Quanto* quanto = (Quanto*)mc->opt_;
	double T = quanto->T_;
	myfile.open("../Validation/delta_quanto.txt");
	int count = 0, count_zc = 0;
	double t = 0, S_t = 0, Z_eur_t = 0, closed_delta = 0, closed_delta_zc = 0;
	PnlVect* mc_deltas = pnl_vect_create(2);
	PnlVect* ic = pnl_vect_create(2);
	int n_time_steps = quanto->nbTimeSteps_;
	int M = simulated_path->m - 1;
	PnlMat* past = pnl_mat_create(1, 1);
	myfile << "Validation deltas" << std::endl;
	for (double i = 0.0; i <= double(M); i++) {
		t = T * i / (double)M;
		model->getPast(past, simulated_path, t, n_time_steps, T);
		S_t = MGET(simulated_path, int(i), 0);
		Z_eur_t = MGET(simulated_path, int(i), 1);
		closed_delta = quanto->delta(t, S_t, Z_eur_t, model->vect_r, model->sigma_, rho);
		//std::cout << "delta closed is: " << closed_delta << std::endl;
		closed_delta_zc = quanto->delta_zc(t, S_t, Z_eur_t, model->vect_r, model->sigma_, rho);
		//std::cout << "delta zc closed is: " << closed_delta_zc << std::endl;

		for (int k = 0; k < n_scenarios; k++) {
			mc->delta(past, t, mc_deltas, ic);
			//std::cout << "delta mc is: " << pnl_vect_get(mc_deltas, 0) << std::endl;
			//std::cout << "delta mc zc is " << pnl_vect_get(mc_deltas, 1) << std::endl;
			if (pnl_vect_get(mc_deltas, 0) - pnl_vect_get(ic, 0) / 2 <= closed_delta && closed_delta <= pnl_vect_get(mc_deltas, 0) + pnl_vect_get(ic, 0) / 2)
			{
				//myfile << " in ";
				count++;
			}

			if (pnl_vect_get(mc_deltas, 1) - pnl_vect_get(ic, 1) / 2 <= closed_delta_zc && closed_delta_zc <= pnl_vect_get(mc_deltas, 1) + pnl_vect_get(ic, 1) / 2)
			{
				//myfile << " in ";
				count_zc++;
			}
			else
			{
				//myfile << " not in ";
			}
			//myfile << "   [" << prix_mc - ic / 2 << ", " << prix_mc + ic / 2 << "] " << " ic = " << ic << std::endl;
			pnl_vect_set_all(mc_deltas, 0.0);
			pnl_vect_set_all(ic, 0);
		}
		myfile << std::endl << count << " / " << n_scenarios << "sont dans l'intervalle de confiance" << std::endl;
		myfile << std::endl << count_zc << " / " << n_scenarios << "sont dans l'intervalle de confiance Zéro Coupon" << std::endl;

		count = 0;
		count_zc = 0;
		pnl_vect_set_all(mc_deltas, 0.0);
		pnl_vect_set_all(ic, 0);
	}
	pnl_mat_free(&past);
	pnl_vect_free(&mc_deltas);
	pnl_vect_free(&ic);
	myfile.close();
}
void histogram_erorrs_quanto(MonteCarlo* mc, FXBlackScholes* model, PnlRng* rng, int M, PnlVect* freqs, int scenarios) {
	Quanto* quanto = (Quanto*)mc->opt_;
	PnlMat* past = pnl_mat_create(1, 2);
	int n_time_steps = quanto->nbTimeSteps_;
	double T = quanto->T_;
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
		filename = "../Validation/quanto_histogram_errors_M" + std::to_string(M) + "_freq_" + std::to_string((int)freq) + ".csv";
		myfile.open(filename);
		for (int i = 0; i < scenarios; i++) {
			model->simul_market(simulated_path, T, M, rng);
			hedge.PnLfreq(simulated_path, n_time_steps + 1, freq, portfolio_values, option_values, error); // amine
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