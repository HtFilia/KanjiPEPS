#pragma once
#include "Computations.hpp"
using namespace System;

namespace Wrapper {

	public ref class WrapperClass
	{
	private:
		double confidenceInterval;
		double price;
		// gestion des tableaux managés a revoir
		
	public:
		WrapperClass() { confidenceInterval = price = 0; };
		array<double, 1> ^deltas = gcnew array<double, 1>(3); // a revoir (private)
		void getPriceCallEuro(int sampleNb, double T, double S0, double K, double sigma, double r);
		void getPriceCallMC(int sampleNb, double T, double S0, double K, double sigma, double r);
		void getPriceDeltaPerf(int sampleNb, double T, double S0[], double K, double sigma[], double correlation, double r);
		double getPrice() { return price; };
		double getIC() { return confidenceInterval; };
	};
}