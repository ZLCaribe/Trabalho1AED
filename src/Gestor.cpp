#include <iostream>
#include <algorithm>
#include "Gestor.h"
#include <fstream>
#include <string>
#include "Estudante.h"
#include "UCTurma.h"

using namespace std;
/**
 * Função que trata dos pedidos dos alunos, os pedidos são guardados numa fila e retira-se o primeiro. Caso seja aceite é
 * realizado o pedido, caso não seja aceite é enviado para uma lista dos pediods recusados.
 * @return retorna se o pedido é possível ou não.
 */
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


/**
 * Envia o pedido para a fila dos pedidos a serem ralizados.
 * @param pedido pedido feito
 */
void Gestor::guardarPedido(const Pedido& pedido) {
    this->pedidosFila.push(pedido);
}

/**
 * Procura o horario de uma turma
 * @param codUC codigo da UC
 * @param codTurma codigo da turma
 * @return retorna um inteiro com a posição da turma
 */

int Gestor::getUCTurma(const string& codUC, const string& codTurma) const{
    for(int i = 0; i < horario.size(); i++){
        if(this->horario[i].getCodTurma() == codTurma && this->horario[i].getCodUC() == codUC) {
            return i;
        }
    }
    return -1;
}

/**
 * A função permite separar as linhas do ficheiro em leitura, por um parâmetro a ser definido. Neste caso separa as palavras de
 * acordo com a virgula.
 * @param s linha do ficheiro que esta aser lida
 * @param c caracter pelo qual se quer separar
 * @return vetor de strings com as palavras separadas
 */
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

/**
 * ler o ficehrio e criar o objeto turma
 */
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

/**
 * lê o ficheiro que tem o horário das turmas e cria um segundo objeto com base nas turmas anteriormente criadas e
 * adiciona a essas turmas o horário e tipo das aulas.
 */
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

/**
 * lê o ficheiro dos estudantes e cria um objeto estudante com o seu número, nome e turmas.
 */
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

/**
 * Criado um novo horário esta função vai ver se não há aulas sobrepostas.
 * @param novoHorario novo horario que foi criado para o aulo ( a ser analisado)
 * @return True caso nao haja sobreposição e False caso contrário
 */
bool Gestor::compativel(const vector<Slot>& novoHorario) {
    for(int i = 0; i < novoHorario.size() - 1; i++)
        for(int j = i + 1; j < novoHorario.size(); j++)
            if(novoHorario.at(i).conflito(novoHorario.at(j)))
                return false;
    return true;
}

/**
 * Pega duas listas de turmas e troca as turmas atuais pelas do pedido, todas as trocadas têm de ter a mesma UC.
 * @param turmas turmas atuais
 * @param turmasNovas turmas do pedido
 * @return retorna um vetor com o novo horário depois de feitas as trocas.
 */
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

/**
 * Dada uma turma devolve os harários dessa turma
 * @param turma turma da qual queremos saber os horários
 * @return retorna a lista com o horário da turma
 */
list<Slot> Gestor::getHorariosDeTurma(const UCTurma& turma) const {
    return this->getTurmaH(turma).getHoraUCTurma();
}

/**
 *
 * @param turmasPedidas
 * @param tipoPedido
 * @param estudante
 * @return
 */
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
 * Retorna o elemento da TurmaH que está dentro do horário
 * @param ucTurma turma que se quer retornar
 * @return retorna o elemento TurmaH
 */
TurmaH Gestor::getTurmaH(const UCTurma& ucTurma) const{
    return this->horario.at(this->getUCTurma(ucTurma.getCodUC(),ucTurma.getCodTurma()));
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
/**
 * Menu caso no menu pricipal seja escolhida a primeira opcao. Mais uma vez, este apresenta as opcoes possiveis de executar
 */
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
/**
 * Cria o menu pricipal, onde vamos escolher a opcao a realizar
 */
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

/**
 * Função que chama todas aquelas que vão ler os ficheiros e criar os objetos.
 */
void Gestor::lerFicheiros() {
    this->addUC();
    this->addHorario();
    this->addEstudante();
}

/**
 * imprime na consola o horário do estudante de acordo com o numero introduzido.
 */
void Gestor::verHorariosEstudante() {
    Estudante estudante = this->inputEstudante();
    cout << estudante.estudanteToString() << endl;
    cout << this->getEstudanteHorario(estudante);
}

/**
 * Pede o input do estudante (código e nome) de acordo com o que foi pedido no menu.
 * @return retorna o objeto estudante que coincide com os dados introduzios.
 */
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

/**
 * Dada uma turma devolve os estudantes dessa turma.
 * @param ucTurma string com os estudantes organizados por código e numero
 */
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
/**
 * Pede o input da turma (codigo da turma e codigo da UC) de acordo com o que foi pedido no menu.
 * @return retorna o objeto turma que coincide com os dados introduzios.
 */
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
/**
 * Imprime na consola o numero de estudante de cada turma de uma dada UC.
 */
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
