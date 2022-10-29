#ifndef TRABALHO1AED_PEDIDO_H
#define TRABALHO1AED_PEDIDO_H

#include <vector>
#include "Estudante.h"
#include "Alteracao.h"

using namespace std;

enum TipoPedido{ADICIONAR,REMOVER,ALTERAR,ALTERARCONJ};

class Pedido {
    private:
        Estudante estudante;
        vector<UCTurma> ucTurmaDesejadas;
        TipoPedido tipoPedido;
    public:
        Pedido(const Estudante &estudante, TipoPedido tipoPedido);

        Estudante &getEstudante();
        vector<UCTurma> &getUCDesejadas();
        TipoPedido getTipoPedido();

        void setEstudante(const Estudante &estudante);
        void addUCDesejada(const UCTurma &ucTurmaDesejada);
        void setTipoPedido(TipoPedido tipoPedido);
};

#endif //TRABALHO1AED_PEDIDO_H
