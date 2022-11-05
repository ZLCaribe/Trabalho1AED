#include <iostream>
#include "Gestor.h"
#include <fstream>
#include <string>
#include "Estudante.h"
#include "UCTurma.h"

using namespace std;
/**
 *
 * @return
 */
bool Gestor::processarPedido() {
    if(this->pedidosFila.empty()) return false;
    Pedido pedidoAtual = this->pedidosFila.front();
    this->pedidosFila.pop();
    vector<Slot> novoHorario;
    switch (pedidoAtual.getTipoPedido()) {
        case REMOVER:
            pedidoAtual.getEstudante().rmUCTurma(pedidoAtual.getUCDesejadas().at(0));
            this->getTurmaH(pedidoAtual.getUCDesejadas().at(0)).operator--();
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
        if(i != this->estudantes.end())
            est.addUCTurma(ucTurma);
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
        if(this->getNEstudantesTurma(turmaPedida) + 1 - min >= 4)
            return false;
    }
    return true;
}
/**
 * Dada uma cadeira vamos receber todas as turmas dessa cadeira
 * @param codUC cadeira que pretendemos saber as turamas
 * @return lista de turmas da cdeira pretendida
 */
list<TurmaH> Gestor::getTurmasByUC(const string& codUC) {
    list<TurmaH> turmas;
    for(const auto& turma : this->horario){
        if(turma.getCodUC() == codUC)
            turmas.push_back(turma);
    }
    return turmas;
}

/**
 * o metodo devolve um inteiro com o numero de estudantes que estao numa certa turma
 * @param ucTurma turma da qual se pretende saber o numero de alunos
 * @return numero de alunos da turma
 */
int Gestor::getNEstudantesTurma(const UCTurma& ucTurma) const {
    return this->getTurmaH(ucTurma).getNEstudantes();
}

/**
 *
 * @param ucTurma
 * @return
 */
TurmaH Gestor::getTurmaH(const UCTurma& ucTurma) const{
    for(auto turma : this->horario)
        if(turma.getCodUC() == ucTurma.getCodUC() && turma.getCodTurma() == ucTurma.getCodTurma())
            return turma;
    return {};
}
/**
 * Para um determinado estudante este metodo procura o seu horario e organiza numa string
 * @param estudante estudante que se pretende ver o horario
 * @return string com o horario do estudante numa formatacao do genero (Dia: ; Hora de inicio: ; Tipo: )
 */
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
/**
 * Menu caso no menu pricipal seja escolhida a terceira opcao. Mais uma vez, este apresenta as opcoes possiveis de executar
 */
void Gestor::menuVerDados(){
    int i = 0;
    while(i != 4) {
        cout << "Selecione a opcao: \n";
        cout << "1: Ocupação de turmas/ano/UC \n";
        cout << "2: Horário de determinado estudante \n";
        cout << "3: Estudantes em determinada turma/UC/ano  \n";
        cout << "4: Voltar atras \n";
        cin >> i;
        switch (i) {
            case 1:
                this->ocupacao();
                break;
            case 2:
                this->verHorariosEstudante();
                break;
            case 3:
                this->verEstudanteTurma();
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
/**
 * Menu caso no menu pricipal seja escolhida a primeira opcao. Mais uma vez, este apresenta as opcoes possiveis de executar
 */
void Gestor::menuAlterar(){
    int i = 0;
    while(i != 5) {
        cout << "Selecione a opcao: \n";
        cout << "1: Remover estudante de turma/UC \n";
        cout << "2: Adicionar estudante a uma turma/UC \n";
        cout << "3: Alterar a turma/UC de um estudante \n";
        cout << "4: Alterar um conjunto de turmas/UCs de um estudante \n";
        cout << "5: Voltar atras \n";
        cin >> i;
        switch (i) {
            case 1:

                break;
            case 2:

                break;
            case 3:

                break;
            case 4:
                break;
            case 5:
                break;
            default:
                cout << "Selecione uma opcao valida!" << endl;
                break;
        }
    }
}
/**
 * Cria o menu pricipal, onde vamos escolher a opcao a realizar
 */
void Gestor::mainMenu(){
    int i = 0;
    while(i != 4){
        cout << "Selecione a opcao: \n";
        cout << "1: Novo pedido de alteração de turma\n";
        cout << "2: Processar pedido de alteração de turma \n";
        cout << "3: Listagens \n";
        cout << "4: Sair \n";
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
    }while(i != this->estudantes.end());
    return *i;
}

string Gestor::getEstudantesTurma(UCTurma ucTurma){
    string s;
    for (auto i = estudantes.begin(); i != estudantes.end(); i++){
        auto k = i->getTurmas();
        for(auto j = k.begin(); j != k.end();j++){
            if(j->operator==(ucTurma)){
                s+= "Codigo: "+ i->getCodEst() + "; Nome:" + i->getNomeEst() + "\n";
            }
        }

    }

}
void Gestor::verEstudanteTurma() {
    UCTurma ucTurma = this->inputTurma();
    for (auto i = estudantes.begin(); i != estudantes.end(); i++){
        auto k = i->getTurmas();
        for(auto j = k.begin(); j != k.end();j++){
            if(j->operator==(ucTurma)){
                cout << i->getCodEst() + i->getNomeEst() << endl;
            }
            else if(i == this->estudantes.end()) {
                cout << "Codigo invalido!" << endl;
            }
        }

    }
}

TurmaH Gestor::inputTurma() {
    string codTurma, codUC;

    UCTurma ucTurma = UCTurma(codUC,codTurma);
    auto i = this->horario.begin();
    do {
        cout << "Insira o codigo da turma: ";
        cin >> codTurma;
        cout << "Insira o codigo da UC";
        cin >> codUC;
        if(i == this->horario.end())
            cout << "Codigo invalido!" << endl;
    }while(i != this->horario.end());
    return this->getTurmaH(ucTurma);

}

void Gestor::ocupacao() {
    string s;
    sort(horario.begin(),horario.end(), TurmaH::compararNumEstudante);
    for(auto i: horario){
        s += "Codigo UC: " + i.getCodUC() +" ; Codigo turma: " + i.getCodTurma() +
                " ; Numero de estudantes: " + to_string(i.getNEstudantes()) + "\n";
    }
    cout << s;
}


void Gestor::switchTurmasEstudante(Estudante& estudante, const vector<UCTurma>& turmasNovas) const {
    auto turmas = estudante.getTurmas();
    for(auto turma = turmas.begin(); turma != turmas.end();){
        for(const auto& turmaNova : turmasNovas){
            if(turma->getCodUC() == turmaNova.getCodUC()){
                this->getTurmaH(*turma).operator--();
                turma = estudante.rmUCTurma(*turma);
                estudante.addUCTurma(turmaNova);
                this->getTurmaH(turmaNova).operator++();
                break;
            }else
                turma++;
        }
    }
}

