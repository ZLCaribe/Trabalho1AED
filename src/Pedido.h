//
// Created by ZL on 18/10/2022.
//

#ifndef TRABALHO1AED_PEDIDO_H
#define TRABALHO1AED_PEDIDO_H

#include "Estudante.h"
#include "Alteracao.h"

namespace std {

    class Pedido {
    private{
            Estudante estudante;
            Alteracao alteracao;
        }
    public{
            Pedido(Estudante estuda, Alteracao aleracao);nte

            Estudante getEstudante;
            Alteracao getAlteracao;

            void setEstudante(Estudante estudante);
            void setAleracao(Alteracao alteracao);
        }


    };
} // std

#endif //TRABALHO1AED_PEDIDO_H
