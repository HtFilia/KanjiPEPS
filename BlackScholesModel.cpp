/**
 * \file BlackScholesModel.cpp
 *
 *
 * \brief Fichier source de la classe BlackScholesModel
 *
 * \authors LEBIHAN Lucas, COUTE Lucas, MOMMEJA Léonard, PRÊTRE-HECKENROTH Raphaël
 * Fait le 11.09.2019
 *
 */

#include <iostream>
#include "BlackScholesModel.hpp"
#include "math.h"

/**
 * Génère une trajectoire du modèle et la stocke dans path
 *
 * @param[out] path contient une trajectoire du modèle.
 * C'est une matrice de taille (nbTimeSteps+1) x d
 * @param[in] T  maturité
 * @param[in] nbTimeSteps nombre de dates de constatation
 */
void BlackScholesModel::asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng) {
    // Calcul du timestep
    double timestep = T / (double)nbTimeSteps;

    // Calcul de L
    PnlMat* L = pnl_mat_create_from_scalar(size_, size_, rho_);
    for (int d = 0; d < size_; d++) {
        pnl_mat_set(L, d, d, 1);
    }
    pnl_mat_chol(L);
    PnlVect* L_d = pnl_vect_create(size_);

    // Calcul de G
    PnlMat* G = pnl_mat_create(nbTimeSteps, size_);
    pnl_mat_rng_normal(G, nbTimeSteps, size_, rng);
    PnlVect* G_i = pnl_vect_create(size_);

    // On copie les spots sur le marché
    pnl_mat_set_row(path, spot_, 0);

    // Pour chaque temps t_i
    for (int i = 1; i <= nbTimeSteps; i++) {
        // Récupération de G_i
        pnl_mat_get_row(G_i, G, i - 1);

        // Pour chaque spot d
        for (int d = 0; d < size_; d++) {
            // Récupération de L_d
            pnl_mat_get_row(L_d, L, d);

            // Calcul de l'élément (t_i, d) de la matrice en fonction de l'élément (t_(i-1), d)
            double exposant = (r_ - SQR(GET(sigma_, d)) / 2) * timestep + GET(sigma_, d) * sqrt(timestep) * pnl_vect_scalar_prod(L_d, G_i);
            pnl_mat_set(path, i, d, pnl_mat_get(path, i - 1, d) * exp(exposant));
        }
    }

    //free
//    pnl_vect_free(&G_i);
//    pnl_vect_free(&L_d);
//    pnl_mat_free(&L);
//    pnl_mat_free(&G);
}

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
 */
void BlackScholesModel::asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past) {
    int index = ceil(t * nbTimeSteps / T);

    // Calcul de L
    PnlMat* L = pnl_mat_create_from_scalar(size_, size_, rho_);
    for (int d = 0; d < size_; d++) {
        pnl_mat_set(L, d, d, 1);
    }
    pnl_mat_chol(L);
    PnlVect* L_d = pnl_vect_create(size_);

    // Calcul de G
    PnlMat* G = pnl_mat_create(nbTimeSteps - index + 1, size_);
    pnl_mat_rng_normal(G, nbTimeSteps - index + 1, size_, rng);
    PnlVect* G_i = pnl_vect_create(size_);

    // On copie les résultats de past (lignes [0 ; i]) plus les résultats en t
    pnl_mat_set_subblock(path, past, 0, 0);

    // Remplacement des cours en t par ceux en t_(i+1) (ligne i+1)
    pnl_mat_get_row(G_i, G, 0);

    double timestep = (index * T / (double)nbTimeSteps) - t;
    for (int d = 0; d < size_; d++) {
        pnl_mat_get_row(L_d, L, d);
        double exposant = (r_ - SQR(GET(getSigma(), d)) / 2) * timestep + GET(getSigma(), d) * sqrt(timestep) * pnl_vect_scalar_prod(L_d, G_i);
        pnl_mat_set(path, index, d, pnl_mat_get(path, index, d) * exp(exposant));
    }

    timestep = T / (double)nbTimeSteps;
    // Pour chaque temps restant (lignes [i+2 ; N])
    for (int i = index + 1; i <= nbTimeSteps; i++) {
        // Calcul de G_i
        pnl_mat_get_row(G_i, G, i - index);

        // Pour chaque spot
        for (int d = 0; d < size_; d++) {
            pnl_mat_get_row(L_d, L, d);
            double exposant = (r_ - SQR(GET(getSigma(), d)) / 2) * timestep + GET(getSigma(), d) * sqrt(timestep) * pnl_vect_scalar_prod(L_d, G_i);
            pnl_mat_set(path, i, d, pnl_mat_get(path, i - 1, d) * exp(exposant));
        }
    }

    //free
//    pnl_vect_free(&G_i);
//    pnl_vect_free(&L_d);
//    pnl_mat_free(&L);
//    pnl_mat_free(&G);
}

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
void BlackScholesModel::shiftAsset(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep) {

    // Copy shift-path
    pnl_mat_set_subblock(shift_path, path, 0, 0);

    // Shifting Start
    for (int i = floor(t / timestep) + 1; i < path->m; i++) {
        pnl_mat_set(shift_path, i, d, pnl_mat_get(shift_path, i, d) * (1 + h));
    }
}

/**
     * Simulation du marché
     *
     * @param[out]  path contient en input la simulation du marché
     * @param[in] T date jusqu'à laquelle on simule la trajectoire
     * @param[in] H nombre de dates pour la simulation
     * @param[in] rng générateur de nombres aléatoires
     */
void BlackScholesModel::simul_market(PnlMat *path, double T, int H, PnlRng *rng) {
    // Calcul du timestep
    double timestep = T / (double)H;

    // Calcul de L
    PnlMat* L = pnl_mat_create_from_scalar(size_, size_, rho_);
    for (int d = 0; d < size_; d++) {
        pnl_mat_set(L, d, d, 1);
    }
    pnl_mat_chol(L);
    PnlVect* L_d = pnl_vect_create(size_);

    // Calcul de G
    PnlMat* G = pnl_mat_create(H, size_);
    pnl_mat_rng_normal(G, H, size_, rng);
    PnlVect* G_i = pnl_vect_create(size_);

    // On copie les spots sur le marché
    pnl_mat_set_row(path, spot_, 0);

    // Pour chaque temps t_i
    for (int i = 1; i <= H; i++) {
        // Récupération de G_i
        pnl_mat_get_row(G_i, G, i - 1);

        // Pour chaque spot d
        for (int d = 0; d < size_; d++) {
            // Récupération de L_d
            pnl_mat_get_row(L_d, L, d);

            // Calcul de l'élément (t_i, d) de la matrice en fonction de l'élément (t_(i-1), d)
            double exposant = (GET(trend_, d) - SQR(GET(sigma_, d)) / 2) * timestep + GET(sigma_, d) * sqrt(timestep) * pnl_vect_scalar_prod(L_d, G_i);
            pnl_mat_set(path, i, d, pnl_mat_get(path, i - 1, d) * exp(exposant));
        }
    }

    //free
    pnl_vect_free(&G_i);
    pnl_vect_free(&L_d);
    pnl_mat_free(&L);
    pnl_mat_free(&G);
}
