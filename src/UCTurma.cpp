#include "UCTurma.h"

#include <utility>

using namespace std;

int UCTurma::MAX_ESTUDANTES = 25;

UCTurma::UCTurma(string codUC,string codTurma):codUC(std::move(codUC)),codTurma(std::move(codTurma)) {}

string UCTurma::getCodUC() const{
    return this->codUC.substr(0,this->codUC.length());
}

string UCTurma::getCodTurma() const{
    return this->codTurma.substr(0,this->codTurma.length());
}

void UCTurma::setCodUC(const string &codUC) {
    this->codUC = codUC;
}

void UCTurma::setCodTurma(const string &codTurma) {
    this->codTurma = codTurma;
}

bool UCTurma::operator==(const UCTurma& ucTurma) {
    return this->codUC == ucTurma.codUC && this->codTurma == ucTurma.codTurma;
}

UCTurma::UCTurma() {
    this->codUC = "";
    this->codTurma = "";
}
