using namespace std;

#include "pnl/pnl_matrix.h"
/// \brief Classe qui lit les données du marché
class DataMarketReader {

public:
    /**
* Genere une matrice des prix des sous-jacents à partir des données du marché
*
* @param[in] infile fichier qui contient les données du marché
*/
    PnlMat* readMarketData(char* infile); /// nombre de lignes de la matrice des spots
    int  m_; /// nombre de colonnes de la matrice des spots
    int n_;

};

