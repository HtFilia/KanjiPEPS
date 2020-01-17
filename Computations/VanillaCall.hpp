//
// Created by afrai on 9/13/19.
//

#ifndef MC_PRICER_VANILLACALLOPTION_H
#define MC_PRICER_VANILLACALLOPTION_H

#include "Option.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_cdf.h"
#include "pnl/pnl_specfun.h"
/// \brief Classe de l'option performance

class VanillaCall : public Option {

public:
	double strike_;
	/**
* Constructeur de l'option performance
*
* @param[in] T maturité de l'option
* @param[in] nbTimeSteps  nombre de jours de simulation
* @param[in] size nombre des sous-jacents
 * @param[in] payOffCoeffs vecteur des coefficients de payoffs
*/
	VanillaCall(double T, int nbTimeSteps, int size, double strike) {
		T_ = T;
		nbTimeSteps_ = nbTimeSteps;
		size_ = size;
		strike_ = strike;
	}
	/**
* Payoff de l'option performance à partir du marché
*
* @param[in] path matrices des trajectoires des sous-jacents
*/
	double payoff(double S_T);

	double payoff(const PnlMat *path, double past_payoff, int past_len);
	double payoff(const PnlMat *path);
	double price(double t, double spot, double r, double sigma, double maturity, double strike);
	double delta(double t, double spot, double r, double sigma, double maturity, double strike);
};



#endif //MC_PRICER_PERFORMANCEOPTION_H
