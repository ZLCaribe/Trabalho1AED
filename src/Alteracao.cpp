#include "Alteracao.h"

namespace std {
    Alteracao::Alteracao (UCTurma atual, UCTurma pretendida){
        this->atual = atual;
        this->pretendida = pretendida;
    }

    UCTurma Alteracao::getAtual(){return atual;}
    UCTurma Alteracao::getPretendida(){return pretendida;}

    void UCTurma Alteracao::setAtual(UCTurma atual){
        this->atual = atual;
    }

    void UCTurma Alteracao::setPretendida(UCTurma pretendida){
        this->pretendida = pretendida;
    }

} // std