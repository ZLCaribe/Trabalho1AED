#include <iostream>
#include "Gestor.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Estudante.h"
#include "UCTurma.h"

using namespace std;
void Gestor::processarPedido() {
    Pedido pedidoAtual = this->pedidosFila.front();
    vector<Slot> novoHorario;
    switch (pedidoAtual.getTipoPedido()) {
        case REMOVER:
            pedidoAtual.getEstudante().rmUCTurma(pedidoAtual.getUCDesejadas().at(0));
            this->getTurmaH(pedidoAtual.getUCDesejadas().at(0)).operator--();
            break;
        case ADICIONAR:
        case ALTERAR:
        case ALTERARCONJ:
            if(checkDisponibilidadeTurmas(pedidoAtual.getUCDesejadas(),
                                          pedidoAtual.getTipoPedido(),
                                          pedidoAtual.getEstudante())
              && Gestor::compativel(this->novoHorario(pedidoAtual.getEstudante().getTurmas(),
                                                             pedidoAtual.getUCDesejadas()))){
                //TODO fazer troca ou adição (ZL)
            }else
                this->pedidosRejeitados.push_back(pedidoAtual);
            break;
    }
    this->pedidosFila.pop();
}

void Gestor::guardarPedido(const Pedido& pedido) {
    this->pedidosFila.push(pedido);
}

int Gestor::getUCTurma(const string& codUC, const string& codTurma) const{
    for(int i = 0; i < horario.size(); i++){
        if(this->horario[i].getCodTurma() == codTurma && this->horario[i].getCodUC() == codUC) {
            return i;
        }
    }
    return -1;
}

UCTurma Gestor::inputUCTurma() {
    string codUc, codTurma;
    int pos;
    while(true){
        cin >> codUc;
        cin >> codTurma;
        if((pos = this->getUCTurma(codUc,codTurma)) == -1)
            break;
        else
            cout << "Código da";
    }
    return this->horario.at(pos);
}

vector<string> explode(const string& s, const char& c){
    string buff;
    vector<string> v;

    for(auto n:s){
        if(n != c) buff+=n; else
        if(n == c && !buff.empty()) { v.push_back(buff); buff = ""; }
    }
    if(!buff.empty()) v.push_back(buff);

    return v;
}

void Gestor::addUC(){
    string linha;
    ifstream in("classes_per_uc.csv");

    getline(in,linha);
    while( getline( in, linha) ) {
        vector<string> v{explode(linha, ',')};
        TurmaH ucTurma(v[2], v[3]);
        this->horario.push_back(ucTurma);
    }
}
void Gestor::addHorario(){
    string linha;
    ifstream in("classes.csv");

    getline(in,linha);
    while( getline( in, linha) ) {
        vector<string> v{explode(linha, ',')};
        int j = getUCTurma(v[0],v[1]);
        DiaSemana dia = Slot::stringToDiaSemana(v[2]);
        TipoAula tipo = Slot::stringToTipo(v[5]);
        float hora = stof(v[3]);
        float duracao = stof(v[4]);
        Slot slot(dia,hora,duracao,tipo);
        this->horario[j].addSlot(slot);

    }
}

void Gestor::addEstudante(){
    string linha;
    ifstream in("students_classes.csv");

    getline(in,linha);
    while( getline( in, linha) ) {
        vector<string> v{explode(linha, ',')};
        Estudante estudante(v[0], v[1]);
        UCTurma ucTurma(v[2],v[3]);
        this->getTurmaH(ucTurma).operator++();
        auto i = this->estudantes.find(estudante);
        auto est = *i;
        if(i != this->estudantes.end()){
            est.addUCTurma(ucTurma);
        }
        else {
            estudante.addUCTurma(ucTurma);
            this->estudantes.insert(estudante);
        }
    }
}

bool Gestor::compativel(const vector<Slot>& novoHorario) {
    for(int i = 0; i < novoHorario.size() - 1; i++)
        for(int j = i + 1; j < novoHorario.size(); j++)
            if(novoHorario.at(i).conflito(novoHorario.at(j)))
                return false;
    return true;
}

vector<Slot> Gestor::novoHorario(const list<UCTurma>& turmas, vector<UCTurma> turmasNovas) const {
    vector<Slot> novoHorario;
    for(auto turma : turmas) {
        if(!turmasNovas.empty())
            for (auto turmaNova = turmasNovas.begin(); turmaNova != turmasNovas.end(); turmaNova++)
                if (turma.operator==(*turmaNova)) {
                    for (auto slot: this->getHorariosDeTurma(turma))
                        if (slot.gettipo() != T) { novoHorario.push_back(slot); }
                    turmasNovas.erase(turmaNova);
                    break;
                }
        for(auto slot : this->getHorariosDeTurma(turma))
            if(slot.gettipo() != T)
                novoHorario.push_back(slot);
    }
    return novoHorario;
}

list<Slot> Gestor::getHorariosDeTurma(const UCTurma& turma) const {
    return this->getTurmaH(turma).getHoraUCTurma();
}

bool Gestor::checkDisponibilidadeTurmas(const vector<UCTurma>& turmasPedidas, TipoPedido tipoPedido, Estudante estudante) {
    for(const auto& turmaPedida : turmasPedidas){
        auto turmas = this->getTurmasByUC(turmaPedida.getCodUC());
        if(turmas.front().getNEstudantes() >= UCTurma::MAX_ESTUDANTES) return false;
        int min = turmas.front().getNEstudantes();
        for(const auto& turma : turmas){
            int nEstudantes = turma.getNEstudantes();
            if(tipoPedido != ADICIONAR)
                if(!estudante.getTurmaByUC(turma.getCodUC()).getCodUC().empty())
                    nEstudantes--;
            if(nEstudantes < min)
                min = turma.getNEstudantes();
        }
        if(this->getNEstudantesTurma(turmaPedida) - min >= 4)
            return false;
    }
    return true;
}

list<TurmaH> Gestor::getTurmasByUC(const string& codUC) {
    list<TurmaH> turmas;
    for(const auto& turma : this->horario){
        if(turma.getCodUC() == codUC)
            turmas.push_back(turma);
    }
    return turmas;
}

int Gestor::getNEstudantesTurma(const UCTurma& ucTurma) const {
    return this->getTurmaH(ucTurma).getNEstudantes();
}

TurmaH Gestor::getTurmaH(const UCTurma& ucTurma) const{
    for(auto turma : this->horario)
        if(turma.getCodUC() == ucTurma.getCodUC() && turma.getCodTurma() == ucTurma.getCodTurma())
            return turma;
}
