#ifndef TRABALHO1AED_UCTURMA_H
#define TRABALHO1AED_UCTURMA_H

#include "string"

using namespace std;

class UCTurma {
    private:
        string codUC;
        string codTurma;
    public:
        UCTurma(string codUC, string codTurma);

        string getCodUC();
        string getCodTurma();

        void setCodUC(string codUC);
        void setCodTurma(string codTurma);
};

#endif //TRABALHO1AED_UCTURMA_H
