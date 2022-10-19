#include "Alteracao.h"

using namespace std;

Alteracao::Alteracao (UCTurma atual, UCTurma pretendida){
    this->atual = atual;
    this->pretendida = pretendida;
}

UCTurma Alteracao::getAtual(){return this->atual;}
UCTurma Alteracao::getPretendida(){return this->pretendida;}

void Alteracao::setAtual(UCTurma atual){
    this->atual = atual;
}

void Alteracao::setPretendida(UCTurma pretendida){
    this->pretendida = pretendida;
}