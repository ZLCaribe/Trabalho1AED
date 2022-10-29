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
        int getUCTurma(string codUC, string codTurma);
        void guardarPedido(const Estudante &estudante,TipoPedido tipoPedido);
        void processarPedido();
        UCTurma inputUCTurma();
};


#endif //TRABALHO1AED_GESTOR_H
