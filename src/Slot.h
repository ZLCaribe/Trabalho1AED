#ifndef TRABALHO1AED_SLOT_H
#define TRABALHO1AED_SLOT_H

#include <string>

enum DiaSemana{SEGUNDA, TERCA, QUARTA, QUINTA, SEXTA};

enum TipoAula{T, TP, PL};

using namespace std;

class Slot {
    private:
        DiaSemana dia;
        float horaInicio;
        float duracao;
        TipoAula tipo;
    public:
        Slot(DiaSemana dia, float horaInicio, float duracao, TipoAula tipo);
        Slot();

        DiaSemana getdia();
        float getHoraInicio() const;
        float getDuracao() const;
        TipoAula gettipo();

        void setdia(DiaSemana dia);
        void setHoraInicio(float horaInicio);
        void setDuracao(float duracao);
        void settipo(TipoAula tipo);

        static DiaSemana stringToDiaSemana(const string& dia);
        static TipoAula stringToTipo(const string& tipo);
        static string DiaSemanaToString(const DiaSemana& dia);
        static string tipoToString(const TipoAula& tipo);

        bool conflito(Slot slot) const;
};

#endif //TRABALHO1AED_SLOT_H
