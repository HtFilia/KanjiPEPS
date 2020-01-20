//
// Created by afrai on 9/13/19.
//

#ifndef MC_PRICER_PERFORMANCEOPTION_H
#define MC_PRICER_PERFORMANCEOPTION_H

#include "Option.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
/// \brief Classe de l'option performance

class PerformanceOption : public Option {

public:
    /**
* Constructeur de l'option performance
*
* @param[in] T maturité de l'option
* @param[in] nbTimeSteps  nombre de jours de simulation
* @param[in] size nombre des sous-jacents
 * @param[in] payOffCoeffs vecteur des coefficients de payoffs
*/
    PerformanceOption(double T, int nbTimeSteps, int size, PnlVect * weights) {
        T_ = T;
        nbTimeSteps_ = nbTimeSteps;
        size_ = size;
		weights_ = weights;
    }
    /**
* Payoff de l'option performance à partir du marché
*
* @param[in] path matrices des trajectoires des sous-jacents
*/
    double payoff(const PnlMat *path);
};

#endif //MC_PRICER_PERFORMANCEOPTION_H
