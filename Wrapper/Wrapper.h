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
		// gestion des tableaux manags a revoir

	public:
		WrapperClass() { confidenceInterval = price = 0; };
		void getPriceCallEuro(int sampleNb, double T, double S0, double K, double sigma, double r);
		void getPriceCallMC(int sampleNb, double T, double S0, double K, double sigma, double r);
		void getPriceDeltaPerf(int sampleNb, double T, array<double, 1> ^spots, array<double, 1> ^sigmas, double correlation, double r);
		void getPriceDeltaPerft(int sampleNb, double T, double t, array<double, 1> ^path, double nb_dates, array<double, 1> ^sigmas, array<double, 1> ^correlation, double r);
		void SimulMarket(double t, double maturity, int nbHedging_dates, array<double, 1> ^spots, array<double, 1> ^trends, array<double, 1> ^sigmas, double correlation, double r);
		double getPrice() { return price; };
		double getIC() { return confidenceInterval; };
		array<double, 1> ^ getDeltas() { return deltas; };
		array<double, 1> ^ getICDeltas() { return ic_deltas; };
		array<double, 1> ^ getPath() { return path; };


	};
}
