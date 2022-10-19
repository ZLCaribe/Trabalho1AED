#ifndef TRABALHO1AED_ALTERACAO_H
#define TRABALHO1AED_ALTERACAO_H

#include "UCTurma.h"

using namespace std;

class Alteracao {
    private:
        UCTurma atual;
        UCTurma pretendida;
    public:
        Alteracao (UCTurma atual, UCTurma pretendida);

        UCTurma getAtual();
        UCTurma getPretendida();

        void setAtual(UCTurma atual);
        void setPretendida(UCTurma pretendida);
};

#endif //TRABALHO1AED_ALTERACAO_H
