#include "QuantoValidation.hpp"

void validate_quanto(PnlRng* rng) {
	int n_time_steps = 1;
	double T = 1;
	int size = 2;

	const double *list_r = new double[size] {0.02, 0.05};
	PnlVect* r = pnl_vect_create_from_ptr(size, list_r);
	const double *list_sigmas = new double[size]{ 0.2, 0.01};
	PnlVect* sigmas = pnl_vect_create_from_ptr(size, list_sigmas);
	double rho = 0.2;
	PnlMat* corr = pnl_mat_create_from_scalar(size, size, rho);
	for (int diag = 0; diag < size; diag++)
		pnl_mat_set(corr, diag, diag, 1);
	const double* list_spots = new double[size]{ 100, 0.89 };
	PnlVect* spots = pnl_vect_create_from_ptr(size, list_spots);

	const double* list_trends = new double[size]{0.02, -0.03};
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
	for (int i = 0; i < n_scenarios; i++) {
		//model->simul_market(simulated_path, T, M, rng);
		//pnl_mat_print(simulated_path);
		double prix0 = quanto->price(0, 100, 0.89, r, sigmas, rho);
		std::cout << "closed price at t = 0 is: " << prix0 << std::endl;
		mc->price(mc_price, mc_ic);
		std::cout << "mc prie at t = 0 is: " << mc_price << std::endl;
		std::cout << "ic at t = 0 is " << mc_ic << std::endl;
	}
	std::cout << "hi" << std::endl;

	//validate_price_quanto(simulated_path, model, mc, rng);
}