#pragma once

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

#include <cmath>

/// \brief Mod�le de Black et Scholes
class BlackScholesModel
{
public:         /// nombre d'actifs du mod�le
    int size_;  /// taux d'int�r�t
    double r_; /// param�tre de corr�lation
    double rho_; /// vecteur de volatilit�s
    PnlVect* sigma_;    /// valeurs initiales des sous-jacents
    PnlVect* spot_;  /// matrice de corr�lation des sous-jacents
    PnlMat* corr; /// param�tre de tendance de march�
    PnlVect* trend_;  /// matrice de vecteurs gaussiens, servant � estimer les trajectoires avec B&S
    PnlMat* G;  /// une Ligne d de la matrice triangulaire inf�rieure extraite par cholesky
    PnlVect* Ld;  /// Vecteur gaussien i
    PnlVect* Gi;
    /**
        * G�n�re une trajectoire du mod�le et la stocke dans path
        *
        * Destructeur de l'objet BlackScholesModel, fait appel aux m�thodes free de pnl
        */
    ~BlackScholesModel();
    /**
    * Constructeur de l'object BlackScholesModel
    *
    * @param[in] size nombre de sous-jacents
    * @param[in] r  taux d'int�ret
    * @param[in] rho coefficient de corr�lation
     * @param[in] sigma vecteur de volatilit�s des sous-jacents
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
     * G�n�re une trajectoire du mod�le et la stocke dans path
     *
     * @param[out] path contient une trajectoire du mod�le.
     * C'est une matrice de taille (nbTimeSteps+1) x d
     * @param[in] T  maturit�
     * @param[in] nbTimeSteps nombre de dates de constatation
     * @param[in] rng G�n�rateur al�atoire de pnl
     */
    void asset(PnlMat* path, double T, int nbTimeSteps, PnlRng* rng);



    /**
     * Calcule une trajectoire du mod�le connaissant le
     * pass� jusqu' � la date t
     *
     * @param[out] path  contient une trajectoire du sous-jacent
     * donn�e jusqu'� l'instant t par la matrice past
     * @param[in] t date jusqu'� laquelle on connait la trajectoire.
     * t n'est pas forc�ment une date de discr�tisation
     * @param[in] nbTimeSteps nombre de pas de constatation
     * @param[in] T date jusqu'� laquelle on simule la trajectoire
     * @param[in] past trajectoire r�alis�e jusqu'a la date t
     * @param[in] rng G�n�rateur al�atoire de pnl

     */
    void asset(PnlMat* path, double t, double T, int nbTimeSteps, PnlRng* rng, const PnlMat* past);

    /**
     * Shift d'une trajectoire du sous-jacent
     *
     * @param[in]  path contient en input la trajectoire
     * du sous-jacent
     * @param[out] shift_path contient la trajectoire path
     * dont la composante d a �t� shift�e par (1+h)
     * � partir de la date t.
     * @param[in] t date � partir de laquelle on shift
     * @param[in] h pas de diff�rences finies
     * @param[in] d indice du sous-jacent � shifter
     * @param[in] timestep pas de constatation du sous-jacent
     */
    void shiftAsset(PnlMat* shift_path, const PnlMat* path, int d, double h, double t, double timestep);

    /**
 * Simule le march� (trajectoires des sous-jacents) � partir du vecteur des tendances.
 *
 * @param[out]  path contient en output la trajectoire
 * des sous-jacents
 * @param[in] T maturit� de l'option
 * @param[in] heg_dates_number nombres de dates de rebalancement
 * @param[in] rng gener�ateur aleatoire de pnl
 */
    void simul_market(PnlMat* path, double T, int heg_dates_number, PnlRng* rng);

};
