//
// Created by afrai on 9/13/19.
//

#ifndef MC_PRICER_PERFORMANCEOPTION_H
#define MC_PRICER_PERFORMANCEOPTION_H

#include "Option.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
/// \brief Classe de l'option performance

class PerformanceOption : public Option {

public:
    /**
* Constructeur de l'option performance
*
* @param[in] T maturit� de l'option
* @param[in] nbTimeSteps  nombre de jours de simulation
* @param[in] size nombre des sous-jacents
 * @param[in] payOffCoeffs vecteur des coefficients de payoffs
*/
    PerformanceOption(double T, int nbTimeSteps, int size, PnlVect* payOffCoeffs) {
        T_ = T;
        nbTimeSteps_ = nbTimeSteps;
        size_ = size;
        payOffCoeffs_ = payOffCoeffs;
    }
    /**
* Payoff de l'option performance � partir du march�
*
* @param[in] path matrices des trajectoires des sous-jacents
*/
    double payoff(const PnlMat* path);
    /**
    * Version optimis�e du calcul du payoff
    *
    * @param[in] path contient les trajectoires simul�es des sous-jacents
    * @param[in] past_payoff  contient la partie constante du payoff(calcul�e � partir
     * des donn�es du march�)
    * @param[in] past_len nombres de dates disponibles sur le march�.
    */
    double payoff(const PnlMat* path, double past_payoff, int past_len);
};

#endif //MC_PRICER_PERFORMANCEOPTION_H
