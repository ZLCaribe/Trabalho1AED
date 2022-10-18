//
// Created by ZL on 18/10/2022.
//

#ifndef TRABALHO1AED_SLOT_H
#define TRABALHO1AED_SLOT_H

enum DiaSemana{SEGUNDA, TERCA, QUARTA, QUINTA, SEXTA};

enum TipoAula{T, TP};

namespace std {

    class Slot {
        DiaSemana dia;
        float HoraInicio;
        float Duracao;
        TipoAula tipo;
    };

} // std

#endif //TRABALHO1AED_SLOT_H
