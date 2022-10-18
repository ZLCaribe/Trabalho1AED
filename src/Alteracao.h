#ifndef TRABALHO1AED_ALTERACAO_H
#define TRABALHO1AED_ALTERACAO_H

#include "UCTurma.h"

namespace std {

    class Alteracao {
        private{
            UCTurma atual;
            UCTurma pretendida;
        }
        public{
            Alteracao (UCTurma atual, UCTurma pretendida);

            UCTurma getAtual();
            UCTurma getPretendida();

            void UCTurma setAtual(UCTurma atual);
            void UCTurma setPretendida(UCTurma pretendida);
            
        }
    };

} // std

#endif //TRABALHO1AED_ALTERACAO_H
