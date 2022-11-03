#ifndef TRABALHO1AED_GESTOR_H
#define TRABALHO1AED_GESTOR_H

#include <set>
#include <vector>
#include <queue>
#include "Estudante.h"
#include "TurmaH.h"
#include "Pedido.h"

using namespace std;

class Gestor {
    private:
        set<Estudante> estudantes;
        vector<TurmaH> horario;
        queue<Pedido> pedidosFila;
        vector<Pedido> pedidosRejeitados;
    public:
        int getUCTurma(const string& codUC, const string& codTurma) const;
        TurmaH getTurmaH(const UCTurma& ucTurma) const;
        list<Slot> getHorariosDeTurma(const UCTurma& turma) const;
        int getNEstudantesTurma(const UCTurma& ucTurma) const;
        list<TurmaH> getTurmasByUC(const string& codUC);

        void guardarPedido(const Pedido& pedido);
        void processarPedido();

        void addUC();
        void addHorario();
        void addEstudante();

        UCTurma inputUCTurma();

        static bool compativel(const vector<Slot>& novoHorario);
        bool checkDisponibilidadeTurmas(const vector<UCTurma>& turmas, TipoPedido tipoPedido, Estudante estudante);
        vector<Slot> novoHorario(const list<UCTurma>& turmas, vector<UCTurma> turmasNovas) const;
};


#endif //TRABALHO1AED_GESTOR_H
