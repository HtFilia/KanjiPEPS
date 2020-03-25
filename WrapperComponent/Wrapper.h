#pragma once

#include "Computations.hpp"
using namespace System;

namespace Wrapper {

	public ref class WrapperClass
	{
	private:
		double confidenceInterval;
		double price;
		array<double, 1> ^deltas;
		array<double, 1> ^ic_deltas;
		array<double, 1> ^path;
		double price_fx;
		double ic_fx;
		array<double, 1>^ deltas_fx;
		array<double, 1>^ ic_deltas_fx;
		array<double, 1>^ path_fx;

		int sampleNb = 1000;
		// gestion des tableaux manags a revoir

	public:
		WrapperClass() { confidenceInterval = price = price_fx = ic_fx = 0; };
		void getPriceCallEuro(double T, double S0, double K, double sigma, double r);
		void getPriceCallMC(double T, double S0, double K, double sigma, double r);
		void getDeltaPerft(double netAssetValue, double T, double t, array<double, 1>^ past, array<double, 1> ^initial_values, int nb_dates, array<double, 1>^ sigmas, array<double, 1>^ correlation, double r);
		void getPricePerft(double netAssetValue, double T, double t, array<double, 1>^ past, array<double, 1> ^initial_values, int nb_dates, array<double, 1>^ sigmas, array<double, 1>^ correlation, double r);
		void getPriceDeltaPerft(double netAssetValue, double T, double t, array<double, 1> ^past, array<double, 1> ^initial_values, int nb_dates, array<double, 1> ^sigmas, array<double, 1> ^correlation, double r);
		void SimulMarket(double t, double maturity, int nb_dates, array<double, 1> ^spots, array<double, 1> ^trends, array<double, 1> ^sigmas, array<double, 1> ^correlation, double r);
		double getPrice() { return price; };
		double getIC() { return confidenceInterval; };
		array<double, 1> ^ getDeltas() { return deltas; };
		array<double, 1> ^ getICDeltas() { return ic_deltas; };
		array<double, 1> ^ getPath() { return path; };
		void getPricePerft_fx(double netAssetValue, double T, double t, array<double, 1>^ past, array<double, 1>^ initial_values, int nb_dates, array<double, 1>^ sigmas, array<double, 1 >^ correlation, double r, double r_US, double r_HK);
		void getDeltaPerft_fx(double netAssetValue, double T, double t, array<double, 1>^ past, array<double, 1>^ initial_values, int nb_dates, array<double, 1>^ sigmas, array<double, 1 >^ correlation, double r, double r_US, double r_HK);
		void getPriceDeltaPerft_fx(double netAssetValue, double T, double t, array<double, 1>^ past, array<double, 1>^ initial_values, int nb_dates, array<double, 1>^ sigmas, array<double, 1>^ correlation, double r, double r_US, double r_HK);
		void SimulMarket(double t, double maturity, int nb_dates, array<double, 1>^ spots, array<double, 1>^ trends, array<double, 1>^ sigmas, array<double, 1>^ correlation, double r, double r_US, double r_HK);

		void ComputePrice(double netAssetValue, double T, double t, array<double, 1>^ past, array<double, 1>^ initial_values, int nb_dates, array<double, 1>^ sigmas, array<double, 1 >^ correlation, double r, double r_US, double r_HK, bool fx);
		void ComputeDelta(double netAssetValue, double T, double t, array<double, 1>^ past, array<double, 1>^ initial_values, int nb_dates, array<double, 1>^ sigmas, array<double, 1 >^ correlation, double r, double r_US, double r_HK, bool fx);
		void ComputePriceDelta(double netAssetValue, double T, double t, array<double, 1>^ past, array<double, 1>^ initial_values, int nb_dates, array<double, 1>^ sigmas, array<double, 1>^ correlation, double r, double r_US, double r_HK, bool fx);
		void SimulMarket(double t, double maturity, int nb_dates, array<double, 1>^ spots, array<double, 1>^ trends, array<double, 1>^ sigmas, array<double, 1>^ correlation, double r, double r_US, double r_HK, bool fx);

		double getPrice_fx() { return price_fx; };
		double getIC_fx() { return ic_fx; };
		array<double, 1>^ getDeltas_fx() { return deltas_fx; };
		array<double, 1>^ getICDeltas_fx() { return ic_deltas_fx; };
		array<double, 1>^ getPath_fx() { return path_fx; };

		double getPrice(bool fx);
		double getIC(bool fx);
		array<double, 1>^ getDeltas(bool fx);
		array<double, 1>^ getICDeltas(bool fx) ;
		array<double, 1>^ getPath(bool fx) ;

	};
}