#ifndef TRABALHO1AED_TURMAH_H
#define TRABALHO1AED_TURMAH_H

#include <string>
#include <list>
#include "Slot.h"
#include "UCTurma.h"

using namespace std;

class TurmaH: public UCTurma{
    private:
        string codUC;
        string codTurma;
        list<Slot> horarios;
        int nEstudantes = 0;
    public:
        TurmaH(const string &codUC,const string &codTurma);

        list<Slot>& getHoraUCTurma();
        int getNEstudantes() const;

        void addSlot(Slot horario);

        void operator++();
};

#endif //TRABALHO1AED_TURMAH_H
