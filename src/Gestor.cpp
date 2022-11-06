#include "Gestor.h"

using namespace std;

/**
 * Função que trata dos pedidos dos alunos, os pedidos são guardados numa fila e retira-se o primeiro. Caso seja aceite é
 * realizado o pedido, caso não seja aceite é enviado para uma lista dos pediods recusados.<br>
 * COMPLEXIDADE O(cdt+nh+cp+ste)<br>
 * cdt = complexidade da funcção checkDisponibilidadeTurmas(){O(n*t*u*e)}<br>
 * nh = complexidade da funcção novoHorario(){O(e*(n+a))}<br>
 * cp = complexidade da funcção compativel(){O(a^2)}<br>
 * ste = complexidade da funcção switchTurmasEstudante(){O(e*n)}<br>
 * RESULTADO O(e*p(t*u + e*a + a^2))<br>
 * e = número de turmas do estudante<br>
 * p = número de turmas que o estudante pretende entrar<br>
 * t = número de turmas total<br>
 * u = número de turmas de cada UC<br>
 * a = número de aulas de cada turma
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
            if(checkDisponibilidadeTurmas(pedidoAtual.getUCDesejadas(),pedidoAtual.getTipoPedido(),pedidoAtual.getEstudante()) &&
              Gestor::compativel(this->novoHorario(pedidoAtual.getEstudante().getTurmas(), pedidoAtual.getUCDesejadas()))){
                this->switchTurmasEstudante(pedidoAtual.getEstudante(),pedidoAtual.getUCDesejadas(),pedidoAtual.getTipoPedido());
                return true;
            }else {
                this->pedidosRejeitados.push_back(pedidoAtual);
                return false;
            }
    }
    return false;
}

/**
 * Procura o horario de uma turma<br>
 * COMPLEXIDADE O(t)<br>
 * t = número de turmas total
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
 * acordo com a virgula.<br>
 * COMPLEXIDADE O(c)<br>
 * c = número de caracteres na string
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
 * ler o ficehrio e criar o objeto turma<br>
 * COMPLEXIDADE O(l*c)<br>
 * l = número de linhas do ficheiro<br>
 * c = número de caracteres em cada linha
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
 * adiciona a essas turmas o horário e tipo das aulas.<br>
 * COMPLEXIDADE O(l*t*c)<br>
 * l = número de linhas do ficheiro<br>
 * t = número de turmas total<br>
 * c = número de caracteres em cada linha
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
    }
}

/**
 * lê o ficheiro dos estudantes e cria um objeto estudante com o seu número, nome e turmas.<br>
 * COMPLEXIDADE O(n*(t+2log(j))*c)<br>
 * n = número de linhas no ficheiro<br>
 * t = número de turmas total<br>
 * j = número de estudantes<br>
 * c = número de caracteres em cada linha
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
            auto& est = const_cast<Estudante &>(*i);
            est.addUCTurma(ucTurma);
        }else {
            estudante.addUCTurma(ucTurma);
            this->estudantes.insert(estudante);
        }
    }
}

/**
 * Criado um novo horário esta função vai ver se não há aulas sobrepostas.<br>
 * COMPLEXIDADE O(a^2)<br>
 * a = número de aulas
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
 * Pega duas listas de turmas e troca as turmas atuais pelas do pedido, todas as trocadas têm de ter a mesma UC.<br>
 * COMPLEXIDADE O(e*(p+a))<br>
 * e = número de turmas atuais<br>
 * p = número de turmas do pedido<br>
 * a = número de aulas das turmas
 * @param turmas turmas atuais
 * @param turmasNovas turmas do pedido
 * @return retorna um vetor com o novo horário depois de feitas as trocas.
 */
vector<Slot> Gestor::novoHorario(const list<UCTurma>& turmas, vector<UCTurma> turmasNovas) const {
    vector<Slot> novoHorario;
    for(const auto& turma : turmas) {
        bool flag = true;
        if(!turmasNovas.empty()) {
            for (auto turmaNova = turmasNovas.begin(); turmaNova != turmasNovas.end();) {
                if (turma.getCodUC() == turmaNova->getCodUC()) {
                    for (auto slot: this->getHorariosDeTurma(turma))
                        if (slot.gettipo() != T) { novoHorario.push_back(slot); }
                    turmaNova = turmasNovas.erase(turmaNova);
                    flag = false;
                    break;
                }else{turmaNova++;}
            }
        }
        if(flag) {
            for (auto slot: this->getHorariosDeTurma(turma))
                if (slot.gettipo() != T)
                    novoHorario.push_back(slot);
        }
    }
    return novoHorario;
}

/**
 * Dada uma turma devolve os harários dessa turma<br>
 * COMPLEXIDADE O(t)<br>
 * t = número de turmas total
 * @param turma turma da qual queremos saber os horários
 * @return retorna a lista com o horário da turma
 */
list<Slot> Gestor::getHorariosDeTurma(const UCTurma& turma) const {
    return this->getTurmaH(turma).getHoraUCTurma();
}

/**
 * Verifica se um estudante pode ser adicionado num conjunto de turmas<br>
 * COMPLEXIDADE O(p*t*u*e)<br>
 * p = número de turmas pedidas<br>
 * t = número de turmas total<br>
 * u = número de turmas de cada UC<br>
 * e = número de turmas do estudante
 * @param turmasPedidas Turmas que o estudante pretende ser adicionado
 * @param tipoPedido Pode ser alteração ou adição
 * @param estudante Estudante que pretende ser adicionados as turmas
 * @return TRUE se as turmas não estão cheias e a adição do estudante nelas não vai gerar desequilíbrio<br>
 * FALSE se alguma das turmas já está cheia ou a adição de um estudante nela iria gerar ou acentuar um desequilíbrio<br>
 * Nota: Considerar que existe desequilíbrio nas turmas de uma UC se a diferença
 * entre o nº de estudantes em duas quaisquer turmas dessa UC é >=4
 */
bool Gestor::checkDisponibilidadeTurmas(const vector<UCTurma>& turmasPedidas, TipoPedido tipoPedido, Estudante estudante) {
    for(const auto& turmaPedida : turmasPedidas){
        if(this->getNEstudantesTurma(turmaPedida) >= UCTurma::MAX_ESTUDANTES) return false;
        auto turmas = this->getTurmasByUC(turmaPedida.getCodUC());
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
 * Dada uma cadeira vamos receber todas as turmas dessa cadeira<br>
 * COMPLEXIDADE O(t)<br>
 * t = número de turmas total
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
 * o metodo devolve um inteiro com o numero de estudantes que estao numa certa turma<br>
 * COMPLEXIDADE O(t)<br>
 * t = número de turmas total
 * @param ucTurma turma da qual se pretende saber o numero de alunos
 * @return numero de alunos da turma
 */
int Gestor::getNEstudantesTurma(const UCTurma& ucTurma) const {
    return this->getTurmaH(ucTurma).getNEstudantes();
}

/**
 * Retorna o elemento da TurmaH que está dentro do horário<br>
 * COMPLEXIDADE O(t)<br>
 * t = número de turmas total
 * @param ucTurma turma que se quer retornar
 * @return retorna o elemento TurmaH
 */
TurmaH Gestor::getTurmaH(const UCTurma& ucTurma) const{
    return this->horario.at(this->getUCTurma(ucTurma.getCodUC(),ucTurma.getCodTurma()));
}

/**
 * Para um determinado estudante este metodo procura o seu horario e organiza numa string<br>
 * COMPLEXIDADE O(e*a)<br>
 * e = número de turmas do estudante<br>
 * a = média dos números de aulas das turmas
 * @param estudante estudante que se pretende ver o horario
 * @return string com o horario do estudante numa formatacao do genero (Dia: ; Hora de inicio: ; Tipo: )
 */
string Gestor::getEstudanteHorario(const Estudante& estudante) const{
      string stringHorario;
      auto turmas = estudante.getTurmas();
      for(const auto& turma : turmas){
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
 * <br>COMPLEXIDADE O(1)
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
 * <br>COMPLEXIDADE O(1)
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
 * <br>COMPLEXIDADE O(1)
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
 * Função que chama todas aquelas que vão ler os ficheiros e criar os objetos.<br>
 * COMPLEXIDADE O(c*(l1+l2*k+l3*(t+2log(j))))<br>
 * li = número de linhas de cada ficheiro<br>
 * t = número de turmas total<br>
 * j = número de estudantes<br>
 * c = número de caracteres em cada linha de cada ficheiro
 */
void Gestor::lerFicheiros() {
    this->addUC();
    this->addHorario();
    this->addEstudante();
}

/**
 * imprime na consola o horário do estudante de acordo com o numero introduzido.<br>
 * COMPLEXIDADE O(log(j)+k*a)<br>
 * j = número de estudantes<br>
 * t = número de turmas total<br>
 * a = média dos números de aulas das turmas
 */
void Gestor::verHorariosEstudante() {
    Estudante estudante = this->inputEstudante();
    cout << estudante.estudanteToString() << endl;
    cout << this->getEstudanteHorario(estudante);
}

/**
 * Pede o input do estudante (código e nome) de acordo com o que foi pedido no menu.<br>
 * COMPLEXIDADE O(k*log(j))<br>
 * j = número de estudantes<br>
 * k = número de vezes que o utilizador inserir um código inválido
 * @return retorna o objeto estudante que coincide com os dados introduzios.
 */
Estudante& Gestor::inputEstudante() {
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
    return const_cast<Estudante &>(*i);
}

/**
 * Dada uma turma devolve os estudantes dessa turma.<br>
 * COMPLEXIDADE O(j*k)<br>
 * j = número de estudantes<br>
 * k = média dos números de turmas dos estudantes
 * @param ucTurma string com os estudantes organizados por código e numero
 */
void Gestor::getEstudantesTurma(const UCTurma& ucTurma){
    string s;
    for (const auto & estudante : estudantes){
        auto k = estudante.getTurmas();
        for(auto & j : k){
            if(j.operator==(ucTurma)){
                s+= "Codigo: "+ estudante.getCodEst() + "; Nome:" + estudante.getNomeEst() + "\n";
                break;
            }
        }
    }
    cout << s;
}

/**
 * Pede o input da turma (codigo da turma e codigo da UC) de acordo com o que foi pedido no menu.<br>
 * COMPLEXIDADE O(t*k)<br>
 * t = número de turmas total<br>
 * k = número de vezes que o utilizador inserir um código inválido
 * @return retorna o objeto turma que coincide com os dados introduzios.
 */
TurmaH Gestor::inputTurma() {
    string codTurma, codUC;

    int i;
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
 * Imprime na consola o numero de estudante de cada turma de uma dada UC.<br>
 * COMPLEXIDADE O(t)<br>
 * t = número de turmas
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

/**
 * Troca as turmas do estudante que tiverem o mesmo código de UC de alguma das turmas novas
 * e adiciona as turmas novas que não tiverem o mesmo código de UC de alguma das turmas do estudante<br>
 * COMPLEXIDADE O(e*p)<br>
 * e = número de turmas do estudante<br>
 * p = número de turmas que vai trocar
 * @param estudante Estudante que vai ter as turmas trocadas
 * @param turmasNovas Turmas a serem adicionadas/trocadas no estudante
 * @param tipoPedido O pedido pode ser de Alteração ou Adição
 */
void Gestor::switchTurmasEstudante(Estudante& estudante, vector<UCTurma>& turmasNovas,TipoPedido tipoPedido) {
    auto turmas = estudante.getTurmas();
    if(tipoPedido != ADICIONAR) {
        for (auto turma = turmas.begin(); turma != turmas.end();) {
            for (auto turmaNova = turmasNovas.begin(); turmaNova != turmasNovas.end();) {//const auto& turmaNova : turmasNovas){
                if (turma->getCodUC() == turmaNova->getCodUC()) {
                    this->horario.at(this->getUCTurma(turma->getCodUC(), turma->getCodTurma())).operator--();
                    auto aux = turma++;
                    estudante.rmUCTurma(*aux);
                    estudante.addUCTurma(*turmaNova);
                    turmaNova = turmasNovas.erase(turmaNova);
                    this->horario.at(this->getUCTurma(turmaNova->getCodUC(), turmaNova->getCodTurma())).operator++();
                    break;
                } else {
                    turmaNova++;
                }
            }
            turma++;
        }
    }
    for(const auto& turmaNova : turmasNovas){
        estudante.addUCTurma(turmaNova);
        this->horario.at(this->getUCTurma(turmaNova.getCodUC(), turmaNova.getCodTurma())).operator++();
    }
}

/**
 * De acordo com o input do utilizador cria um novo pedido de alteração de turmas
 * e o coloca na fila de pedidos do gestor<br>
 * COMPLEXIDADE O(it + ie)<br>
 * it = complexidade da função inputTurma(){k*log(j)}<br>
 * ie = complexidade da função inputEstudante(){k*t}<br>
 * RESULTADO: O(k*(t + log(j)))<br>
 * k = número de vezes que o utilizador inserir um código inválido<br>
 * t = número de turmas total<br>
 * j = número de estudantes
 * @param tipo Tipo do pedido de alteração, pode ser: Adicionar, Remover, Alterar
 */
void Gestor::novoPedido(TipoPedido tipo) {
    Estudante& est = this->inputEstudante();
    Pedido novoPedido(est,tipo);
    novoPedido.addUCDesejada(this->inputTurma());
    this->pedidosFila.push(novoPedido);
}

/**
 * De acordo com o input do utilizador cria um novo pedido de alteração de turmas do tipo
 * Alterar conjunto de turmas e o coloca na fila de pedidos do gestor<br>
 * COMPLEXIDADE O(n*it + ie)<br>
 * n = número de turmas que o utilizador quer alterar<br>
 * it = complexidade da função inputTurma(){O(t*k)}<br>
 * ie = complexidade da função inputEstudante(){k*log(j)}<br>
 * RESULTADO: O(n*(t*k) + k*log(j))<br>
 * t = número de turmas total<br>
 * k = número de vezes que o utilizador inserir um código inválido<br>
 * j = número de estudantes
 */
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
