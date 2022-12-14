#ifndef TRABALHO1AED_ESTUDANTE_H
#define TRABALHO1AED_ESTUDANTE_H

#include <list>
#include <vector>
#include "UCTurma.h"

using namespace std;

class Estudante {
    private:
        string codEst;
        string nomeEst;
        list <UCTurma> turmas;

    public:
        Estudante(string codEst, string nomeEst);
        Estudante();

        string getCodEst() const;
        string getNomeEst() const;
        UCTurma getTurmaByUC(const string& codUC);
        list <UCTurma> getTurmas() const;

        string estudanteToString();

        void addUCTurma(const UCTurma& ucTurma);
        void rmUCTurma(const UCTurma& ucTurma);

        bool operator<(const Estudante& estudante) const;
        bool operator==(const Estudante& estudante) const;
};

#endif //TRABALHO1AED_ESTUDANTE_H
