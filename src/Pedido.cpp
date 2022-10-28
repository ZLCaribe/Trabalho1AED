#include "Pedido.h"

using namespace std;

Pedido::Pedido(Estudante &estudante,Alteracao &alteracao): estudante(estudante), alteracao(alteracao) {}

Estudante &Pedido::getEstudante() {
    return this -> estudante;
}

Alteracao &Pedido::getAlteracao() {
    return this -> alteracao;
}

void Pedido::setEstudante(const Estudante &estudante) {
    this -> estudante = estudante;
}

void Pedido::setAlteracao(const Alteracao &alteracao) {
    this -> alteracao = alteracao;
}
