#include <iostream>
#include <cassert>
#include "KanjiBlackScholes.hpp"

KanjiBlackScholes::~KanjiBlackScholes()
{
	pnl_vect_free(&sigma_);
	pnl_vect_free(&spot_);
	pnl_mat_free(&corr);
	pnl_vect_free(&trend_);
	//pnl_mat_free(&corrMarket_);
}

KanjiBlackScholes::KanjiBlackScholes(int size, PnlVect * r, PnlMat * corrMarket, PnlVect * sigma, PnlVect * spot, PnlVect * trend)
{

	r_ = r;
	size_ = size;
	//corrMarket_ = corrMarket;
	corr = corrMarket;
	sigma_ = sigma;
	spot_ = spot;
	trend_ = trend;

	/*pnl_mat_clone(corr, corrMarket_);

	pnl_mat_set(corr, 0, 2, (pnl_mat_get(corrMarket_, 0, 2) * pnl_vect_get(sigma_, 2) + pnl_mat_get(corrMarket_, 0, 1) * pnl_vect_get(sigma_, 1)) / sqrt(pow(pnl_vect_get(sigma_, 2), 2) + pow(pnl_vect_get(sigma_, 1), 2) + 2 * pnl_vect_get(sigma_, 2) * pnl_vect_get(sigma_, 1) * pnl_mat_get(corrMarket_, 1, 2)));
	pnl_mat_set(corr, 2, 0, pnl_mat_get(corr, 0, 2));

	pnl_mat_set(corr, 0, 4, (pnl_mat_get(corrMarket_, 0, 4) * pnl_vect_get(sigma_, 4) + pnl_mat_get(corrMarket_, 0, 3) * pnl_vect_get(sigma_, 3)) / sqrt(pow(pnl_vect_get(sigma_, 4), 2) + pow(pnl_vect_get(sigma_, 3), 2) + 2 * pnl_vect_get(sigma_, 4) * pnl_vect_get(sigma_, 3) * pnl_mat_get(corrMarket_, 3, 4)));
	pnl_mat_set(corr, 4, 0, pnl_mat_get(corr, 0, 4));

	pnl_mat_set(corr, 1, 3, (pnl_mat_get(corrMarket_, 1, 2) * pnl_vect_get(sigma_, 2) + pnl_vect_get(sigma_, 1)) / sqrt(pow(pnl_vect_get(sigma_, 2), 2) + pow(pnl_vect_get(sigma_, 1), 2) + 2 * pnl_vect_get(sigma_, 2) * pnl_vect_get(sigma_, 1) * pnl_mat_get(corrMarket_, 1, 2)));
	pnl_mat_set(corr, 3, 1, pnl_mat_get(corr, 1, 3));

	pnl_mat_set(corr, 1, 4, (pnl_mat_get(corrMarket_, 1, 4) * pnl_vect_get(sigma_, 4) + pnl_mat_get(corrMarket_, 1, 3) * pnl_vect_get(sigma_, 3)) / sqrt(pow(pnl_vect_get(sigma_, 4), 2) + pow(pnl_vect_get(sigma_, 3), 2) + 2 * pnl_vect_get(sigma_, 4) * pnl_vect_get(sigma_, 3) * pnl_mat_get(corrMarket_, 3, 4)));
	pnl_mat_set(corr, 4, 1, pnl_mat_get(corr, 1, 4));

	pnl_mat_set(corr, 2, 3, (pnl_mat_get(corrMarket_, 2, 3) * pnl_vect_get(sigma_, 2) + pnl_mat_get(corrMarket_, 1, 3) * pnl_vect_get(sigma_, 1)) / sqrt(pow(pnl_vect_get(sigma_, 2), 2) + pow(pnl_vect_get(sigma_, 1), 2) + 2 * pnl_vect_get(sigma_, 2) * pnl_vect_get(sigma_, 1) * pnl_mat_get(corrMarket_, 1, 2)));
	pnl_mat_set(corr, 3, 2, pnl_mat_get(corr, 2, 3));

	pnl_mat_set(corr, 2, 4, (pnl_mat_get(corrMarket_, 2, 4) * pnl_vect_get(sigma, 2) * pnl_vect_get(sigma, 4) + pnl_mat_get(corrMarket_, 2, 3) * pnl_vect_get(sigma, 2) * pnl_vect_get(sigma, 3) + pnl_mat_get(corrMarket_, 1, 4) * pnl_vect_get(sigma, 1) * pnl_vect_get(sigma, 4) + pnl_mat_get(corrMarket_, 1, 3) * pnl_vect_get(sigma, 1) * pnl_vect_get(sigma, 3)) / (sqrt(pow(pnl_vect_get(sigma_, 2), 2) + pow(pnl_vect_get(sigma_, 1), 2) + 2 * pnl_vect_get(sigma_, 2) * pnl_vect_get(sigma_, 1) * pnl_mat_get(corrMarket_, 1, 2)) * sqrt(pow(pnl_vect_get(sigma_, 4), 2) + pow(pnl_vect_get(sigma_, 3), 2) + 2 * pnl_vect_get(sigma_, 4) * pnl_vect_get(sigma_, 3) * pnl_mat_get(corrMarket_, 3, 4))));
	pnl_mat_set(corr, 4, 2, pnl_mat_get(corr, 2, 4));

	pnl_mat_set(corr, 4, 3, (pnl_mat_get(corrMarket_, 3, 4) * pnl_vect_get(sigma_, 4) + pnl_vect_get(sigma_, 3)) / sqrt(pow(pnl_vect_get(sigma_, 4), 2) + pow(pnl_vect_get(sigma_, 3), 2) + 2 * pnl_vect_get(sigma_, 4) * pnl_vect_get(sigma_, 3) * pnl_mat_get(corrMarket_, 3, 4)));
	pnl_mat_set(corr, 3, 4, pnl_mat_get(corr, 4, 3));*/

	
	vols = pnl_vect_create_from_zero(size);
	pnl_vect_clone(vols, sigma_);
	pnl_vect_set(vols, 2, sqrt(pow(pnl_vect_get(sigma_, 2), 2) + pow(pnl_vect_get(sigma_, 1), 2) + 2 * pnl_vect_get(sigma_, 2) * pnl_vect_get(sigma_, 1) * pnl_mat_get(corr, 1, 2)));
	pnl_vect_set(vols, 4, sqrt(pow(pnl_vect_get(sigma_, 4), 2) + pow(pnl_vect_get(sigma_, 3), 2) + 2 * pnl_vect_get(sigma_, 4) * pnl_vect_get(sigma_, 3) * pnl_mat_get(corr, 3, 4)));

	tends = pnl_vect_create(size);
	pnl_vect_clone(tends, trend_);
	pnl_vect_set(tends, 1, pnl_vect_get(trend_, 1) + pnl_vect_get(r_, 1));
	pnl_vect_set(tends, 2, pnl_vect_get(trend_, 2) + pnl_vect_get(trend_, 1) + pnl_vect_get(sigma_, 2) * pnl_vect_get(sigma_, 1) * pnl_mat_get(corr, 1, 2));
	pnl_vect_set(tends, 3, pnl_vect_get(trend_, 3) + pnl_vect_get(r_, 2));
	pnl_vect_set(tends, 4, pnl_vect_get(trend_, 4) + pnl_vect_get(trend_, 3) + pnl_vect_get(sigma_, 4) * pnl_vect_get(sigma_, 3) * pnl_mat_get(corr, 3, 4));

	pnl_mat_chol(corr);

}

void KanjiBlackScholes::asset(PnlMat * path, double T, int nbTimeSteps, PnlRng * rng)
{


	pnl_mat_set_row(path, spot_, 0);

	PnlMat* G = pnl_mat_create(nbTimeSteps, size_);
	PnlVect* Ld = pnl_vect_create(size_);
	PnlVect* Gi = pnl_vect_create(size_);

	pnl_mat_rng_normal(G, nbTimeSteps, size_, rng);

	double expo = 0;

	double timeSpan = T / nbTimeSteps;

	for (int d = 0; d < size_; d++) {
		pnl_mat_get_row(Ld, corr, d);
		for (int i = 1; i <= nbTimeSteps; i++) {
			pnl_mat_get_row(Gi, G, i - 1);
			expo = pnl_expm1((pnl_vect_get(r_, 0) - pnl_pow_i(GET(vols, d), 2) / 2)  * timeSpan +
				GET(vols, d) * sqrt(timeSpan) * pnl_vect_scalar_prod(Ld, Gi)) + 1;
			pnl_mat_set(path, i, d, pnl_mat_get(path, i - 1, d) * expo);
		}
	}
	pnl_mat_free(&G);
	pnl_vect_free(&Ld);
	pnl_vect_free(&Gi);
}

void KanjiBlackScholes::asset(PnlMat * path, double t, double T, int nbTimeSteps, PnlRng * rng, const PnlMat * past)
{


	double timespan = T / nbTimeSteps;
	PnlMat *G;
	PnlVect *Ld = pnl_vect_create(size_);
	PnlVect *Gi = pnl_vect_create(size_);
	double expo;
	int i = int(t / timespan);
	double t_i = i * timespan;
	bool is_discrete_time = t == t_i;
	// extracting the past part to the path
	pnl_mat_extract_subblock(path, past, 0, i + 1, 0, size_);
	pnl_mat_resize(path, nbTimeSteps + 1, size_);
	G = pnl_mat_create(nbTimeSteps - i, size_);
	pnl_mat_rng_normal(G, nbTimeSteps - i, size_, rng);
	double sigma = 0;
	double s = 0;
	double x = 0;
	double past_s = 0;
	if (!is_discrete_time) {
		double delta_t = (i + 1)*timespan - t;
		pnl_mat_get_row(Gi, G, 0);
		for (int d = 0; d < size_; d++) {
			pnl_mat_get_row(Ld, corr, d);
			sigma = GET(vols, d);
			x = (pnl_vect_get(r_, 0) - 0.5*sigma * sigma) * delta_t + sigma * sqrt(delta_t)*pnl_vect_scalar_prod(Ld, Gi);
			expo = exp(x);
			past_s = pnl_mat_get(past, past->m - 1, d);
			s = past_s * expo;
			pnl_mat_set(path, i + 1, d, s);
		}
	}
	int offset = !is_discrete_time;
	// offset is 1 if t is not a discrete time so that we skip the first row after the past which was filled before
	for (int d = 0; d < size_; d++) {
		pnl_mat_get_row(Ld, corr, d);
		sigma = GET(vols, d);
		for (int k = i + 1 + offset; k <= nbTimeSteps; k++) {
			pnl_mat_get_row(Gi, G, k - i - 1);
			x = (pnl_vect_get(r_, 0) - 0.5*sigma*sigma) * timespan + sigma * sqrt(timespan)*pnl_vect_scalar_prod(Ld, Gi);
			expo = exp(x);
			past_s = pnl_mat_get(path, k - 1, d);
			s = past_s * expo;
			pnl_mat_set(path, k, d, s);
		}
	}
	pnl_mat_free(&G);
	pnl_vect_free(&Ld);
	pnl_vect_free(&Gi);
}

void KanjiBlackScholes::shiftAsset(PnlMat * shift_path, const PnlMat * path, int d, double h, double t, double timestep)
{
	int i = ceil(t / timestep);
	pnl_mat_clone(shift_path, path);
	for (int k = i; k < path->m; ++k) {
		pnl_mat_set(shift_path, k, d, (1 + h)*pnl_mat_get(path, k, d));
	}
}

void KanjiBlackScholes::simul_market(PnlMat * path, double T, int heg_dates_number, PnlRng * rng)
{
	pnl_mat_resize(path, heg_dates_number + 1, size_);
	pnl_mat_set_row(path, spot_, 0);

	PnlMat* G = pnl_mat_create(heg_dates_number, size_);
	pnl_mat_rng_normal(G, heg_dates_number, size_, rng);

	double expo = 0;

	double timeSpan = T / heg_dates_number;

	PnlVect* Ld = pnl_vect_create(size_);
	PnlVect* Gi = pnl_vect_create(size_);

	for (int d = 0; d < size_; d++) {
		pnl_mat_get_row(Ld, corr, d);
		for (int i = 1; i <= heg_dates_number; i++) {
			pnl_mat_get_row(Gi, G, i - 1);
			expo = pnl_expm1((GET(tends, d) - pnl_pow_i(GET(vols, d), 2) / 2)  * timeSpan +
				GET(vols, d) * sqrt(timeSpan) * pnl_vect_scalar_prod(Ld, Gi)) + 1;
			pnl_mat_set(path, i, d, pnl_mat_get(path, i - 1, d) * expo);
		}
	}
	pnl_vect_free(&Ld);
	pnl_vect_free(&Gi);
}

void KanjiBlackScholes::getPast(PnlMat * past, PnlMat * path, double t, int n_time_steps, double T)
{
	int H = path->m - 1;
	double timespan = T / n_time_steps;
	int dates_till_t = ceil(t / timespan) + 1;
	pnl_mat_resize(past, dates_till_t, size_);
	PnlVect *tmp = pnl_vect_create(size_);
	int index;
	for (int i = 0; i < dates_till_t - 1; i++)
	{
		index = i * H / n_time_steps;
		pnl_mat_get_row(tmp, path, index);
		pnl_mat_set_row(past, tmp, i);
	}
	index = std::round(t*H / T);
	pnl_mat_get_row(tmp, path, index);
	pnl_mat_set_row(past, tmp, dates_till_t - 1);
	pnl_vect_free(&tmp);
}




