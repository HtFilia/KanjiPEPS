#include "Computations.hpp"
#include <iostream>
#include <time.h>
#include "pnl/pnl_random.h"

using namespace std;


void Computations::calleuro(double& ic, double& prix, int nb_samples, double T,
    double S0, double K, double sigma, double r)
{
    double drift = (r - sigma * sigma / 2.) * T;
    double sqrt_T = sqrt(T);
    double sum = 0;
    double var = 0;
    PnlRng* rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));
    double payoff;
    for (int i = 0; i < nb_samples; i++)
    {
        payoff = S0 * exp(drift + sigma * sqrt_T * pnl_rng_normal(rng));
        payoff = MAX(payoff - K, 0.);
        sum += payoff;
        var += payoff * payoff;
    }

    prix = exp(-r * T) * sum / nb_samples;
    var = exp(-2. * r * T) * var / nb_samples - prix * prix;
    ic = 1.96 * sqrt(var / nb_samples);
    pnl_rng_free(&rng);
}

void Computations::callMC(double& ic, double& prix, int nb_samples, double T,
    double S0, double K, double sigma, double r)
{
    //TODO
}

void Computations::performance_price_hedge(double& ic, double& prix, double ic_deltas[], double deltas[], int nb_samples, double T,
    double S0_[], double K, double sigma_[], double correlation, double r)
{
    //TODO
}