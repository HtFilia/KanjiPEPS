#pragma once

#include "Option.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_cdf.h"
#include "pnl/pnl_specfun.h"

class Quanto : public Option
{
public:
	double strike_;
	bool mc_pricing;
	PnlVect* vect_r_;

	/**
* Constructeur de l'option quanto
  On considère ici une option quanto EUR/SUD
  Le contrat est un call sur un actif domestique (EUR) payé en dollar.
*
* @param[in] T maturité de l'option
* @param[in] nbTimeSteps  nombre de jours de simulation
* @param[in] size nombre des sous-jacents
* @param[in] strike strike de l'option quanto
*/
	Quanto(double T, int nbTimeSteps, int size, double strike, PnlVect* vect_r) {
		T_ = T;
		nbTimeSteps_ = nbTimeSteps;
		size_ = size;
		strike_ = strike;
		vect_r_ = vect_r;
		type_ = quanto;
		mc_pricing = true;
	}
	/**
* Payoff de l'option quanto à partir du marché
*
* @param[in] S_T le prix à maturité du sous-jacent en euros
  @param[in] r_usd le taux d'intéret sur le marché US
  @param[in] Z_eur : prix à maturité de l'actif sans risque américain en euros
*/
	double payoff(double S_T, double Z_eur, double r_usd);
	/*
	* Prix par formule fermée de l'option quanto EUR/USD
	*
	* param[in] t l'instant de la valorisation de l'option
	* param[in] spot_S valeur du sous-jacent à t en euros
	* param[in] spot_Z valeur à t de l'actif sans risque américain en euros
	* param[in] r vecteur des taux sans risques (r_euro, r_used)
	* param[in] sigma vecteur des volatilités (sigma_S, sigma_X du taux de change USD/EUR)
	* param[in] corr correlation entre le sous-jacent S et le taux de change X
	*/
	double price(double t, double spot_S, double spot_Z, PnlVect* r, PnlVect* sigma, double corr);
	double delta(double t, double spot_S, double spot_Z, PnlVect* r, PnlVect* sigma, double corr);
	double delta_zc(double t, double spot_S, double spot_Z, PnlVect* r, PnlVect* sigmas, double corr);


	double payoff(const PnlMat* path);
};