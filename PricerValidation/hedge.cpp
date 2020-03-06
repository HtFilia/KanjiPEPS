//
// Created by afrai on 9/19/19.
//



#include <iostream>
#include <string>



using namespace std;

int main(int argc, char **argv) {


	char *marketfile = argv[1];
	char *datafile = argv[2];


	PnlMat* path = 



	PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
	pnl_rng_sseed(rng, time(NULL));

	double mu = 0;
	for (int i = 1; i < path->m; ++i) {
		mu += log(MGET(path, i, 0)) - log(MGET(path, i - 1, 0));
	}

	mu /= (path->m - 1) * (data.T / path->m - 1);
	double sigma = pnl_vect_get(data.sigma, 0);
	mu += sigma * sigma*0.5;


	PnlVect *trend = pnl_vect_create_from_scalar(data.size, 0.0001);
	BlackScholesModel* model = new BlackScholesModel(data.size, data.r, data.corr, data.sigma, data.spot, trend);

	Option* option;
	if (data.type == "basket") {
		option = new BasketOption(data.T, data.nbTimeSteps, data.size, data.payOffCoeffs, data.strike);
	}
	else if (data.type == "asian") {
		option = new Asian(data.T, data.nbTimeSteps, data.size, data.payOffCoeffs, data.strike);
	}
	else if (data.type == "performance") {
		option = new PerformanceOption(data.T, data.nbTimeSteps, data.size, data.payOffCoeffs);
	}


	MonteCarlo* mc = new MonteCarlo(model, option, rng, 0.1, data.n_samples);
	Hedge *hedge = new Hedge(mc);



	PnlVect* portfolio_values = pnl_vect_create(path->m);
	PnlVect* option_prices = pnl_vect_create(path->m);
	double error;
	double N = data.nbTimeSteps + 1;
	//model->asset(path, data.T, data.hedging_dates_number, rng);
	//pnl_mat_resize(path ,path->m, 2);
	//model->simul_market(path, data.T, data.hedging_dates_number, rng);
	pnl_mat_print(path);
	std::cout << path->m << std::endl;
	std::cout << data.hedging_dates_number << std::endl;
	//hedge->PnL(path, data.nbTimeSteps, 365, portfolio_values, option_prices, error); // refactored
//    hedge->PnL(path, N, portfolio_values, option_prices, error);
	hedge->PnLfreq(path, N, 22, portfolio_values, option_prices, error);

	HedgingResults res(GET(option_prices, 0), 0, error); //Implementer ic dans Hedge !!!!!!!!!!!
	std::cout << res << std::endl;
	PnlMat* comp = pnl_mat_create(option_prices->size, 2);
	pnl_mat_set_col(comp, option_prices, 0);
	pnl_mat_set_col(comp, portfolio_values, 1);
	pnl_mat_print(comp);
	std::cout << option_prices->size << std::endl;

}
