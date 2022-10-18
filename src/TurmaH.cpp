#include "TurmaH.h"

using namespace std;

TurmaH::TurmaH(string codUC, string codTurma) {
    this->codUC = codUC;
    this->codTurma = codTurma;
}

string TurmaH::getCodUC() {
    return this->codUC;
}

string TurmaH::getCodTurma() {
    return this->codTurma;
}

list<Slot> TurmaH::getHoraUCTurma() {
    return this->horarios;
}

void TurmaH::setCodUC(string codUC) {
    this->codUC = codUC;
}

void TurmaH::setCodTurma(string codTurma) {
    this->codTurma = codTurma;
}

void TurmaH::addSlot(Slot horario) {
    this->horarios.push_back(horario);
}
