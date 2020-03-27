using namespace std;

#include "pnl/pnl_matrix.h"
/// \brief Classe qui lit les donn�es du march�
class DataMarketReader {

public:
    /**
* Genere une matrice des prix des sous-jacents � partir des donn�es du march�
*
* @param[in] infile fichier qui contient les donn�es du march�
*/
    PnlMat* readMarketData(char* infile); /// nombre de lignes de la matrice des spots
    int  m_; /// nombre de colonnes de la matrice des spots
    int n_;

};

