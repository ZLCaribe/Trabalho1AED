#include "UCTurma.h"

using namespace std;

UCTurma::UCTurma(string codUC, string codTurma) {
    this->codUC = codUC;
    this->codTurma = codTurma;
}

string UCTurma::getCodUC() {
    return this->codUC;
}

string UCTurma::getCodTurma() {
    return this->codTurma;
}

void UCTurma::setCodUC(string codUC) {
    this->codUC = codUC;
}

void UCTurma::setCodTurma(string codTurma) {
    this->codTurma = codTurma;
}
