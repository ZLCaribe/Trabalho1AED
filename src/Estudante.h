#ifndef TRABALHO1AED_ESTUDANTE_H
#define TRABALHO1AED_ESTUDANTE_H

#include "string"
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

        string getCodEst();
        string getNomeEst();
        UCTurma getTurmaByUC(const string& codUC);
        list <UCTurma> getTurmas() const;


        void setCodEst(string CodEst);
        void setNomeEst(string nomeEst);
        string estudanteToString(Estudante& estudante);

        void addUCTurma(const UCTurma& ucTurma);
        void rmUCTurma(const UCTurma& ucTurma);

        bool operator<(const Estudante& estudante) const;
        bool operator==(const Estudante& estudante) const;
};

#endif //TRABALHO1AED_ESTUDANTE_H
