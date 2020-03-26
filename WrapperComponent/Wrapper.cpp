#include "pch.h"

#include "Wrapper.h"

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

	void WrapperClass::getDeltaPerft(double netAssetValue, double T, double t, array<double, 1> ^past, array<double, 1> ^initial_values, int nb_dates, array<double, 1> ^sigmas, array<double, 1 > ^correlation, double r) {
		array<double, 1>^ ic_delta = gcnew array<double, 1>(3);
		array<double, 1> ^ delta = gcnew array<double, 1>(3);
		pin_ptr<double> delta_ptr = &delta[0];
		pin_ptr<double> ic_delta_ptr = &ic_delta[0];
		pin_ptr<double> ptr_past = &past[0];
		pin_ptr<double> ptr_sigma = &sigmas[0];
		pin_ptr<double> ptr_corr = &correlation[0];
		pin_ptr<double> ptr_inital_values = &initial_values[0];
		performance_delta_t(netAssetValue, ic_delta_ptr, delta_ptr, sampleNb, T, t, ptr_past, ptr_inital_values, nb_dates, ptr_sigma, ptr_corr, r);
		this->deltas = delta;
		this->ic_deltas = ic_delta;
	}

	void WrapperClass::getPricePerft(double netAssetValue, double T, double t, array<double, 1> ^past, array<double, 1> ^initial_values, int nb_dates, array<double, 1> ^sigmas, array<double, 1 > ^correlation, double r) {
		double ic, px;
		pin_ptr<double> ptr_past = &past[0];
		pin_ptr<double> ptr_sigma = &sigmas[0];
		pin_ptr<double> ptr_corr = &correlation[0];
		pin_ptr<double> ptr_inital_values = &initial_values[0];
		performance_price_t(netAssetValue, ic, px, sampleNb, T, t, ptr_past, ptr_inital_values, nb_dates, ptr_sigma, ptr_corr, r);
		this->confidenceInterval = ic;
		this->price = px;
	}

	void WrapperClass::getPriceDeltaPerft(double netAssetValue, double T, double t, array<double, 1> ^past, array<double, 1> ^initial_values, int nb_dates, array<double, 1> ^sigmas, array<double, 1> ^correlation, double r) {
		double ic, px;
		array<double, 1>^ ic_delta = gcnew array<double, 1>(3);
		array<double, 1> ^ delta = gcnew array<double, 1>(3);
		pin_ptr<double> delta_ptr = &delta[0];
		pin_ptr<double> ic_delta_ptr = &ic_delta[0];
		pin_ptr<double> ptr_past = &past[0];
		pin_ptr<double> ptr_sigma = &sigmas[0];
		pin_ptr<double> ptr_corr = &correlation[0];
		pin_ptr<double> ptr_inital_values = &initial_values[0];
		performance_price_t(netAssetValue, ic, px, sampleNb, T, t, ptr_past, ptr_inital_values, nb_dates, ptr_sigma, ptr_corr, r);
		performance_delta_t(netAssetValue, ic_delta_ptr, delta_ptr, sampleNb, T, t, ptr_past, ptr_inital_values, nb_dates, ptr_sigma, ptr_corr, r);
		this->confidenceInterval = ic;
		this->price = px;
		this->deltas = delta;
		this->ic_deltas = ic_delta;
	}

	void WrapperClass::SimulMarket(double t, double maturity, int nb_dates, array<double, 1> ^spots, array<double, 1> ^trends, array<double, 1> ^sigmas, array<double, 1> ^correlation, double r) {
		array<double, 1>^ path = gcnew array<double, 1>(3 * nb_dates);
		pin_ptr<double> ptr_path = &path[0];
		pin_ptr<double> ptr_S0 = &spots[0];
		pin_ptr<double> ptr_sigma = &sigmas[0];
		pin_ptr<double> ptr_trend = &trends[0];
		pin_ptr<double> ptr_corr = &correlation[0];
		simul_market(ptr_path, t, maturity, nb_dates, ptr_S0, ptr_trend, ptr_sigma, ptr_corr, r);
		this->path = path;
	}

	void WrapperClass::getPricePerft_fx(double netAssetValue, double T, double t, array<double, 1>^ past, array<double, 1>^ initial_values, int nb_dates, array<double, 1>^ sigmas, array<double, 1 >^ correlation, double r, double r_US, double r_HK) {
		double ic, px;
		pin_ptr<double> ptr_past = &past[0];
		pin_ptr<double> ptr_sigma = &sigmas[0];
		pin_ptr<double> ptr_corr = &correlation[0];
		pin_ptr<double> ptr_inital_values = &initial_values[0];
		performance_price_t_fx(netAssetValue, ic, px, sampleNb, T, t, ptr_past, ptr_inital_values, nb_dates, ptr_sigma, ptr_corr, r, r_US, r_HK);
		this->ic_fx = ic;
		this->price_fx = px;
	}


	void WrapperClass::getDeltaPerft_fx(double netAssetValue, double T, double t, array<double, 1> ^past, array<double, 1> ^initial_values, int nb_dates, array<double, 1> ^sigmas, array<double, 1 > ^correlation, double r, double r_US, double r_HK) {
		array<double, 1>^ ic_delta = gcnew array<double, 1>(5);
		array<double, 1> ^ delta = gcnew array<double, 1>(5);
		pin_ptr<double> delta_ptr = &delta[0];
		pin_ptr<double> ic_delta_ptr = &ic_delta[0];
		pin_ptr<double> ptr_past = &past[0];
		pin_ptr<double> ptr_sigma = &sigmas[0];
		pin_ptr<double> ptr_corr = &correlation[0];
		pin_ptr<double> ptr_inital_values = &initial_values[0];
		performance_delta_t_fx(netAssetValue, ic_delta_ptr, delta_ptr, sampleNb, T, t, ptr_past, ptr_inital_values, nb_dates, ptr_sigma, ptr_corr, r, r_US, r_HK);
		this->deltas_fx = delta;
		this->ic_deltas_fx = ic_delta;
	}
	void WrapperClass::getPriceDeltaPerft_fx(double netAssetValue, double T, double t, array<double, 1>^ past, array<double, 1>^ initial_values, int nb_dates, array<double, 1>^ sigmas, array<double, 1>^ correlation, double r, double r_US, double r_HK) {
		double ic, px;
		array<double, 1>^ ic_delta = gcnew array<double, 1>(5);
		array<double, 1>^ delta = gcnew array<double, 1>(5);
		pin_ptr<double> delta_ptr = &delta[0];
		pin_ptr<double> ic_delta_ptr = &ic_delta[0];
		pin_ptr<double> ptr_past = &past[0];
		pin_ptr<double> ptr_sigma = &sigmas[0];
		pin_ptr<double> ptr_corr = &correlation[0];
		pin_ptr<double> ptr_inital_values = &initial_values[0];
		performance_price_t_fx(netAssetValue, ic, px, sampleNb, T, t, ptr_past, ptr_inital_values, nb_dates, ptr_sigma, ptr_corr, r, r_US, r_HK);
		performance_delta_t_fx(netAssetValue, ic_delta_ptr, delta_ptr, sampleNb, T, t, ptr_past, ptr_inital_values, nb_dates, ptr_sigma, ptr_corr, r, r_US, r_HK);
		this->ic_fx = ic;
		this->price_fx = px;
		this->deltas_fx = delta;
		this->ic_deltas_fx = ic_delta;
	}

	void WrapperClass::SimulMarket(double t, double maturity, int nb_dates, array<double, 1>^ spots, array<double, 1>^ trends, array<double, 1>^ sigmas, array<double, 1>^ correlation, double r, double r_US, double r_HK) {
		array<double, 1>^ path = gcnew array<double, 1>(5 * nb_dates);
		pin_ptr<double> ptr_path = &path[0];
		pin_ptr<double> ptr_S0 = &spots[0];
		pin_ptr<double> ptr_sigma = &sigmas[0];
		pin_ptr<double> ptr_trend = &trends[0];
		pin_ptr<double> ptr_corr = &correlation[0];
		simul_market_fx(ptr_path, t, maturity, nb_dates, ptr_S0, ptr_trend, ptr_sigma, ptr_corr, r, r_US, r_HK);
		this->path_fx = path;
	}


	double WrapperClass::getPrice(bool fx) {
		if (fx) {
			return price_fx;
		}
		return price;
	}

	double WrapperClass::getIC(bool fx) {
		if (fx) {
			return ic_fx;
		}
		return confidenceInterval;
	}
	array<double, 1>^ WrapperClass::getDeltas(bool fx) {
		if (fx) {
			return deltas_fx;
		}
		return deltas;
	}
	array<double, 1>^ WrapperClass::getICDeltas(bool fx) {
		if (fx) {
			return ic_deltas_fx;
		}
		return ic_deltas;
	}

	array<double, 1>^ WrapperClass::getPath(bool fx) {
		if (fx) {
			return path_fx;
		}
		return path;
	}


	void WrapperClass::ComputePrice(double netAssetValue, double T, double t, array<double, 1>^ past, array<double, 1>^ initial_values, int nb_dates, array<double, 1>^ sigmas, array<double, 1 >^ correlation, double r, double r_US, double r_HK, bool fx) {
		if (fx) {
			getPricePerft_fx(netAssetValue, T, t, past, initial_values, nb_dates, sigmas, correlation, r, r_US, r_HK);
		}
		else
		{
			getPricePerft(netAssetValue, T, t, past, initial_values, nb_dates, sigmas, correlation, r);
		}
	}
	void WrapperClass::ComputeDelta(double netAssetValue, double T, double t, array<double, 1>^ past, array<double, 1>^ initial_values, int nb_dates, array<double, 1>^ sigmas, array<double, 1 >^ correlation, double r, double r_US, double r_HK, bool fx) {
		if (fx) {
			getDeltaPerft_fx(netAssetValue, T, t, past, initial_values, nb_dates, sigmas, correlation, r, r_US, r_HK);
		}
		else
		{
			getDeltaPerft(netAssetValue, T, t, past, initial_values, nb_dates, sigmas, correlation, r);
		}

	}
	void WrapperClass::ComputePriceDelta(double netAssetValue, double T, double t, array<double, 1>^ past, array<double, 1>^ initial_values, int nb_dates, array<double, 1>^ sigmas, array<double, 1>^ correlation, double r, double r_US, double r_HK, bool fx) {
		if (fx) {
			getPriceDeltaPerft_fx(netAssetValue, T, t, past, initial_values, nb_dates, sigmas, correlation, r, r_US, r_HK);
		}
		else
		{
			getPriceDeltaPerft(netAssetValue, T, t, past, initial_values, nb_dates, sigmas, correlation, r);
		}

	}
	void WrapperClass::SimulMarket(double t, double maturity, int nb_dates, array<double, 1>^ spots, array<double, 1>^ trends, array<double, 1>^ sigmas, array<double, 1>^ correlation, double r, double r_US, double r_HK, bool fx) {
		if (fx) {
			SimulMarket( t,  maturity, nb_dates, spots, trends, sigmas, correlation, r, r_US, r_HK);
		}
		else
		{
			SimulMarket(t, maturity, nb_dates, spots, trends, sigmas, correlation, r);
		}

	}

}