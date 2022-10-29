#include "UCTurma.h"

using namespace std;

int UCTurma::MAX_ESTUDANTES = 25;

UCTurma::UCTurma(const string &codUC,const string &codTurma):codUC(codUC),codTurma(codTurma) {}

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

int UCTurma::getNEstudantes() const {
    return this->nEstudantes;
}

void UCTurma::operator++() {
    this->nEstudantes++;
}

bool UCTurma::operator==(const UCTurma& ucTurma) {
    return this->codUC == ucTurma.codUC && this->codTurma == ucTurma.codTurma;
}

UCTurma::UCTurma() {
    this->codUC = "";
    this->codTurma = "";
}
