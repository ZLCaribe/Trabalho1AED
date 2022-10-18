#ifndef TRABALHO1AED_TURMAH_H
#define TRABALHO1AED_TURMAH_H

#include <string>
#include <list>
#include "Slot.h"

using namespace std;

    class TurmaH {
        private:
            string codUC;
            string codTurma;
            list<Slot> horarios;

        public:
            TurmaH(string codUC, string codTurma);

            string getCodUC();
            string getCodTurma();
            list<Slot> getHoraUCTurma();

            void setCodUC(string codUC);
            void setCodTurma(string codTurma);

            void addSlot(Slot horario);
    };

#endif //TRABALHO1AED_TURMAH_H
