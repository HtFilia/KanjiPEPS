//
// Created by boutzatz on 9/17/19.
//

#ifndef MC_PRICER_HEDGE_H
#define MC_PRICER_HEDGE_H

#include "MonteCarlo.hpp"
/// \brief Classe Hedge. Assure la couverture de l'option en question et calcule l'erreur PNL de couverture

class Hedge {
public:              /// pointeur vers l'estimateur de monte carlo
    MonteCarlo* mc_;
    /**
* Constructeur de la classe Hedge
*
* @param[in] mc estimateur de monte carlo
*/
    Hedge(MonteCarlo* mc) : mc_(mc) {
    }


    /**
     * Calcule la part de l'actif sans risque
     *
     * @param[in] path contient la trajectoire du sous-jacent
     * jusqu'à maturité de l'option
     * @param[in] N contient le pas de simulation
     * @param[out] portfolio_values contient les valeurs du portefeuille de couverture à chaque instant de rebalancement
     * @param[out] option_prices contient les prix de l'option
     * @param[out] error contient l'erreur de couverture
     */
    void PnL(PnlMat *path, int n_time_steps, int H, PnlVect *portfolio_values, PnlVect* option_prices, double& error);

	void PnL(PnlMat * path, int n_time_steps, double hedge_freq, PnlVect * portfolio_values, PnlVect * option_prices, double & error);

	PnlVect * rebalance(PnlMat* path, int path_index, int hedging_index, double t, int N, int H, double T, PnlVect* option_prices, PnlVect* portfolio_values, PnlVect *delta_prev);
};


#endif //MC_PRICER_HEDGE_H
