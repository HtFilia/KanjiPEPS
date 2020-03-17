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
		int sampleNb = 5000;
		// gestion des tableaux manags a revoir

	public:
		WrapperClass() { confidenceInterval = price = 0; };
		void getPriceCallEuro(double T, double S0, double K, double sigma, double r);
		void getPriceCallMC(double T, double S0, double K, double sigma, double r);
		void getDeltaPerft(double netAssetValue, double T, double t, array<double, 1>^ past, array<double, 1> ^initial_values, double nb_dates, array<double, 1>^ sigmas, array<double, 1>^ correlation, double r);
		void getPricePerft(double netAssetValue, double T, double t, array<double, 1>^ past, array<double, 1> ^initial_values, double nb_dates, array<double, 1>^ sigmas, array<double, 1>^ correlation, double r);
		void getPriceDeltaPerft(double netAssetValue, double T, double t, array<double, 1> ^past, array<double, 1> ^initial_values, double nb_dates, array<double, 1> ^sigmas, array<double, 1> ^correlation, double r);
		void SimulMarket(double t, double maturity, int nbHedging_dates, array<double, 1> ^spots, array<double, 1> ^trends, array<double, 1> ^sigmas, array<double, 1> ^correlation, double r);
		double getPrice() { return price; };
		double getIC() { return confidenceInterval; };
		array<double, 1> ^ getDeltas() { return deltas; };
		array<double, 1> ^ getICDeltas() { return ic_deltas; };
		array<double, 1> ^ getPath() { return path; };


	};
}
