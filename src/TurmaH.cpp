#include "TurmaH.h"

using namespace std;

TurmaH::TurmaH(const string &codUC,const string &codTurma): UCTurma(codUC,codTurma){}

list<Slot>& TurmaH::getHoraUCTurma() {
    return this->horarios;
}

void TurmaH::addSlot(Slot horario) {
    this->horarios.push_back(horario);
}

int TurmaH::getNEstudantes() const {
    return this->nEstudantes;
}

void TurmaH::operator++() {
    this->nEstudantes++;
}

void TurmaH::operator--() {
    this->nEstudantes--;
}

bool TurmaH::compararNumEstudante(TurmaH turma1, TurmaH turma2) {
    return turma1.nEstudantes > turma2.nEstudantes;
}
TurmaH::TurmaH(): UCTurma("","") {}


