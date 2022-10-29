#include <iostream>
#include "Gestor.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Estudante.h"
#include "UCturma.h"

using namespace std;

bool compativel(const Estudante &estudante){

    return true;
}

void Gestor::processarPedido() {
    Pedido pedidoAtual = this->pedidosFila.front();
    Estudante estudante(" "," ");
    switch (pedidoAtual.getTipoPedido()) {
        case REMOVER:
            pedidoAtual.getEstudante().rmUCTurma(pedidoAtual.getUCDesejadas().at(0));
            break;
        case ADICIONAR:
            break;
        case ALTERAR:
            break;
        case ALTERARCONJ:
            break;
    }
}

void Gestor::guardarPedido(const Estudante &estudante,TipoPedido tipoPedido) {
    Pedido pedido(estudante,tipoPedido);
    string codUC,codTurma;
    UCTurma ucTurma;
    int pos = -1;
    switch (tipoPedido) {
        case REMOVER:
            do {
                cin >> codTurma;
                cin >> codUC;
            } while ((pos = getUCTurma(codUC,codTurma)) == -1);
            pedido.addUCDesejada(this->horario.at(pos));
            break;
        case ADICIONAR:

            break;
        case ALTERAR:
            break;
        case ALTERARCONJ:
            break;
    }
}

int Gestor::getUCTurma(string codUC, string codTurma) {
    return -1;
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
            cout << "Código da"
    }
    return this->horario.at(pos);
}
const vector<string> explode(const string& s, const char& c)
{
    string buff{""};
    vector<string> v;

    for(auto n:s)
    {
        if(n != c) buff+=n; else
        if(n == c && buff != "") { v.push_back(buff); buff = ""; }
    }
    if(buff != "") v.push_back(buff);

    return v;
}

void addEstudante(){
    string linha;
    ifstream in("classes_per_uc.csv");

    while( getline( in, linha) ) {
        vector<string> v{explode(linha, ',')};
        UCTurma ucTurma(v[2], v[3]);


    }
}

void addEstudante(){
    string linha;
    ifstream in("students_classes.csv");

    while( getline( in, linha) ) {
        vector<string> v{explode(linha, ',')};
        Estudante estudante(v[0], v[1]);



    }
}

