#pragma once
#include "KanjiOption.hpp"
class KanjiOptionFX :
	public KanjiOption
{
public:
	double rUSD_;
	double rHKD_;

	~KanjiOptionFX();

	KanjiOptionFX(double T, int nbTimeSteps, int size, PnlVect* values, double netAssetValue, double rUSD, double rHKD):KanjiOption(T,nbTimeSteps, size, values, netAssetValue) {
		rUSD_ = rUSD;
		rHKD_ = rHKD;
	}

	double payoff(const PnlMat* path);
};

