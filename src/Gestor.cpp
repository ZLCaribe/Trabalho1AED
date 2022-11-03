#include <iostream>
#include "Gestor.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Estudante.h"
#include "UCTurma.h"

using namespace std;
//TODO zl
void Gestor::processarPedido() {
    Pedido pedidoAtual = this->pedidosFila.front();
    vector<Slot> novoHorario;
    switch (pedidoAtual.getTipoPedido()) {
        case REMOVER:
            pedidoAtual.getEstudante().rmUCTurma(pedidoAtual.getUCDesejadas().at(0));
            break;
        case ADICIONAR:
            size_t pos = this->getUCTurma(
                    pedidoAtual.getUCDesejadas().at(0).getCodUC(),
                    pedidoAtual.getUCDesejadas().at(0).getCodTurma());

            for(auto i : this->horario.at(pos).getHoraUCTurma())
                if(i.gettipo() == TP || i.gettipo() == PL)
                    novoHorario.push_back(i);
            for(auto i : pedidoAtual.getEstudante().getTurmas())
                for(auto j : this->horario.at(this->getUCTurma(i.getCodUC(), i.getCodTurma())).getHoraUCTurma())
                    if(j.gettipo() == TP || j.gettipo() == PL)
                        novoHorario.push_back(j);


            break;
        case ALTERAR:

            break;
        case ALTERARCONJ:
            break;
    }
}

void Gestor::guardarPedido(const Pedido& pedido) {
    this->pedidosFila.push(pedido);
}

int Gestor::getUCTurma(const string& codUC, const string& codTurma) {
    for(int i = 0; i < horario.size(); i++){
        if(this->horario[i].getCodTurma() == codTurma && this->horario[i].getCodUC() == codUC) {
            return i;
        }
    }
    return -1;
}

int Gestor::getEstudante(const string& codEst, const string& nomeEst){
    for(int i = 0; i < this->estudantes.size(); i++){
        if(this->estudantes[i].getCodTurma() == codEst && this->estudantes[i].getCodUC() == nomeEst) {
            return i;
        }
    }
}

UCTurma Gestor::inputUCTurma() {
    string codUc, codTurma;
    int pos = -1;
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
        auto i = this->estudantes.find(estudante);
        if(i != this->estudantes.end()){
            UCTurma ucTurma(v[2],v[3]);
            i->Estudante::addUCTurma(ucTurma);
        }
        else
            this->estudantes.insert(estudante);
    }
}

bool Gestor::compativel(const vector<Slot>& novoHorario) {
    for(int i = 0; i < novoHorario.size() - 1; i++)
        for(int j = i + 1; j < novoHorario.size(); j++)
            if(novoHorario.at(i).conflito(novoHorario.at(j)))
                return false;
    return true;
}
//TODO zl
vector<Slot> Gestor::gerarNovoHorario(const list<UCTurma>& turmas,const vector<UCTurma>& turmasNovas) {
    vector<Slot> novoHorario;
    for(auto turma : turmas){

    }
/*
    for(auto i : this->horario.at(pos).getHoraUCTurma())
        if(i.gettipo() == TP || i.gettipo() == PL)
            novoHorario.push_back(i);
    for(auto i : pedidoAtual.getEstudante().getTurmas())
        for(auto j : this->horario.at(this->getUCTurma(i.getCodUC(), i.getCodTurma())).getHoraUCTurma())
            if(j.gettipo() == TP || j.gettipo() == PL)
                novoHorario.push_back(j);
    return novoHorario;
*/
}

list<Slot> Gestor::getHorariosDeTurma(UCTurma turma) {
    return this->horario.at(this->getUCTurma(turma.getCodUC(),turma.getCodTurma())).getHoraUCTurma();
}
