#include "Estudante.h"

#include <utility>

using namespace std;

Estudante::Estudante(string codEst, string nomeEst):codEst(std::move(codEst)),nomeEst(std::move(nomeEst)){}

string Estudante::getCodEst() const {
    return this->codEst;
}

string Estudante::getNomeEst() const {
    return this->nomeEst;
}

list <UCTurma> Estudante::getTurmas() const {
    return this->turmas;
}

void Estudante::setCodEst(string codEst){
    this->codEst = std::move(codEst);
}

void Estudante::setNomeEst(string nomeEst){
    this->nomeEst = std::move(nomeEst);
}

void Estudante::addUCTurma(const UCTurma& ucTurma) {
    this->turmas.push_back(ucTurma);
}

/**
 * Rremove a turma da lista de turmas do estudante.
 * @param ucTurma turma a remover
 */
void Estudante::rmUCTurma(const UCTurma& ucTurma) {
    for(auto it = this->turmas.begin(); it != this->turmas.end();it++) {
        if (it->operator==(ucTurma)) {
            this->turmas.erase(it);
            break;
        }
    }
}

bool Estudante::operator<(const Estudante& estudante) const{
    return this->codEst < estudante.codEst;
}

bool Estudante::operator==(const Estudante& estudante) const{
    return this->codEst == estudante.codEst;
}

/**
 * Quando a função recebe um codigo de uma UC, esta devolve a turma do estudante naquela UC
 * @param codUC codigo da UC que queremos encontrar a turma
 * @return retorna a Turma do aluno naquela UC
 */
UCTurma Estudante::getTurmaByUC(const string& codUC) {
    for(auto ucTurma : this->turmas)
        if(ucTurma.getCodUC() == codUC)
            return ucTurma;
    return {};
}
string Estudante::estudanteToString(){
    return "codigo: " + this->codEst + "nome: " + this->nomeEst;
}

Estudante::Estudante() {
    this->codEst = "";
    this->nomeEst = "";
}

