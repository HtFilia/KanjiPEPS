#pragma once

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

#include <cmath>

/// \brief Modéle de Black et Scholes
class BlackScholesModel
{
public:         /// nombre d'actifs du modèle
    int size_;  /// taux d'intérêt
    double r_; /// paramètre de corrélation
    double rho_; /// vecteur de volatilités
    PnlVect* sigma_;    /// valeurs initiales des sous-jacents
    PnlVect* spot_;  /// matrice de corrélation des sous-jacents
    PnlMat* corr; /// paramètre de tendance de marché
    PnlVect* trend_;  /// matrice de vecteurs gaussiens, servant à estimer les trajectoires avec B&S
    PnlMat* G;  /// une Ligne d de la matrice triangulaire inférieure extraite par cholesky
    PnlVect* Ld;  /// Vecteur gaussien i
    PnlVect* Gi;
    /**
        * Génère une trajectoire du modèle et la stocke dans path
        *
        * Destructeur de l'objet BlackScholesModel, fait appel aux méthodes free de pnl
        */
    ~BlackScholesModel();
    /**
    * Constructeur de l'object BlackScholesModel
    *
    * @param[in] size nombre de sous-jacents
    * @param[in] r  taux d'intéret
    * @param[in] rho coefficient de corrélation
     * @param[in] sigma vecteur de volatilités des sous-jacents
     * @param[in] spot vecteur des valeurs initiales des sous-jacents
     * @param[in] trend vecteur des tendances
    */
    BlackScholesModel(int size, double r, double rho, PnlVect* sigma, PnlVect* spot, PnlVect* trend) {
        size_ = size;
        r_ = r;
        rho_ = rho;
        sigma_ = sigma;
        spot_ = spot;
        trend_ = trend;
        corr = pnl_mat_create_from_scalar(size_, size_, rho_);
        for (int diag = 0; diag < size_; diag++)
            pnl_mat_set(corr, diag, diag, 1);
        pnl_mat_chol(corr);
    }

    /**
     * Génère une trajectoire du modèle et la stocke dans path
     *
     * @param[out] path contient une trajectoire du modèle.
     * C'est une matrice de taille (nbTimeSteps+1) x d
     * @param[in] T  maturité
     * @param[in] nbTimeSteps nombre de dates de constatation
     * @param[in] rng Générateur aléatoire de pnl
     */
    void asset(PnlMat* path, double T, int nbTimeSteps, PnlRng* rng);



    /**
     * Calcule une trajectoire du modèle connaissant le
     * passé jusqu' à la date t
     *
     * @param[out] path  contient une trajectoire du sous-jacent
     * donnée jusqu'à l'instant t par la matrice past
     * @param[in] t date jusqu'à laquelle on connait la trajectoire.
     * t n'est pas forcément une date de discrétisation
     * @param[in] nbTimeSteps nombre de pas de constatation
     * @param[in] T date jusqu'à laquelle on simule la trajectoire
     * @param[in] past trajectoire réalisée jusqu'a la date t
     * @param[in] rng Générateur aléatoire de pnl

     */
    void asset(PnlMat* path, double t, double T, int nbTimeSteps, PnlRng* rng, const PnlMat* past);

    /**
     * Shift d'une trajectoire du sous-jacent
     *
     * @param[in]  path contient en input la trajectoire
     * du sous-jacent
     * @param[out] shift_path contient la trajectoire path
     * dont la composante d a été shiftée par (1+h)
     * à partir de la date t.
     * @param[in] t date à partir de laquelle on shift
     * @param[in] h pas de différences finies
     * @param[in] d indice du sous-jacent à shifter
     * @param[in] timestep pas de constatation du sous-jacent
     */
    void shiftAsset(PnlMat* shift_path, const PnlMat* path, int d, double h, double t, double timestep);

    /**
 * Simule le marché (trajectoires des sous-jacents) à partir du vecteur des tendances.
 *
 * @param[out]  path contient en output la trajectoire
 * des sous-jacents
 * @param[in] T maturité de l'option
 * @param[in] heg_dates_number nombres de dates de rebalancement
 * @param[in] rng generéateur aleatoire de pnl
 */
    void simul_market(PnlMat* path, double T, int heg_dates_number, PnlRng* rng);

};
