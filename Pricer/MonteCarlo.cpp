//
// Created by boutzatz on 9/13/19.
//

#include "MonteCarlo.hpp"
#include <cmath>
#include <iostream>

MonteCarlo::~MonteCarlo() {
    delete mod_;
    pnl_rng_free(&rng_);


}

void MonteCarlo::price(double &prix, double &ic) {
    PnlMat* path =  pnl_mat_create (opt_->nbTimeSteps_ + 1, opt_->size_);
    double payoffs_squared = 0;
    double payoffs = 0;
    double payoff;

    for (int m = 0; m < nbSamples_; m++) {
        mod_->asset(path, opt_->T_, opt_->nbTimeSteps_, rng_);
        payoff = opt_->payoff(path);
        payoffs += payoff;
        payoffs_squared += pnl_pow_i(payoff, 2);
    }
    payoffs = payoffs/nbSamples_;
    payoffs_squared = payoffs_squared/nbSamples_;
    double  var = (pnl_expm1(-2*mod_->r_*opt_->T_)+1)*(payoffs_squared - pnl_pow_i(payoffs,2));
    prix = ( (pnl_expm1(- mod_->r_ * opt_->T_)+1)) * payoffs;
    ic = 2*1.96*sqrt(var)/sqrt(nbSamples_);
    pnl_mat_free(&path);

}

void MonteCarlo::price(const PnlMat *past, double t, double &prix, double &ic) {

    PnlMat* path =  pnl_mat_create (opt_->nbTimeSteps_ + 1, opt_->size_);
    double payoffs_squared = 0;
    double payoffs = 0;
    double payoff;
    for (int m = 0; m < nbSamples_; m++) {
        mod_->asset(path, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
        payoff = opt_->payoff(path);
        payoffs += payoff;
        payoffs_squared += pnl_pow_i(payoff, 2);
    }
    payoffs = payoffs/nbSamples_;
    payoffs_squared = payoffs_squared/nbSamples_;
    double  var = (pnl_expm1(-2*mod_->r_*opt_->T_)+1)*(payoffs_squared - pnl_pow_i(payoffs,2));
    prix = ( (pnl_expm1(- mod_->r_ * (opt_->T_ - t))+1)) * payoffs;
    ic = 2*1.96*sqrt(var)/sqrt(nbSamples_);
    pnl_mat_free(&path);
}


void MonteCarlo::delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic){
	PnlMat* path =  pnl_mat_create (opt_->nbTimeSteps_ + 1, opt_->size_); //creation path, matrice ou on simule
    double timestep = opt_->T_/opt_->nbTimeSteps_; //pas de discretisation
    PnlMat* shifted_pathp =  pnl_mat_create (opt_->nbTimeSteps_ + 1, opt_->size_);
    PnlMat* shifted_pathm =  pnl_mat_create (opt_->nbTimeSteps_ + 1, opt_->size_); //creation de deux paths deviés
    double difference;
    PnlVect *squared_differences = pnl_vect_create_from_scalar(opt_->size_, 0);
    PnlVect *differences = pnl_vect_create_from_scalar(opt_->size_, 0);
    for (int m = 0; m < nbSamples_; ++m) {
        mod_->asset(path,t, opt_->T_, opt_->nbTimeSteps_, rng_, past); //simule le reste du path a partir de past
		if (t == 0.75) { 
			//std::cout << "\n";
			//pnl_mat_print(path); std::cout << "\n";
		} //TEST
		for (int d = 0; d < opt_->size_ ; ++d) {
            mod_->shiftAsset(shifted_pathp, path, d, h, t, timestep); //on shift le path sur la d eme composante
            mod_->shiftAsset(shifted_pathm, path, d, -h, t, timestep);
			difference = opt_->payoff(shifted_pathp) - opt_->payoff(shifted_pathm);//difference des payoffs des deux paths shiftes
			pnl_vect_set(delta, d, difference + pnl_vect_get(delta, d));
            pnl_vect_set(differences, d, difference + pnl_vect_get(differences,d));
            pnl_vect_set(squared_differences,d, pnl_pow_i(difference,2)+pnl_vect_get(squared_differences,d) );
        }
    }

    pnl_vect_mult_scalar(delta, (pnl_expm1(- mod_->r_ *(opt_->T_-t))+1)/(2*nbSamples_*h));
    PnlVect *spot_t = pnl_vect_create(opt_->size_);
    pnl_mat_get_row(spot_t, past, past->m-1);
    pnl_vect_div_vect_term(delta, spot_t);

    pnl_vect_mult_vect_term(differences, differences);
    pnl_vect_div_scalar(differences, nbSamples_);
    pnl_vect_minus_vect(squared_differences, differences);

    pnl_vect_mult_vect_term(spot_t, spot_t); // squared spots vector
    pnl_vect_div_vect_term(squared_differences, spot_t);
    double coeff = (pnl_expm1(-2*mod_->r_*(opt_->T_-t))+1)/(4*nbSamples_*pnl_pow_i(h,2));
    pnl_vect_mult_scalar(squared_differences,coeff);
    pnl_vect_clone(ic, squared_differences);



    // need to square-root it
    int vectsize = ic->size;
    for(int i = 0; i < vectsize; i++){
        pnl_vect_set(ic, i, sqrt(pnl_vect_get(ic,i)));
    }
    pnl_vect_mult_scalar(ic, 2*1.96/sqrt(nbSamples_));

    pnl_vect_free(&differences);
    pnl_vect_free(&squared_differences);
    pnl_mat_free(&path);
    pnl_mat_free(&shifted_pathm);
    pnl_mat_free(&shifted_pathp);
    pnl_vect_free(&spot_t);
}

void MonteCarlo::price_and_delta(const PnlMat *past, double t,double &prix, double &ic, PnlVect *delta, PnlVect *icdelta) {
    PnlMat* path =  pnl_mat_create (opt_->nbTimeSteps_ + 1, opt_->size_);
    double payoffs_squared = 0;
    double payoffs = 0;
    double payoff;
    double timestep = opt_->T_/opt_->nbTimeSteps_;
    PnlMat* shifted_pathp =  pnl_mat_create (opt_->nbTimeSteps_ + 1, opt_->size_);
    PnlMat* shifted_pathm =  pnl_mat_create (opt_->nbTimeSteps_ + 1, opt_->size_);
    PnlVect *squared_differences = pnl_vect_create(opt_->size_);
    PnlVect *differences = pnl_vect_create(opt_->size_);
    double difference;
    for (int m = 0; m < nbSamples_; m++) {
        mod_->asset(path, t, opt_->T_, opt_->nbTimeSteps_, rng_, past);
        payoff = opt_->payoff(path);
        payoffs += payoff;
        payoffs_squared += pnl_pow_i(payoff, 2);
        for (int d = 0; d < opt_->size_ ; ++d) {
            mod_->shiftAsset(shifted_pathp, path, d, h, t, timestep);
            mod_->shiftAsset(shifted_pathm, path, d, -h, t, timestep);
            difference = opt_->payoff(shifted_pathp) - opt_->payoff(shifted_pathm);
            pnl_vect_set(delta, d, difference + pnl_vect_get(delta, d));
            pnl_vect_set(differences, d, difference + pnl_vect_get(differences,d));
            pnl_vect_set(squared_differences,d, pnl_pow_i(difference,2)+pnl_vect_get(squared_differences,d) );

        }
    }
    payoffs = payoffs/nbSamples_;
    payoffs_squared = payoffs_squared/nbSamples_;
    double  var = (pnl_expm1(-2*mod_->r_*opt_->T_)+1)*(payoffs_squared - pnl_pow_i(payoffs,2));
    prix = ( (pnl_expm1(- mod_->r_ * (opt_->T_ - t))+1)) * payoffs;
    ic = 2*1.96*sqrt(var)/sqrt(nbSamples_);
    pnl_vect_mult_scalar(delta, (pnl_expm1(- mod_->r_ *(opt_->T_-t))+1)/(2*nbSamples_*fdStep_));
    PnlVect *spot_t = pnl_vect_create(opt_->size_);
    pnl_mat_get_row(spot_t, past, past->m-1);
    pnl_vect_div_vect_term(delta, spot_t);

    pnl_vect_mult_vect_term(differences, differences);
    pnl_vect_div_scalar(differences, nbSamples_);
    pnl_vect_minus_vect(squared_differences, differences);

    pnl_vect_mult_vect_term(spot_t, spot_t); // squared spots vector
    pnl_vect_div_vect_term(squared_differences, spot_t);
    double coeff = (pnl_expm1(-2*mod_->r_*(opt_->T_-t))+1)/(4*nbSamples_*pnl_pow_i(fdStep_,2));
    pnl_vect_mult_scalar(squared_differences,coeff);
    pnl_vect_clone(icdelta, squared_differences);


    int vectsize = icdelta->size;
    for(int i = 0; i < vectsize; i++){
        pnl_vect_set(icdelta, i, sqrt(pnl_vect_get(icdelta,i)));
    }
    pnl_vect_mult_scalar(icdelta, 2*1.96/sqrt(nbSamples_));

    pnl_vect_free(&differences);
    pnl_vect_free(&squared_differences);
    pnl_mat_free(&path);
    pnl_mat_free(&shifted_pathm);
    pnl_mat_free(&shifted_pathp);
    pnl_vect_free(&spot_t);
}

