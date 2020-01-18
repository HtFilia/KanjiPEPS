#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

/// \brief Classe Option abstraite
class Option
{
public:  /// maturité
    double T_;     /// nombre de pas de temps de discrétisation
    int nbTimeSteps_;  /// dimension du modèle, redondant avec BlackScholesModel::size_
    int size_;  /// vecteur des coefficients du payoff
    PnlVect * payOffCoeffs_;

    /**
     * Calcule la valeur du payoff sur la trajectoire
     *
     * @param[in] path est une matrice de taille (N+1) x d
     * contenant une trajectoire du modèle telle que créée
     * par la fonction asset.
     * @return phi(trajectoire)
     */
    virtual double payoff(const PnlMat *path) = 0;
    /**
* Version optimisée du calcul du payoff
*
* @param[in] path contient les trajectoires simulées des sous-jacents
* @param[in] prev_payoff  contient la partie constante du payoff(calculée à partir
 * des données du marché)
* @param[in] past_length nombres de dates disponibles sur le marché.
* @return phi(trajectoire)
*/
    virtual double payoff(const PnlMat *path, double prev_payoff, int past_length) = 0;
};



