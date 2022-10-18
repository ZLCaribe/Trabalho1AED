#include "Pedido.h"

using namespace std;

Pedido::Pedido(Estudante estudante, Alteracao aleracao) {
    this -> Estudante = estudante;
    this -> Aleracao = alteracao;
}

Estudante Pedido::getEstudante() {
    return this -> estudante;
}

Alteracao Pedido::getAleracao() {
    return this -> alteracao;
}

void Pedido::setEstudante(Estudante estudante) {
    this -> estudante = estudante;
}

void Pedido::setAlteracao(Alteracao alteracao) {
    this -> elteracao = alteracao;
}
