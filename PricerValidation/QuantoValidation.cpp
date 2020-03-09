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
	model->simul_market(simulated_path, T, M, rng);

	//for (int i = 0; i < n_scenarios; i++) {
	//	//model->simul_market(simulated_path, T, M, rng);
	//	//pnl_mat_print(simulated_path);
	//	double prix0 = quanto->price(0, 100, 0.89, r, sigmas, rho);
	//	std::cout << "closed price at t = 0 is: " << prix0 << std::endl;
	//	mc->price(mc_price, mc_ic);
	//	std::cout << "mc price at t = 0 is: " << mc_price << std::endl;
	//	std::cout << "ic at t = 0 is " << mc_ic << std::endl;
	//}
	//std::cout << "hi" << std::endl;

	validate_price_quanto(simulated_path, model, mc, n_scenarios, rho);
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
		for (int k = 0; k < n_scenarios; k++) {
			mc->price(past, t, mc_price, ic);
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
