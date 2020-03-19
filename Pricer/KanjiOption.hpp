#ifndef MC_PRICER_PERFORMANCEOPTION_H
#define MC_PRICER_PERFORMANCEOPTION_H

#include "Option.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
/// \brief Classe de l'option performance

class KanjiOption : public Option {

public:
	PnlVect *initial_values;
	double netAssetValue_;

	~KanjiOption();
	/**
* Constructeur de l'option performance
*
* @param[in] T maturité de l'option
* @param[in] nbTimeSteps  nombre de jours de simulation
* @param[in] size nombre des sous-jacents
 * @param[in] payOffCoeffs vecteur des coefficients de payoffs
*/
	KanjiOption(double T, int nbTimeSteps, int size, PnlVect* values, double netAssetValue = 100) {
		T_ = T;
		nbTimeSteps_ = nbTimeSteps;
		size_ = size;
		initial_values = pnl_vect_copy(values);
		type_ = kanji;
		netAssetValue_ = netAssetValue;
	}

    /**
* Payoff de l'option performance à partir du marché
*
* @param[in] path matrices des trajectoires des sous-jacents
*/
	double payoff(const PnlMat* path);

};

#endif //MC_PRICER_PERFORMANCEOPTION_H