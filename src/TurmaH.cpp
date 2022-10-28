#include "TurmaH.h"

using namespace std;

TurmaH::TurmaH(const string &codUC,const string &codTurma): UCTurma(codUC,codTurma){}

list<Slot>& TurmaH::getHoraUCTurma() {
    return this->horarios;
}

void TurmaH::addSlot(Slot horario) {
    this->horarios.push_back(horario);
}
