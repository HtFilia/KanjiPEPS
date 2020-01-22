//
// Created by boutzatz on 9/13/19.
//
#include <iostream>
#include <cassert>
#include "BlackScholesModel.hpp"


BlackScholesModel::~BlackScholesModel() {
    pnl_vect_free(&sigma_);
    pnl_vect_free(&spot_);
    pnl_mat_free(&corr);
}

void BlackScholesModel::asset(PnlMat *path, double T, int nbTimeSteps, PnlRng *rng) {

    pnl_mat_set_row (path, spot_, 0);

    PnlMat* G = pnl_mat_create (nbTimeSteps, size_);
    PnlVect* Ld = pnl_vect_create (size_);
    PnlVect* Gi = pnl_vect_create (size_);

    pnl_mat_rng_normal(G, nbTimeSteps, size_, rng);

    double expo = 0;

    double timeSpan = T/nbTimeSteps;

    for (int d = 0; d < size_; d++) {
        pnl_mat_get_row(Ld, corr, d);
        for (int i = 1; i <= nbTimeSteps; i++) {
            pnl_mat_get_row(Gi, G, i - 1);
            expo = pnl_expm1( (r_ - pnl_pow_i(GET(sigma_, d), 2) / 2)  * timeSpan +
                              GET(sigma_, d) * sqrt(timeSpan) * pnl_vect_scalar_prod(Ld, Gi) ) + 1;
            pnl_mat_set(path, i, d, pnl_mat_get(path, i - 1, d) * expo);
        }
    }
    pnl_mat_free(&G);
    pnl_vect_free(&Ld);
    pnl_vect_free(&Gi);



}



//void BlackScholesModel::asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past) {
//	double timespan = T / nbTimeSteps;
//	PnlMat *G;
//	PnlVect *Ld = pnl_vect_create(size_);
//	PnlVect *Gi = pnl_vect_create(size_);
//	double expo;
//	int i = int(t/timespan);
//	double t_i = i * timespan;
//	bool is_discrete_time = t == t_i;
//	// extracting the past part to the path
//	pnl_mat_extract_subblock(path, past, 0, i + 1, 0, size_);
//	pnl_mat_resize(path, nbTimeSteps + 1, size_);
//	G = pnl_mat_create(nbTimeSteps - i, size_);
//	pnl_mat_rng_normal(G, nbTimeSteps - i, size_, rng);
//	double sigma = 0;
//	double s = 0;
//	double x = 0;
//	double past_s = 0;
//	if (!is_discrete_time) {
//		double delta_t = (i + 1)*timespan - t;
//		pnl_mat_get_row(Gi, G, 0);
//		for (int d = 0; d < size_; d++) {
//			pnl_mat_get_row(Ld, corr, d);
//			sigma = GET(sigma_, d);
//			x = (r_ - 0.5*sigma * sigma) * delta_t + sigma * sqrt(delta_t)*pnl_vect_scalar_prod(Ld, Gi);
//			expo = exp(x);
//			past_s = pnl_mat_get(past, past->m - 1, d);
//			s =  past_s * expo;
//			pnl_mat_set(path, i+1, d, s);
//		}
//	}
//	int offset = !is_discrete_time; 
//	// offset is 1 if t is not a discrete time so that we skip the first row after the past which was filled before
//	for (int d = 0; d < size_; d++) {
//	    pnl_mat_get_row(Ld, corr, d);
//		sigma = GET(sigma_, d);
//	    for (int k = i+1+offset; k <= nbTimeSteps; k++) {
//	        pnl_mat_get_row(Gi, G, k-i-1);
//			x = (r_ - 0.5*sigma*sigma) * timespan + sigma * sqrt(timespan)*pnl_vect_scalar_prod(Ld, Gi);
//			expo = exp(x);
//			past_s = pnl_mat_get(path, k - 1, d);
//			s = past_s * expo;
//			pnl_mat_set(path, k, d, s);
//	    }
//	}
//	pnl_mat_free(&G);
//	pnl_vect_free(&Ld);
//	pnl_vect_free(&Gi);
//}

void BlackScholesModel::asset(PnlMat *path, double t, double T, int nbTimeSteps, PnlRng *rng, const PnlMat *past) {


	if (t == T) {
		pnl_mat_clone(path, past);
		return;
	}
	if (past->m == 1 || t <= 0) {
		asset(path, T, nbTimeSteps, rng);
	}
	else {

		double timeSpan = T / nbTimeSteps;
		double lastTime = (past->m - 2) * timeSpan;

		PnlVect *row = pnl_vect_create(size_);

		for (int n = 0; n <= past->m - 2; n++) {
			pnl_mat_get_row(row, past, n);
			pnl_mat_set_row(path, row, n);
		}

		double expo = 0;

		PnlMat *G;
		PnlVect *Ld = pnl_vect_create(size_);
		PnlVect *Gi = pnl_vect_create(size_);

		if (abs(t - lastTime - timeSpan) < 0.00001) { //bc of rounding pnl error difference should be less than hedging timespan temporarily set to 0.00001

			pnl_mat_get_row(row, past, past->m - 1);
			pnl_mat_set_row(path, row, past->m - 1);

			G = pnl_mat_create(nbTimeSteps + 1 - past->m, size_);
			pnl_mat_rng_normal(G, nbTimeSteps + 1 - past->m, size_, rng);

			for (int d = 0; d < size_; d++) {
				pnl_mat_get_row(Ld, corr, d);
				for (int i = past->m; i <= nbTimeSteps; i++) {
					pnl_mat_get_row(Gi, G, i - past->m);
					expo = pnl_expm1((r_ - pnl_pow_i(GET(sigma_, d), 2) / 2) * timeSpan +
						GET(sigma_, d) * sqrt(timeSpan) * pnl_vect_scalar_prod(Ld, Gi)) + 1;
					pnl_mat_set(path, i, d, pnl_mat_get(path, i - 1, d) * expo);
				}
			}

		}
		else {
			G = pnl_mat_create(nbTimeSteps - past->m + 2, size_);
			pnl_mat_rng_normal(G, nbTimeSteps - past->m + 2, size_, rng);

			for (int d = 0; d < size_; d++) {

				pnl_mat_get_row(Ld, corr, d);

				for (int i = past->m - 1; i <= nbTimeSteps; i++) {

					if (i == past->m - 1) {

						pnl_mat_get_row(Gi, G, i - past->m + 1);
						expo = pnl_expm1((r_ - pnl_pow_i(GET(sigma_, d), 2) / 2) * (i * timeSpan - t) +
							GET(sigma_, d) * sqrt(i * timeSpan - t) * pnl_vect_scalar_prod(Ld, Gi)) + 1;
						pnl_mat_set(path, i, d, expo * pnl_mat_get(past, past->m - 1, d));

					}
					else {

						pnl_mat_get_row(Gi, G, i - past->m + 1);
						expo = pnl_expm1((r_ - pnl_pow_i(GET(sigma_, d), 2) / 2) * timeSpan +
							GET(sigma_, d) * sqrt(timeSpan) * pnl_vect_scalar_prod(Ld, Gi)) + 1;
						pnl_mat_set(path, i, d, pnl_mat_get(path, i - 1, d) * expo);

					}
				}
			}
		}
		pnl_mat_free(&G);
		pnl_vect_free(&Ld);
		pnl_vect_free(&Gi);
		pnl_vect_free(&row);
	}
}


void BlackScholesModel::shiftAsset(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep){
    int i = ceil(t/timestep);
    pnl_mat_clone(shift_path, path);
    for (int k = i; k < path->m; ++k) {
        pnl_mat_set(shift_path,k,d, (1+h)*pnl_mat_get(path,k,d));
    }
}


void BlackScholesModel::simul_market(PnlMat *path, double T, int M, PnlRng *rng) {
	
	pnl_mat_resize(path, M + 1, size_);
	pnl_mat_set_row(path, spot_, 0);
	PnlMat* G = pnl_mat_create(M+1, size_);
	pnl_mat_rng_normal(G, M+1, size_, rng);

	double expo = 0;

	double timeSpan = T / M;

	PnlVect* Ld = pnl_vect_create(size_);
	PnlVect* Gi = pnl_vect_create(size_);

	for (int d = 0; d < size_; d++) {
		pnl_mat_get_row(Ld, corr, d);
		for (int i = 1; i < M+1; i++) {
			pnl_mat_get_row(Gi, G, i - 1);
			expo = pnl_expm1((GET(trend_, d) - pnl_pow_i(GET(sigma_, d), 2) / 2)  * i * timeSpan +
				GET(sigma_, d) * sqrt(timeSpan) * pnl_vect_scalar_prod(Ld, Gi)) + 1;
			pnl_mat_set(path, i, d, pnl_mat_get(path, 0, d) * expo);
		}
	}
	pnl_mat_free(&G);
	pnl_vect_free(&Ld);
	pnl_vect_free(&Gi);
}

void BlackScholesModel::getPast(PnlMat *past, PnlMat *path, double t, int n_time_steps,  double T)
{
	int H = path->m - 1;
	double timespan = T / n_time_steps;
	int dates_till_t = ceil(t / timespan) + 1;
	pnl_mat_resize(past, dates_till_t , size_);
	PnlVect *tmp = pnl_vect_create(size_);
	int index;
	for (int i = 0; i < dates_till_t-1; i++)
	{
		index = i * H / n_time_steps;
		pnl_mat_get_row(tmp, path, index);
		pnl_mat_set_row(past, tmp, i);
	}
	index = int(t*H / T);
	pnl_mat_get_row(tmp, path, index);
	pnl_mat_set_row(past, tmp, dates_till_t - 1);
	pnl_vect_free(&tmp);
}

