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
    set<Estudante> estudantes;
    vector<TurmaH> horario;
    queue<Pedido> pedidos;
};


#endif //TRABALHO1AED_GESTOR_H
