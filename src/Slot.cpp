#include "Slot.h"

using namespace std;

Slot::Slot(DiaSemana dia, float horaInicio, float duracao, TipoAula tipo):
    dia(dia),horaInicio(horaInicio),duracao(duracao),tipo(tipo) {}

DiaSemana Slot::getdia() {
    return this -> dia;
}

float Slot::getHoraInicio() const {
    return this -> horaInicio;
}

float Slot::getDuracao() const {
    return this -> duracao;
}

TipoAula Slot::gettipo() {
    return this -> tipo;
}

void Slot::setdia(DiaSemana dia) {
    this -> dia = dia;
}

void Slot::setHoraInicio(float horaInicio) {
    this -> horaInicio = horaInicio;
}

void Slot::setDuracao(float duracao) {
    this -> duracao = duracao;
}

void Slot::settipo(TipoAula tipo) {
    this -> tipo = tipo;
}

/**
 * Dada uma string esta função trasnforma no tipo de dado DiaSemana<br>
 * COMPLEXIDADE O(1)
 * @param dia string correspondete ao dia da semana
 * @return DiaSemana correspondete ao que estava na string
 */
DiaSemana Slot::stringToDiaSemana(const string& dia) {
    if(dia == "Monday") return SEGUNDA;
    if(dia == "Tuesday") return TERCA;
    if(dia == "Wednesday")  return QUARTA;
    if(dia == "Thursday") return QUINTA;
    return SEXTA;
}

/**
 * Dada uma string esta função trasnforma no tipo de dado Tipo<br>
 * COMPLEXIDADE O(1)
 * @param tipo tipo da aula como string
 * @return retorna o tipo de aula com o tipo de dado Tipo
 */
TipoAula Slot::stringToTipo(const string& tipo){
    if(tipo == "T") return T;
    if(tipo == "TP") return TP;
    else return PL;
}

/**
 * Dada um DiaSemana esta função trasnforma em string<br>
 * COMPLEXIDADE O(1)
 * @param dia DiaSemana que se prentede alterar para sting
 * @return string com o dia da semana
 */
string Slot::DiaSemanaToString(const DiaSemana& dia) {
    if(dia == SEGUNDA) return "segunda";
    if(dia == TERCA) return "terca";
    if(dia == QUARTA)  return "quarta";
    if(dia == QUINTA) return "quinta";
    return "sexta";
}

/**
 * Dada um Tipo esta função trasnforma em string<br>
 * COMPLEXIDADE O(1)
 * @param tipo Tipo que se prentede alterar para sting
 * @return string com o tipo de aula
 */
string Slot::tipoToString(const TipoAula& tipo){
    if(tipo == T) return "T";
    if(tipo == TP) return "TP";
    else return "PL";
}

Slot::Slot() {
    this->tipo = T;
    this->duracao = 0;
    this->horaInicio = 0;
    this->dia = SEGUNDA;
}

/**
 * recebe uma aula e verifica se não há sobreposição de horário<br>
 * COMPLEXIDADE O(1)
 * @param slot slot com que vai ser feita a comparação
 * @return caso não haja conflito de horário vai retornar True
 */
bool Slot::conflito(Slot slot) const {
    if(this->dia != slot.dia) return false;
    if(this->horaInicio == slot.horaInicio) return true;
    if(this->horaInicio < slot.horaInicio)
        return slot.horaInicio < this->horaInicio + this->duracao;
    else
        return this->horaInicio < slot.horaInicio + slot.duracao;
}
