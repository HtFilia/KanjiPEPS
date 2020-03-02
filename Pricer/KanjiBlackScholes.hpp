#pragma once

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "BlackScholesModel.hpp"

#include <math.h> 
#include <cmath>

/// Nouveau Modéle de Black et Scholes
class KanjiBlackScholes
{
public:
	int size_ = 5;  /// nombre d'actifs et des taux de change du modèle = 5
	PnlVect *r_; /// taux d'intérêt de nos marchés (r Marché EUR --Domestique--, r Marché USD, r Marché HKD)
	PnlVect *sigma_;   /// vecteur de volatilités (sigma Actif Marché EUR --Domestique--, sigma Taux change + Actif Marché USD, sigma Taux change + Actif Marché HKD)
	PnlVect *vols;
	PnlVect *spot_;  /// valeurs initiales des sous-jacents (spot Actifs Marché EUR --Domestique--, spot Taux change + Actif Marché USD, spot Taux change + Actif Marché HKD)
	PnlMat *corrMarket_; /// correlations entre nos Actifs/Taux de change (Colonne j : correlations de l'Actif j avec les différents autres actifs dans l'ordre déclaré ci-dessus)
	PnlMat *corr; /// matrice de corrélation des sous-jacents
	PnlVect *trend_;  /// paramètre de tendance de marché  (tendance Actifs Marché EUR --Domestique--, tendance Taux change + Actif Marché USD, tendance Taux change + Actif Marché HKD)
	PnlMat* G;  /// matrice de vecteurs gaussiens, servant à estimer les trajectoires avec B&S
	PnlVect* Ld;  /// une Ligne d de la matrice triangulaire inférieure extraite par cholesky
	PnlVect* Gi; /// Vecteur gaussien i

	~KanjiBlackScholes();

	KanjiBlackScholes(PnlVect *r, PnlMat *corrMarket, PnlVect *sigma, PnlVect *spot, PnlVect* trend);

	void asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng);

	void asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past);

	void asset2(PnlMat * path, double t, double T, int nbTimeSteps, PnlRng * rng, const PnlMat * past, PnlMat * G);

	void shiftAsset(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep);

	void simul_market(PnlMat *path, double T, int M, PnlRng *rng);

	void getPast(PnlMat *past, PnlMat *path, double t, int n_time_steps, double T);

};
