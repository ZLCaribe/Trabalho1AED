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

DiaSemana Slot::stringToDiaSemana(string dia) {
    if(dia == "Monday") return SEGUNDA;
    if(dia == "Tuesday") return TERCA;
    if(dia == "Wednesday")  return QUARTA;
    if(dia == "Thursday") return QUINTA;
    if(dia == "Friday") return SEXTA;
}

TipoAula Slot::stringToTipo(string tipo){
    if(tipo == "T") return T;
    if(tipo == "TP") return TP;
}
