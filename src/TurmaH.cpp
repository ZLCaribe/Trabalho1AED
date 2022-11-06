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

/**
 * aumenta o contador do numero de estudantes numa turma
 * @complexity O(1)
 */
void TurmaH::operator++() {
    this->nEstudantes++;
}

/**
 * diminui o contador do numero de estudantes numa turma.
 * @complexity O(1)
 */
void TurmaH::operator--() {
    this->nEstudantes--;
}

/**
 * compara o numero de estudantes em duas turmas.
 * @param turma1 primeira turma a comparar
 * @param turma2 segunda turma a comparar
 * @return se a primeira turma tiver mais estudantes do que a segunda retorna True e Fals caso seja ao contrário.
 * @complexity O(1)
 */
bool TurmaH::compararNumEstudante(TurmaH turma1, TurmaH turma2) {
    return turma1.nEstudantes > turma2.nEstudantes;
}

TurmaH::TurmaH(): UCTurma("","") {}
