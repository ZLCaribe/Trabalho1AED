#ifndef TRABALHO1AED_UCTURMA_H
#define TRABALHO1AED_UCTURMA_H

#include "string"

using namespace std;

class UCTurma {
    private:
        string codUC;
        string codTurma;
        int nEstudantes = 0;
    public:
        UCTurma(string &codUC, string &codTurma);

        string &getCodUC();
        string &getCodTurma();

        void setCodUC(const string &codUC);
        void setCodTurma(const string &codTurma);
};

#endif //TRABALHO1AED_UCTURMA_H
