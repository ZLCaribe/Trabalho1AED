//
// Created by ZL on 18/10/2022.
//

#ifndef TRABALHO1AED_SLOT_H
#define TRABALHO1AED_SLOT_H

enum DiaSemana{SEGUNDA, TERCA, QUARTA, QUINTA, SEXTA};

enum TipoAula{T, TP};

namespace std {

    class Slot {
    private{
            DiaSemana dia;
            float HoraInicio;
            float Duracao;
            TipoAula tipo;
        }

    public{
            Slot(DiaSemana dia, float HoraInicio, float Duracao, TipoAula tipo);

            DiaSemana getdia();
            float getHoraInicio();
            float getDuracao();
            TipoAula gettipo();

            void setdia(DiaSemana dia);
            void setHoraInicio(float HoraInicio);
            void setDuracao(float Duracao);
            void settipo(TipoAula tipo);
        }
    };

} // std

#endif //TRABALHO1AED_SLOT_H
