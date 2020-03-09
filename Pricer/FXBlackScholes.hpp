#pragma once
#include "BlackScholesModel.hpp"
class FXBlackScholes :
	public BlackScholesModel
{
public:            // vecteur des taux d'intérets (r_eur, r_usd, t_hkd)  
	PnlVect*vect_r; 

	/**
	* destructeur du modèle Black Scholes generalisé
	*/
	~FXBlackScholes();

	FXBlackScholes(int size, PnlVect* r, PnlVect* sigmas, PnlVect* spots, PnlVect* trends, PnlMat* corr_matrix):BlackScholesModel(size, pnl_vect_get(r, 0), sigmas, spots, trends, corr_matrix) {
		vect_r = r;

		// case kanji product
		if (size == 5) {
			pnl_vect_set(sigma_, 2, sqrt(pow(pnl_vect_get(sigmas, 2), 2) + pow(pnl_vect_get(sigmas, 1), 2) + 2 * pnl_vect_get(sigmas, 2) * pnl_vect_get(sigmas, 1) * pnl_mat_get(corr_matrix, 1, 2)));
			pnl_vect_set(sigma_, 4, sqrt(pow(pnl_vect_get(sigmas, 4), 2) + pow(pnl_vect_get(sigmas, 3), 2) + 2 * pnl_vect_get(sigmas, 4) * pnl_vect_get(sigmas, 3) * pnl_mat_get(corr_matrix, 3, 4)));

			pnl_vect_set(trend_, 1, pnl_vect_get(trends, 1) + pnl_vect_get(r, 1));
			pnl_vect_set(trend_, 2, pnl_vect_get(trends, 2) + pnl_vect_get(trends, 1) + pnl_vect_get(sigmas, 2) * pnl_vect_get(sigmas, 1) * pnl_mat_get(corr_matrix, 1, 2));
			pnl_vect_set(trend_, 3, pnl_vect_get(trends, 3) + pnl_vect_get(r, 2));
			pnl_vect_set(trend_, 4, pnl_vect_get(trends, 4) + pnl_vect_get(trends, 3) + pnl_vect_get(sigmas, 4) * pnl_vect_get(sigmas, 3) * pnl_mat_get(corr_matrix, 3, 4));
		}
		// case quanto product
		else if (size == 2){
			pnl_vect_set(trend_, 1, pnl_vect_get(trends, 1) + pnl_vect_get(r, 1));

		}
		



			
	} 

};



