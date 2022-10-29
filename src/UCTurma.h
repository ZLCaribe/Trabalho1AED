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
        static int MAX_ESTUDANTES;
        UCTurma();
        UCTurma(const string &codUC, const string &codTurma);

        string &getCodUC();
        string &getCodTurma();
        int getNEstudantes() const;

        void setCodUC(const string &codUC);
        void setCodTurma(const string &codTurma);

        bool operator==(const UCTurma& ucTurma);
        void operator++();
};

#endif //TRABALHO1AED_UCTURMA_H
