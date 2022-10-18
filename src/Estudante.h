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
        string codEst;
        string nomeEst;
        list<UCTurma> turmas;
    };

} // std

#endif //TRABALHO1AED_ESTUDANTE_H
