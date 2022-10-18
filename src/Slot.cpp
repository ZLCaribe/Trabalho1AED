#include "Slot.h"

using namespace std;

Slot::Slot(DiaSemana dia, float HoraInicio, float Duracao, TipoAula tipo) {
    this -> DiaSemana = dia;
    this -> HoraInicio = HoraInicio;
    this -> Duracao = Duracao;
    this -> TipoAula = tipo;
}

DiaSemana Slot::getdia() {
    return this -> dia;
}

float Slot::getHoraInicio() {
    return this -> HoraInicio;
}

float Slot::getDuracao() {
    return this -> Duracao;
}

TipoAula Slot::gettipo() {
    return this -> tipo;
}

void Slot::setdia(DiaSemana dia) {
    this -> DiaSemana = dia;
}

void Slot::setHoraInicio(float HoraIncio) {
    this -> HoraIncio = HoraIncio;
}

void Slot::setDuracao(float Duracao) {
    this -> Duracao = Duracao;
}

void Slot::settipo(TipoAula tipo) {
    this -> TipoAula = tipo;
}