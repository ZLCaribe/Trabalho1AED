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

DiaSemana Slot::stringToDiaSemana(const string& dia) {
    if(dia == "Monday") return SEGUNDA;
    if(dia == "Tuesday") return TERCA;
    if(dia == "Wednesday")  return QUARTA;
    if(dia == "Thursday") return QUINTA;
    return SEXTA;
}

TipoAula Slot::stringToTipo(const string& tipo){
    if(tipo == "T") return T;
    if(tipo == "TP") return TP;
    else return PL;
}

string Slot::DiaSemanaToString(const DiaSemana& dia) {
    if(dia == SEGUNDA) return "segunda";
    if(dia == TERCA) return "terca";
    if(dia == QUARTA)  return "quarta";
    if(dia == QUINTA) return "quinta";
    return "sexta";
}

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

bool Slot::conflito(Slot slot) const {
    if(this->dia != slot.dia) return false;
    if(this->horaInicio == slot.horaInicio) return true;
    if(this->horaInicio < slot.horaInicio)
        return slot.horaInicio < this->horaInicio + this->duracao;
    else
        return this->horaInicio < slot.horaInicio + slot.duracao;
}
