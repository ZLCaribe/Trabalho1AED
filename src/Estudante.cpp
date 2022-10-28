#include "Estudante.h"

using namespace std;

Estudante::Estudante(string &codEst, string &nomeEst):codEst(codEst),nomeEst(nomeEst){}

string Estudante::getCodEst(){
    return this->codEst;
}

string Estudante::getNomeEst(){
    return this->nomeEst;
}

list <UCTurma> Estudante::getTurmas(){
    return this->turmas;
}

void Estudante::setCodEst(string codEst){
    this->codEst = codEst;
}

void Estudante::setNomeEst(string nomeEst){
    this->nomeEst = nomeEst;
}

void Estudante::addUCTurma(UCTurma turma){
    this->turmas.push_back(turma);
}