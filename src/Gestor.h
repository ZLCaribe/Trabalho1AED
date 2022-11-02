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
        int getUCTurma(const string& codUC, const string& codTurma);
        list<Slot> getHorariosDeTurma(UCTurma turma);
        int getEstudante(const string& codEst, const string& nomeEst);
        void guardarPedido(const Pedido& pedido);
        void processarPedido();
        void addUC();
        void addHorario();
        void addEstudante();
        UCTurma inputUCTurma();
        static bool compativel(const vector<Slot>& novoHorario);
        vector<Slot> gerarNovoHorario(const list<UCTurma>& turmas,const vector<UCTurma>& turmasNovas);
};


#endif //TRABALHO1AED_GESTOR_H
