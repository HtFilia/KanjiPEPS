#include "CallValidation.hpp"

void validate_call(PnlRng* rng) {

	// initializing the model and the call option
	double r = 0.04879;
	double spot_ = 100;
	double sigma_ = 0.2;
	PnlVect *spot = pnl_vect_create_from_scalar(1, spot_);
	PnlVect *sigma = pnl_vect_create_from_scalar(1, sigma_);
	PnlVect *trend = pnl_vect_create_from_scalar(1, r);
	double T = 0.1;
	double strike = 100;
	BlackScholesModel* model = new BlackScholesModel(1, r, 0, sigma, spot, trend);
	int n_time_steps = 1;
	VanillaCall* call = new VanillaCall(T, n_time_steps, 1, strike);

	// initializing the montecarlo tool
	int n_samples = 50000;
	double epsilon = 0.00001;
	double gamma = -1.0 / 4.0;
	double epsilon_n = epsilon * pow(n_samples, -gamma);
	MonteCarlo* mc = new MonteCarlo(model, call, rng, T / n_time_steps, n_samples, epsilon_n);
	int M = 64;
	int H = M;
	int n_scenarios = 100;
	PnlMat* simulated_path = pnl_mat_create(1, 1);
	model->simul_market(simulated_path, T, M, rng);
	//validate_price_call(simulated_path, mc, model);
	//validate_delta_call(simulated_path, mc, model);
	validate_hedging_frequency_call(mc, model, rng, M);
	//validate_mean_error_call(mc, model, rng, M, n_scenarios);
	//histogram_errors_call(mc, model, rng, M, H, n_scenarios);
	pnl_mat_free(&simulated_path);

}


void validate_price_call(PnlMat* simulated_path, MonteCarlo* mc, BlackScholesModel* model) {
	std::ofstream myfile;
	double r = model->r_;
	VanillaCall* call = (VanillaCall*)mc->opt_;
	double sigma_ = pnl_vect_get(model->sigma_,0);
	PnlMat* past = pnl_mat_create(1, 1);
	double strike = call->strike_;
	int n_time_steps = call->nbTimeSteps_;
	double T = call->T_;
	myfile.open("../Validation/price_vanilla.txt");
	double spot_at_t = 0, prix_mc = 0, ic = 0, prix = 0, t = 0;
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
	pnl_mat_free(&past);
	myfile.close();
}

void validate_delta_call(PnlMat* simulated_path, MonteCarlo* mc, BlackScholesModel* model) {
	PnlVect* delta = pnl_vect_create(1);
	PnlVect* ic_delta = pnl_vect_create(1);
	double r = model->r_;
	VanillaCall* call = (VanillaCall*)mc->opt_;
	double sigma_ = pnl_vect_get(model->sigma_, 0);
	PnlMat* past = pnl_mat_create(1, 1);
	double strike = call->strike_;
	int n_time_steps = call->nbTimeSteps_;
	double T = call->T_;
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
	for (double i = 0.0; i < (double)M; i++)
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
	pnl_vect_free(&ic_delta);
	pnl_vect_free(&delta);
	pnl_mat_free(&past);
	myfile.close();
}



void validate_hedging_frequency_call(MonteCarlo* mc, BlackScholesModel* model, PnlRng* rng, int M) {
	double r = model->r_;
	VanillaCall* call = (VanillaCall*)mc->opt_;
	double sigma_ = pnl_vect_get(model->sigma_, 0);
	PnlMat* past = pnl_mat_create(1, 1);
	double strike = call->strike_;
	int n_time_steps = call->nbTimeSteps_;
	double T = call->T_;
	PnlMat* simulated_path = pnl_mat_create(1, 1);
	model->simul_market(simulated_path, T, M, rng);
	Hedge hedge(mc);
	double error = 0;
	std::ofstream myfile;
	myfile.open("../Validation/variation_frequence_call.txt");
	PnlVect* option_values = pnl_vect_create(1);
	PnlVect* portfolio_values = pnl_vect_create(1);
	for (int H = M; H >= 2; H /= 2) {
		hedge.PnL(simulated_path, n_time_steps, H, portfolio_values, option_values, error);
		myfile << H << ";" << error << std::endl;
	}
	myfile.close();
	pnl_mat_free(&simulated_path);
	pnl_mat_free(&past);
	pnl_vect_free(&option_values);
	pnl_vect_free(&portfolio_values);
}


void validate_mean_error_call(MonteCarlo* mc, BlackScholesModel* model, PnlRng *rng, int M, int n_scenarios) {
	std::ofstream myfile;
	myfile.open("../Validation/mean_error_call.txt");
	double r = model->r_;
	VanillaCall* call = (VanillaCall*)mc->opt_;
	double strike = call->strike_;
	int n_time_steps = call->nbTimeSteps_;
	double T = call->T_;
	PnlMat* simulated_path = pnl_mat_create(1, 1);
	model->simul_market(simulated_path, T, M, rng);
	PnlVect* option_values = pnl_vect_create(1);
	PnlVect* portfolio_values = pnl_vect_create(1);
	Hedge hedge(mc);
	double mean_error = 0, error = 0;
	myfile << " Calcul de l'erreur moyenne sur la couverture d'un produit kanji de maturité " << T << " ans sur des indices de spot initials " << pnl_vect_get(model->spot_,0) << std::endl;
	double prix = 0, ic_prix = 0;
	for (int H = M; H >= H/4; H/=2)
	{
		myfile << H << ";";
		for (int i = 0; i < n_scenarios; i++) {
			pnl_vect_resize(portfolio_values, H + 1);
			pnl_vect_resize(option_values, H + 1);
			mc->mod_->simul_market(simulated_path, T, M, rng);
			//mc->profitAndLoss(simulated_path, prix, ic_prix, error);
			pnl_mat_print(simulated_path);
			//hedge.PnL(simulated_path, n_time_steps, H, portfolio_values, option_values, error);
			hedge.PnL(simulated_path, n_time_steps+1, portfolio_values, option_values, error, ic_prix);
			PnlMat* comparaison = pnl_mat_create(H + 1, 2);
			pnl_mat_set_col(comparaison, portfolio_values, 0);
			pnl_mat_set_col(comparaison, option_values, 1);
			pnl_mat_print(comparaison);
			myfile << error << ";";
			//myfile << " Erreur sur le scénario " << i << " : " << error << std::endl;
			mean_error += error;
		}
		myfile << std::endl;
	}
	mean_error = mean_error / n_scenarios;
	myfile << "Erreur moyenne sur " << n_scenarios << " scénarios : " << mean_error;
	myfile.close();
	pnl_vect_free(&portfolio_values);
	pnl_vect_free(&portfolio_values);
	pnl_mat_free(&simulated_path);
}


void histogram_errors_call(MonteCarlo* mc, BlackScholesModel* model, PnlRng* rng, int M, int H, int scenarios) {
	double r = model->r_;
	VanillaCall* call = (VanillaCall*)mc->opt_;
	double sigma_ = pnl_vect_get(model->sigma_, 0);
	PnlMat* past = pnl_mat_create(1, 1);
	double strike = call->strike_;
	int n_time_steps = call->nbTimeSteps_;
	double T = call->T_;
	PnlMat* simulated_path = pnl_mat_create(1, 1);
	model->simul_market(simulated_path, T, M, rng);
	Hedge hedge(mc);
	double error = 0;
	std::ofstream myfile;
	myfile.open("../Validation/histogram_errors.csv");
	PnlVect* option_values = pnl_vect_create(1);
	PnlVect* portfolio_values = pnl_vect_create(1);
	for (int i = 0; i < scenarios; i++) {
		model->simul_market(simulated_path, T, M, rng);
		hedge.PnL(simulated_path, n_time_steps, H, portfolio_values, option_values, error);
		myfile << error << ";"<< std::endl;
	}
	myfile.close();
	pnl_mat_free(&simulated_path);
	pnl_mat_free(&past);
	pnl_vect_free(&option_values);
	pnl_vect_free(&portfolio_values);
}
