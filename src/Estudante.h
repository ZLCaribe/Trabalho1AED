//
// Created by ZL on 18/10/2022.
//

#ifndef TRABALHO1AED_ESTUDANTE_H
#define TRABALHO1AED_ESTUDANTE_H

#include "string"
#include <list>
#include "UCTurma.h"

namespace std {

    class Estudante {
        private{
            string codEst;
            string nomeEst;
            list <UCTurma> turmas;
        }

        public{
            Estudante(CodEst, string nomeEst, list <UCTurma> turmas);

            string getCodEst();
            string getNomeEst();
            list <UCTurma> getTurmas();


            void setCodEst(CodEst);
            void

        }
    };

} // std

#endif //TRABALHO1AED_ESTUDANTE_H
