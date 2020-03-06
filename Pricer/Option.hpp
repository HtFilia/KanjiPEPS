#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_mathtools.h"

enum optionType { call, kanji };

/// \brief Classe Option abstraite
class Option
{
public:  /// maturité
    double T_;     /// nombre de pas de temps de discrétisation
    int nbTimeSteps_;  /// dimension du modèle, redondant avec BlackScholesModel::size_
    int size_; /// type de l'option
	optionType type_;


    /**
     * Calcule la valeur du payoff sur la trajectoire
     *
     * @param[in] path est une matrice de taille (N+1) x d
     * contenant une trajectoire du modèle telle que créée
     * par la fonction asset.
     * @return phi(trajectoire)
     */
    virtual double payoff(const PnlMat *path) = 0;
	virtual double payoff(const PnlMat *path, PnlVect* initial_values) = 0;


};



