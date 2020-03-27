#pragma once
#include "BlackScholesModel.hpp"
class FXBlackScholes :public BlackScholesModel
{
public:            // vecteur des taux d'intérets (r_eur, r_usd, t_hkd)  
	PnlVect* vect_r;

	/**
	* destructeur du modèle Black Scholes generalisé
	*/
	~FXBlackScholes();

	FXBlackScholes(int size, PnlVect* r, PnlVect* sigmas, PnlVect* spots, PnlVect* trends, PnlMat* corr_matrix) :BlackScholesModel(size, pnl_vect_get(r, 0), sigmas, spots, trends, corr_matrix) {
		vect_r = r;
	}
};