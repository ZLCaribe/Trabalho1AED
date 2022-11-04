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
        Estudante();

        string getCodEst() const;
        string getNomeEst() const;
        UCTurma getTurmaByUC(const string& codUC);
        list <UCTurma> getTurmas() const;


        void setCodEst(string CodEst);
        void setNomeEst(string nomeEst);
        string estudanteToString();

        void addUCTurma(const UCTurma& ucTurma);
        _List_iterator<UCTurma> rmUCTurma(const UCTurma& ucTurma);
        void switchTurmas(const vector<UCTurma>& turmasNovas);

        bool operator<(const Estudante& estudante) const;
        bool operator==(const Estudante& estudante) const;
};

#endif //TRABALHO1AED_ESTUDANTE_H
