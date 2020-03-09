#pragma once

#include "Option.hpp"
#include "VanillaCall.hpp"
#include "BlackScholesModel.hpp"
#include "pnl/pnl_random.h"

/// \brief Classe de l'estimateur de Monte Carlo

class MonteCarlo
{
public:                      ///  pointeur vers le modèle
    BlackScholesModel *mod_;  /// pointeur sur l'option
    Option *opt_; /// pointeur sur le générateur
    PnlRng *rng_; /// pas de différence finie
    double fdStep_; /// nombre de tirages Monte Carlo
    int nbSamples_; /// pas de différence finie
    double h;
	PnlMat* path = pnl_mat_create(opt_->nbTimeSteps_ + 1, opt_->size_);

    /**
* Destructeur de la classe Monte Carlo
     */
    ~MonteCarlo();
    /**
* Constructeur de la classe Monte Carlo
*
 * @param[in] mod pointeur vers le modéle de B&S
 * @param[in] opt pointeur vers l'option
  * @param[in] rng pointeur vers le générateur aléatoire de pnl
  * @param[in] fdStep pas de différence finies de simulation
  * @param[in] nbSamples nombre de tirages Monte Carlo
   * @param[in] h pas de différence finie de rebalancement

*/
    MonteCarlo(BlackScholesModel *mod, Option *opt, PnlRng *rng, double fdStep, int nbSamples, double h) : mod_(mod), opt_(opt),
                                                                                                             rng_(rng),
                                                                                                             fdStep_(fdStep),
                                                                                                             nbSamples_(nbSamples),
                                                                                                             h(h)
                                                                                                             {}
/**
     * Calcule le prix de l'option à la date 0
     *
     * @param[out] prix valeur de l'estimateur Monte Carlo
     * @param[out] ic largeur de l'intervalle de confiance
     */
    void price(double &prix, double &ic);
    /**
     * Calcule le prix de l'option à la date t
     *
     * @param[in]  past contient la trajectoire du sous-jacent
     * jusqu'à l'instant t
     * @param[in] t date à laquelle le calcul est fait
     * @param[out] prix contient le prix
     * @param[out] ic contient la largeur de l'intervalle
     * de confiance sur le calcul du prix
     */
    void price(const PnlMat *past, double t, double &prix, double &ic);

    /**
     * Calcule le delta de l'option à la date t
     *
     * @param[in] past contient la trajectoire du sous-jacent
     * jusqu'à l'instant t
     * @param[in] t date à laquelle le calcul est fait
     * @param[out] delta contient le vecteur de delta
     * @param[out] ic contient la largeur de l'intervalle
     * de confiance sur le calcul du delta
     */
    void delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic);


    /**
     * Calcule le prix et les deltas de l'option à la date t
     *
     * @param[in] past contient la trajectoire du sous-jacent
     * jusqu'à l'instant t
     * @param[in] t date à laquelle le calcul est fait
     * @param[out] prix contient le prix
     * @param[out] ic contient la largeur de l'intervalle
     * de confiance sur le calcul du prix
     * @param[out] delta contient le vecteur de delta
     * @param[out] icdelta contient la largeur de l'intervalle
     * de confiance sur le calcul du delta
     */
    void price_and_delta(const PnlMat *past, double t, double &prix, double &ic, PnlVect *delta, PnlVect *icdelta);
	 void profitAndLoss(const PnlMat * path, double & prix, double & icPrix, double & error);
};


