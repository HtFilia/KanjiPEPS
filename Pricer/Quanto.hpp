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
  On consid�re ici une option quanto EUR/SUD
  Le contrat est un call sur un actif domestique (EUR) pay� en dollar.
*
* @param[in] T maturit� de l'option
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
* Payoff de l'option quanto � partir du march�
*
* @param[in] S_T le prix � maturit� du sous-jacent en euros
  @param[in] r_usd le taux d'int�ret sur le march� US
  @param[in] Z_eur : prix � maturit� de l'actif sans risque am�ricain en euros
*/
	double payoff(double S_T, double Z_eur, double r_usd);
	/*
	* Prix par formule ferm�e de l'option quanto EUR/USD
	*
	* param[in] t l'instant de la valorisation de l'option
	* param[in] spot_S valeur du sous-jacent � t en euros
	* param[in] spot_Z valeur � t de l'actif sans risque am�ricain en euros
	* param[in] r vecteur des taux sans risques (r_euro, r_used)
	* param[in] sigma vecteur des volatilit�s (sigma_S, sigma_X du taux de change USD/EUR)
	* param[in] corr correlation entre le sous-jacent S et le taux de change X
	*/
	double price(double t, double spot_S, double spot_Z, PnlVect* r, PnlVect* sigma, double corr);
	double delta(double t, double spot_S, double spot_Z, PnlVect* r, PnlVect* sigma, double corr);
	double delta_zc(double t, double spot_S, double spot_Z, PnlVect* r, PnlVect* sigmas, double corr);


	double payoff(const PnlMat* path);
};