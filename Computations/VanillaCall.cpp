#include "VanillaCall.hpp"
#include <iostream>


double VanillaCall::payoff(double S_T) {


	return S_T > strike_ ? S_T - strike_: 0 ;
}

double VanillaCall::payoff(const PnlMat *path, double past_payoff, int past_len) {

	
	return 1;
}

double VanillaCall::payoff(const PnlMat *path) {
	PnlVect *vect = pnl_vect_create(path->m);
	pnl_mat_get_col(vect, path, 0);
	double S_T = pnl_vect_get(vect, path->m-1);
	return S_T > strike_ ? S_T - strike_ : 0;

}
