//
// Created by ZL on 18/10/2022.
//

#ifndef TRABALHO1AED_PEDIDO_H
#define TRABALHO1AED_PEDIDO_H

#include "Estudante.h"
#include "Alteracao.h"

using namespace std;

class Pedido {
    private:
        Estudante estudante;
        Alteracao alteracao;
    public:
        Pedido(Estudante estudante, Alteracao alteracao);

        Estudante getEstudante();
        Alteracao getAlteracao();

        void setEstudante(Estudante estudante);
        void setAlteracao(Alteracao alteracao);
};

#endif //TRABALHO1AED_PEDIDO_H
