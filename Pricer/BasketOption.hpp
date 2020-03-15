//
// Created by boutzatz on 9/13/19.
//

#ifndef PRICERMONTECARLO_BASKETOPTION_H
#define PRICERMONTECARLO_BASKETOPTION_H

#include "Option.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
/// \brief Classe de l'option panier

class BasketOption : public Option {

public:                 /// strike de l'option panier
    double strike_;
    PnlVect* payOffCoeffs_;
    /**
* Constructeur de l'option panier
*
* @param[in] T maturité de l'option
* @param[in] nbTimeSteps  nombre de jours de simulation
* @param[in] size nombre des sous-jacents
* @param[in] payOffCoeffs  vecteur des coefficients de payoffs
* @param[in] strike strike de l'option panier
*/
    BasketOption(double T, int nbTimeSteps, int size, PnlVect* payOffCoeffs, double strike) {
        T_ = T;
        nbTimeSteps_ = nbTimeSteps;
        size_ = size;
        payOffCoeffs_ = payOffCoeffs;
        strike_ = strike;
    }
    /**
* Payoff de l'option panier à partir du marché
*
* @param[in] path matrices des trajectoires des sous-jacents
*/
    double payoff(const PnlMat* path);
    /**
     * Destructeur de l'objet option basket
     */
    ~BasketOption() {
        pnl_vect_free(&payOffCoeffs_);
    };
    /**
    * Version optimisée du calcul du payoff
    *
    * @param[in] path contient les trajectoires simulées des sous-jacents
    * @param[in] prev_payoff  contient la partie constante du payoff(calculée à partir
     * des données du marché)
    * @param[in] past_length nombres de dates disponibles sur le marché.
    */
    double payoff(const PnlMat* path, double prev_payoff, int past_length);
};


#endif //PRICERMONTECARLO_BASKETOPTION_H


