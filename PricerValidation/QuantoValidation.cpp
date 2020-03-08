#include "QuantoValidation.hpp"

void validate_quanto(PnlRng* rng) {
	int n_time_steps = 1;
	double T = 1;
	int size = 2;

	const double *list_r = new double[size] {0.02, 0.05};
	PnlVect* r = pnl_vect_create_from_ptr(size, list_r);
	const double *list_sigmas = new double[5]{ 0.16, 0.15, 0.2, 0.02, 0.2 };
	PnlVect* sigmas = pnl_vect_create_from_ptr(5, list_sigmas);
	double rho = 0.2;
	PnlMat* corr = pnl_mat_create_from_scalar(5, 5, rho);
	for (int diag = 0; diag < 5; diag++)
		pnl_mat_set(corr, diag, diag, 1);
	const double* list_spots = new double[2]{ 100, 0.89 };
	PnlVect* spots = pnl_vect_create_from_ptr(2, list_spots);

	const double* list_trends = new double[5]{ 0.02, -0.03, 0.05, -0.08,  0.1};
	PnlVect* trends = pnl_vect_create_from_ptr(5, list_trends);
	Quanto* quanto = new Quanto(T, n_time_steps, size, 100);
	KanjiBlackScholes* model = new KanjiBlackScholes(2, r, corr, sigmas, spots, trends);
	int n_samples = 50000;
	double epsilon = 0.000001;
	double gamma = -1.0 / 4.0;
	double epsilon_n = epsilon * pow(n_samples, -gamma);
	//MonteCarlo* mc = new MonteCarlo(model, quanto, rng, T / n_time_steps, n_samples, 0.0001);
	int M = 365;
	PnlMat* simulated_path = pnl_mat_create(M + 1, 2);
	int n_scenarios = 50;
	for (int i = 0; i < n_scenarios; i++) {
		model->simul_market(simulated_path, T, M, rng);
		pnl_mat_print(simulated_path);
		double prix0 = quanto->price(0, 100, 0.89, r, sigmas, rho);
		std::cout << "price at t = 0 is: " << prix0 << std::endl;
	}
	std::cout << "hi" << std::endl;

	//validate_price_quanto(simulated_path, model, mc, rng);
}