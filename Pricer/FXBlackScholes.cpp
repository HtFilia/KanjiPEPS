#include "FXBlackScholes.hpp"
FXBlackScholes::~FXBlackScholes() {
	pnl_vect_free(&vect_r);
}