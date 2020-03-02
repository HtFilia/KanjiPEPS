#pragma once

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "BlackScholesModel.hpp"

#include <math.h> 
#include <cmath>

/// Nouveau Mod�le de Black et Scholes
class KanjiBlackScholes
{
public:
	int size_ = 5;  /// nombre d'actifs et des taux de change du mod�le = 5
	PnlVect *r_; /// taux d'int�r�t de nos march�s (r March� EUR --Domestique--, r March� USD, r March� HKD)
	PnlVect *sigma_;   /// vecteur de volatilit�s (sigma Actif March� EUR --Domestique--, sigma Taux change + Actif March� USD, sigma Taux change + Actif March� HKD)
	PnlVect *vols;
	PnlVect *spot_;  /// valeurs initiales des sous-jacents (spot Actifs March� EUR --Domestique--, spot Taux change + Actif March� USD, spot Taux change + Actif March� HKD)
	PnlMat *corrMarket_; /// correlations entre nos Actifs/Taux de change (Colonne j : correlations de l'Actif j avec les diff�rents autres actifs dans l'ordre d�clar� ci-dessus)
	PnlMat *corr; /// matrice de corr�lation des sous-jacents
	PnlVect *trend_;  /// param�tre de tendance de march�  (tendance Actifs March� EUR --Domestique--, tendance Taux change + Actif March� USD, tendance Taux change + Actif March� HKD)
	PnlMat* G;  /// matrice de vecteurs gaussiens, servant � estimer les trajectoires avec B&S
	PnlVect* Ld;  /// une Ligne d de la matrice triangulaire inf�rieure extraite par cholesky
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
