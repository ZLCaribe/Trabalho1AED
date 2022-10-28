#include "UCTurma.h"

using namespace std;

UCTurma::UCTurma(string &codUC,string &codTurma):codUC(codUC),codTurma(codTurma) {}

string& UCTurma::getCodUC() {
    return this->codUC;
}

string& UCTurma::getCodTurma() {
    return this->codTurma;
}

void UCTurma::setCodUC(const string &codUC) {
    this->codUC = codUC;
}

void UCTurma::setCodTurma(const string &codTurma) {
    this->codTurma = codTurma;
}
