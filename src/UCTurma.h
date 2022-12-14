#ifndef TRABALHO1AED_UCTURMA_H
#define TRABALHO1AED_UCTURMA_H

#include <utility>
#include <string>

using namespace std;

class UCTurma {
    private:
        string codUC;
        string codTurma;
    public:
        static int MAX_ESTUDANTES;
        UCTurma();
        UCTurma(string codUC, string codTurma);

        string getCodUC()const;
        string getCodTurma()const;

        void setCodUC(const string &codUC);
        void setCodTurma(const string &codTurma);

        bool operator==(const UCTurma& ucTurma);
};

#endif //TRABALHO1AED_UCTURMA_H
