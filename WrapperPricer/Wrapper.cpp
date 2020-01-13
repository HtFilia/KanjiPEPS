#include "Wrapper.h"

using namespace Computations;
namespace Wrapper {
	void WrapperClass::getPriceCallEuro(int sampleNb, double T, double S0, double K, double sigma, double r) {
		double ic, px;
		calleuro(ic, px, sampleNb, T, S0, K, sigma, r);
		this->confidenceInterval = ic;
		this->price = px;
	}

	void WrapperClass::getPriceCallMC(int sampleNb, double T, double S0, double K, double sigma, double r) {
		double ic, px;
		callMC(ic, px, sampleNb, T, S0, K, sigma, r);
		this->confidenceInterval = ic;
		this->price = px;
	}

	void WrapperClass::getPriceDeltaPerf(int sampleNb, double T, double S0[], double K, double sigma[], double correlation, double r) {
		double ic, px;
		double ic_delta[3];
		double delta[3];
		performance_price_hedge(ic, px, ic_delta, delta, sampleNb, T, S0, K, sigma, correlation, r);
		this->confidenceInterval = ic;
		this->price = px;
		for (int i = 0; i < 3; i++)
		{
			deltas[i] = delta[i];
		}

	}


}