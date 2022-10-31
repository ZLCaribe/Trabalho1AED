#ifndef TRABALHO1AED_SLOT_H
#define TRABALHO1AED_SLOT_H

#include <string>

enum DiaSemana{SEGUNDA, TERCA, QUARTA, QUINTA, SEXTA};

enum TipoAula{T, TP};

using namespace std;

class Slot {
    private:
        DiaSemana dia;
        float horaInicio;
        float duracao;
        TipoAula tipo;
    public:
        Slot(DiaSemana dia, float horaInicio, float duracao, TipoAula tipo);

        DiaSemana getdia();
        float getHoraInicio() const;
        float getDuracao() const;
        TipoAula gettipo();

        void setdia(DiaSemana dia);
        void setHoraInicio(float horaInicio);
        void setDuracao(float duracao);
        void settipo(TipoAula tipo);
        static DiaSemana stringToDiaSemana(string dia);
        static TipoAula stringToTipo(string tipo);
};

#endif //TRABALHO1AED_SLOT_H
