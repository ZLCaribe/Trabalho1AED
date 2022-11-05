#include <iostream>
#include <algorithm>
#include "Gestor.h"
#include <fstream>
#include <string>
#include "Estudante.h"
#include "UCTurma.h"

using namespace std;
bool Gestor::processarPedido() {
    if(this->pedidosFila.empty()) return false;
    Pedido pedidoAtual = this->pedidosFila.front();
    this->pedidosFila.pop();
    vector<Slot> novoHorario;
    UCTurma ucTurma;
    switch (pedidoAtual.getTipoPedido()) {
        case REMOVER:
            ucTurma = pedidoAtual.getUCDesejadas().at(0);
            pedidoAtual.getEstudante().rmUCTurma(ucTurma);
            this->horario.at(this->getUCTurma(ucTurma.getCodUC(),ucTurma.getCodTurma())).operator--();
            return true;
        case ADICIONAR:
        case ALTERAR:
        case ALTERARCONJ:
            if(checkDisponibilidadeTurmas(pedidoAtual.getUCDesejadas(),
                                          pedidoAtual.getTipoPedido(),
                                          pedidoAtual.getEstudante())
              && Gestor::compativel(this->novoHorario(pedidoAtual.getEstudante().getTurmas(),
                                                             pedidoAtual.getUCDesejadas()))){
                this->switchTurmasEstudante(pedidoAtual.getEstudante(),pedidoAtual.getUCDesejadas());
                return true;
            }else {
                this->pedidosRejeitados.push_back(pedidoAtual);
                return false;
            }
    }
    return false;
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
    ifstream in("../schedule/classes_per_uc.csv");

    getline(in,linha);
    while( getline( in, linha) ) {
        vector<string> v{explode(linha, ',')};
        TurmaH ucTurma(v[0], v[1]);
        this->horario.push_back(ucTurma);
    }
}
void Gestor::addHorario(){
    string linha;
    ifstream in("../schedule/classes.csv");

    getline(in,linha);
    while( getline( in, linha) ) {
        vector<string> v{explode(linha, ',')};
        int j = getUCTurma(v[1],v[0]);
        DiaSemana dia = Slot::stringToDiaSemana(v[2]);
        TipoAula tipo = Slot::stringToTipo(v[5]);
        float hora = stof(v[3]);
        float duracao = stof(v[4]);
        Slot slot(dia,hora,duracao,tipo);
        if(j >= 0)
            this->horario[j].addSlot(slot);
        //else
            //cout << "UCTurma n existe" << endl;
    }
}

void Gestor::addEstudante(){
    string linha;
    ifstream in("../schedule/students_classes.csv");

    getline(in,linha);
    while( getline( in, linha) ) {
        vector<string> v{explode(linha, ',')};
        Estudante estudante(v[0], v[1]);
        UCTurma ucTurma(v[2],v[3]);
        this->horario.at(this->getUCTurma(v[2],v[3])).operator++();
        auto i = this->estudantes.find(estudante);
        if(i != this->estudantes.end()) {
            auto est = *i;
            est.addUCTurma(ucTurma);
        }else {
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
        if(this->getNEstudantesTurma(turmaPedida) + 1 - min >= 4)
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
    return this->horario.at(this->getUCTurma(ucTurma.getCodUC(),ucTurma.getCodTurma()));
}

string Gestor::getEstudanteHorario(const Estudante& estudante) const{
      string stringHorario;
      for(const auto& turma : estudante.getTurmas()){
          list<Slot> a = getHorariosDeTurma(turma);
          stringHorario += "CodUC: " + turma.getCodUC() + "; CodTurma: " + turma.getCodTurma() + "\n";
          for(auto j : a){
              stringHorario += "Dia: " + Slot::DiaSemanaToString(j.getdia()) + "; \n Hora de inicio: " +
                               to_string(j.getHoraInicio()) + ";\n Duracao: " + to_string(j.getDuracao()) +
                               ";\n Tipo:" + Slot::tipoToString(j.gettipo()) + "\n";
          }
      }
      return stringHorario;

}

void Gestor::menuVerDados(){
    int i = 0;
    while(i != 4) {
        cout << "------------MENU LISTAGENS----------" << endl;
        cout << "Selecione a opcao: \n";
        cout << "1: Ocupacao de turmas/ano/UC \n";
        cout << "2: Horario de determinado estudante \n";
        cout << "3: Estudantes em determinada turma/UC/ano  \n";
        cout << "4: Voltar atras \n";
        cout << "opcao: ";
        cin >> i;
        switch (i) {
            case 1:
                this->ocupacao();
                break;
            case 2:
                this->verHorariosEstudante();
                break;
            case 3:
                this->getEstudantesTurma(this->inputTurma());
                break;
            case 4:
                cout << "A voltar..." << endl;
                break;
            default:
                cout << "Selecione uma opcao valida!" << endl;
                break;
        }
    }
}

void Gestor::menuAlterar(){
    int i = 0;
    while(i != 5) {
        cout << "------------MENU ALTERAR----------" << endl;
        cout << "Selecione a opcao: \n";
        cout << "1: Remover estudante de turma/UC \n";
        cout << "2: Adicionar estudante a uma turma/UC \n";
        cout << "3: Alterar a turma/UC de um estudante \n";
        cout << "4: Alterar um conjunto de turmas/UCs de um estudante \n";
        cout << "5: Voltar atras \n";
        cout << "opcao: ";
        cin >> i;
        switch (i) {
            case 1:
                this->novoPedido(REMOVER);
                break;
            case 2:
                this->novoPedido(ADICIONAR);
                break;
            case 3:
                this->novoPedido(ALTERAR);
                break;
            case 4:
                this->novoPedidoConj();
                break;
            case 5:
                cout << "A voltar..." << endl;
                break;
            default:
                cout << "Selecione uma opcao valida!" << endl;
                break;
        }
    }
}

void Gestor::mainMenu(){
    int i = 0;
    while(i != 4){
        cout << "------------MENU PRINCIPAL----------" << endl;
        cout << "Selecione a opcao: \n";
        cout << "1: Novo pedido de alteracao de turma\n";
        cout << "2: Processar pedido de alteracao de turma \n";
        cout << "3: Listagens \n";
        cout << "4: Sair \n";
        cout << "opcao: ";
        cin >> i;
        switch (i) {
            case 1:
                this->menuAlterar();
                break;
            case 2:
                if(this->processarPedido())
                    cout << "Pedido satisfeito!" << endl;
                else
                    cout << "Pedido rejeitado." << endl;
                break;
            case 3:
                this->menuVerDados();
                break;
            case 4:
                cout << "A sair..." << endl;
                break;
            default:
                cout << "Selecione uma opcao valida!" << endl;
        }
    }
}

void Gestor::lerFicheiros() {
    this->addUC();
    this->addHorario();
    this->addEstudante();
}

void Gestor::verHorariosEstudante() {
    Estudante estudante = this->inputEstudante();
    cout << estudante.estudanteToString() << endl;
    cout << this->getEstudanteHorario(estudante);
}

Estudante Gestor::inputEstudante() {
    string codEst;
    Estudante est;
    auto i = this->estudantes.begin();
    do {
        cout << "Insira o codigo do estudante: ";
        cin >> codEst;
        est = Estudante(codEst,"");
        i = this->estudantes.find(est);
        if(i == this->estudantes.end())
            cout << "Codigo invalido!" << endl;
    }while(i == this->estudantes.end());
    return *i;
}

void Gestor::getEstudantesTurma(const UCTurma& ucTurma){
    string s;
    for (const auto & estudante : estudantes){
        auto k = estudante.getTurmas();
        for(auto & j : k){
            if(j.operator==(ucTurma)){
                s+= "Codigo: "+ estudante.getCodEst() + "; Nome:" + estudante.getNomeEst() + "\n";
            }
        }
    }
    cout << s;
}

TurmaH Gestor::inputTurma() {
    string codTurma, codUC;

    int i = -1;
    do {
        cout << "Insira o codigo da turma: ";
        cin >> codTurma;
        cout << "Insira o codigo da UC: ";
        cin >> codUC;
        i = this->getUCTurma(codUC,codTurma);
        if(i == -1)
            cout << "Codigo invalido!" << endl;
    }while(i == -1);
    return this->horario.at(i);
}

void Gestor::ocupacao() {
    string s;
    sort(this->horario.begin(), this->horario.end(),TurmaH::compararNumEstudante);
    for(const auto& i: horario){
        s += "Codigo UC: " + i.getCodUC() +" ; Codigo turma: " + i.getCodTurma() +
                " ; Numero de estudantes: " + to_string(i.getNEstudantes()) + "\n";
    }
    cout << s;
}


void Gestor::switchTurmasEstudante(Estudante& estudante, const vector<UCTurma>& turmasNovas) {
    auto turmas = estudante.getTurmas();
    for(auto turma = turmas.begin(); turma != turmas.end();){
        for(const auto& turmaNova : turmasNovas){
            if(turma->getCodUC() == turmaNova.getCodUC()){
                this->horario.at(this->getUCTurma(turma->getCodUC(),turma->getCodTurma())).operator--();
                auto aux = turma++;
                estudante.rmUCTurma(*aux);
                estudante.addUCTurma(turmaNova);
                this->horario.at(this->getUCTurma(turmaNova.getCodUC(),turmaNova.getCodTurma())).operator++();
                break;
            }else
                turma++;
        }
    }
}

void Gestor::novoPedido(TipoPedido tipo) {
    Estudante est = this->inputEstudante();
    Pedido novoPedido(est,tipo);
    novoPedido.addUCDesejada(this->inputTurma());
    this->pedidosFila.push(novoPedido);
}

void Gestor::novoPedidoConj() {
    Estudante est = this->inputEstudante();
    Pedido novoPedido(est,ALTERARCONJ);
    int n;
    cout << "Quantas Turmas quer alterar? ";
    cin >> n;
    cout << endl;
    for(int i = 0; i < n; i++){
        novoPedido.addUCDesejada(inputTurma());
    }
    this->pedidosFila.push(novoPedido);
}
