#include "DataMarketReader.h"
#include <fstream>
#include <vector>
#include <pnl/pnl_matvect.h>
#include <pnl/pnl_matrix.h>
#include <iostream>
#include <string> 

vector<string> split(string str, string token) {
    vector<string>result;
    while (str.size()) {
        int index = str.find(token);
        if (index != string::npos) {
            result.push_back(str.substr(0, index));
            str = str.substr(index + token.size());
            if (str.size() == 0)result.push_back(str);
        }
        else {
            result.push_back(str);
            str = "";
        }
    }
    return result;
}
PnlMat* DataMarketReader::readMarketData(char* infile) {
    ifstream file(infile);
    string str;
    vector<string> line;
    int compteur = 0;
    PnlMat* path  = pnl_mat_create(0, 0);
    while (std::getline(file, str)) {
        line = split(str, " ");
        if (line[0] == "#") {
            if (line[1] == "number" && line[2] == "of" && line[3] == "dates:") {
                m_ = (int)stod(line[4]);
                path = pnl_mat_create(m_, n_);
            }
            else if (line[1] == "model" && line[2] == "size:") {
                n_ = (int)stod(line[3]);
            }
        }
        else {
            for (int j = 0; j < 3; j++)
                if(compteur <= 1905)
                    pnl_mat_set(path, compteur, j, stod(line[j]));
            compteur++;
        }

    }
    pnl_mat_resize(path, 1905, n_);
    return path;
}