#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

/// \brief Classe Option abstraite
class Option
{
public:  /// maturit�
    double T_;     /// nombre de pas de temps de discr�tisation
    int nbTimeSteps_;  /// dimension du mod�le, redondant avec BlackScholesModel::size_
    int size_;  /// vecteur des coefficients du payoff
    PnlVect* payOffCoeffs_;

    /**
     * Calcule la valeur du payoff sur la trajectoire
     *
     * @param[in] path est une matrice de taille (N+1) x d
     * contenant une trajectoire du mod�le telle que cr��e
     * par la fonction asset.
     * @return phi(trajectoire)
     */
    virtual double payoff(const PnlMat* path) = 0;
    /**
* Version optimis�e du calcul du payoff
*
* @param[in] path contient les trajectoires simul�es des sous-jacents
* @param[in] prev_payoff  contient la partie constante du payoff(calcul�e � partir
 * des donn�es du march�)
* @param[in] past_length nombres de dates disponibles sur le march�.
* @return phi(trajectoire)
*/
    virtual double payoff(const PnlMat* path, double prev_payoff, int past_length) = 0;
};