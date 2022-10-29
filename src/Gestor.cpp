#include <iostream>
#include "Gestor.h"

using namespace std;

bool compativel(const Estudante &estudante){

    return true;
}

void Gestor::processarPedido() {
    Pedido pedidoAtual = this->pedidosFila.front();
    Estudante estudante(" "," ");
    switch (pedidoAtual.getTipoPedido()) {
        case REMOVER:
            pedidoAtual.getEstudante().rmUCTurma(pedidoAtual.getUCDesejadas().at(0));
            break;
        case ADICIONAR:
            break;
        case ALTERAR:
            break;
        case ALTERARCONJ:
            break;
    }
}

void Gestor::guardarPedido(const Estudante &estudante,TipoPedido tipoPedido) {
    Pedido pedido(estudante,tipoPedido);
    string codUC,codTurma;
    UCTurma ucTurma;
    int pos = -1;
    switch (tipoPedido) {
        case REMOVER:
            do {
                cin >> codTurma;
                cin >> codUC;
            } while ((pos = getUCTurma(codUC,codTurma)) == -1);
            pedido.addUCDesejada(this->horario.at(pos));
            break;
        case ADICIONAR:

            break;
        case ALTERAR:
            break;
        case ALTERARCONJ:
            break;
    }
}

int Gestor::getUCTurma(string codUC, string codTurma) {
    return -1;
}

UCTurma Gestor::inputUCTurma() {
    string codUc, codTurma;
    int pos = -1;
    while(true){
        cin >> codUc;
        cin >> codTurma;
        if((pos = this->getUCTurma(codUc,codTurma)) == -1)
            break;
        else
            cout << "Código da"
    }
    return this->horario.at(pos);
}
