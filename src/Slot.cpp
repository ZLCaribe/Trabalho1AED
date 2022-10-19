#include "Slot.h"

using namespace std;

Slot::Slot(DiaSemana dia, float horaInicio, float duracao, TipoAula tipo) {
    this -> dia = dia;
    this -> horaInicio = horaInicio;
    this -> duracao = duracao;
    this -> tipo = tipo;
}

DiaSemana Slot::getdia() {
    return this -> dia;
}

float Slot::getHoraInicio() {
    return this -> horaInicio;
}

float Slot::getDuracao() {
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