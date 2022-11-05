#include "Pedido.h"

using namespace std;

Pedido::Pedido(const Estudante &estudante, TipoPedido tipoPedido):
        estudante(const_cast<Estudante &>(estudante)), tipoPedido(tipoPedido) {}

Estudante &Pedido::getEstudante() {
    return this->estudante;
}

vector<UCTurma> &Pedido::getUCDesejadas() {
    return this -> ucTurmaDesejadas;
}

void Pedido::setEstudante(const Estudante &estudante) {
    this -> estudante = estudante;
}

void Pedido::addUCDesejada(const UCTurma &ucTurmaDesejada) {
    this -> ucTurmaDesejadas.push_back(ucTurmaDesejada);
}

TipoPedido Pedido::getTipoPedido() {
    return this->tipoPedido;
}

void Pedido::setTipoPedido(TipoPedido tipoPedido) {
    this->tipoPedido = tipoPedido;
}
