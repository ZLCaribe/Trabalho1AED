#include "Pedido.h"

using namespace std;

Pedido::Pedido(Estudante estudante, Alteracao alteracao) {
    this -> estudante = estudante;
    this -> alteracao = alteracao;
}

Estudante Pedido::getEstudante() {
    return this -> estudante;
}

Alteracao Pedido::getAlteracao() {
    return this -> alteracao;
}

void Pedido::setEstudante(Estudante estudante) {
    this -> estudante = estudante;
}

void Pedido::setAlteracao(Alteracao alteracao) {
    this -> alteracao = alteracao;
}
