#include "Wrapper.h"
#include <iostream>
using namespace Computations;
namespace Wrapper {
	void WrapperClass::getPriceCallEuro(double T, double S0, double K, double sigma, double r) {
		double ic, px;
		calleuro(ic, px, sampleNb, T, S0, K, sigma, r);
		this->confidenceInterval = ic;
		this->price = px;
	}

	void WrapperClass::getPriceCallMC(double T, double S0, double K, double sigma, double r) {
		double ic, px;
		callMC(ic, px, sampleNb, T, S0, K, sigma, r);
		this->confidenceInterval = ic;
		this->price = px;
	}

	void WrapperClass::getPriceDeltaPerf(double T, array<double, 1> ^spots, array<double, 1> ^sigmas, double correlation, double r) {
		double ic = 0;
		double px = 0;
		array<double, 1>^ ic_delta = gcnew array<double, 1>(3);
		array<double, 1> ^ delta = gcnew array<double, 1>(3);;
		pin_ptr<double> delta_ptr = &delta[0];
		pin_ptr<double> ic_delta_ptr = &ic_delta[0];


		pin_ptr<double> ptr_S0 = &spots[0];
		pin_ptr<double> ptr_sigma = &sigmas[0];

		performance_price_hedge(ic, px, ic_delta_ptr, delta_ptr, sampleNb, T, ptr_S0, ptr_sigma, correlation, r);
		this->confidenceInterval = ic;
		this->price = px;
		this->deltas = delta;
		this->ic_deltas = ic_delta;

	}
	void WrapperClass::getPriceDeltaPerft(double T, double t, array<double, 1> ^path, double nb_dates, array<double, 1> ^sigmas, array<double, 1 > ^correlation, double r) {
		double ic, px;
		array<double, 1>^ ic_delta = gcnew array<double, 1>(3);
		array<double, 1> ^ delta = gcnew array<double, 1>(3);
		pin_ptr<double> delta_ptr = &delta[0];
		pin_ptr<double> ic_delta_ptr = &ic_delta[0];
		pin_ptr<double> ptr_path = &path[0];
		pin_ptr<double> ptr_sigma = &sigmas[0];
		pin_ptr<double> ptr_corr = &correlation[0];
		performance_price_hedge_t(ic, px, ic_delta_ptr, delta_ptr, sampleNb, T, t, ptr_path, nb_dates, ptr_sigma, ptr_corr, r);
		this->confidenceInterval = ic;
		this->price = px;
		this->deltas = delta;
		this->ic_deltas = ic_delta;
	}

	void WrapperClass::SimulMarket(double t, double maturity, int nbHedging_dates, array<double, 1> ^spots, array<double, 1> ^trends, array<double, 1> ^sigmas, array<double, 1> ^correlation, double r) {
		int path_size = (int)(t * nbHedging_dates) / maturity;
		array<double, 1>^ path = gcnew array<double, 1>(3 * path_size);
		pin_ptr<double> ptr_path = &path[0];
		pin_ptr<double> ptr_S0 = &spots[0];
		pin_ptr<double> ptr_sigma = &sigmas[0];
		pin_ptr<double> ptr_trend = &trends[0];
		pin_ptr<double> ptr_corr = &correlation[0];
		simul_market(ptr_path, t, maturity, nbHedging_dates, ptr_S0, ptr_trend, ptr_sigma, ptr_corr, r);
		this->path = path;
	}
}
