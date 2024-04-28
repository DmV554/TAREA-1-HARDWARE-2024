#include "Cliente.h"

Cliente::Cliente(int entrada, int espera, int corte) 
    : tiempoEntrada(entrada), tiempoEspera(espera), tiempoCorte(corte), sillaUtilizada(-1), sillaBUtilizada(-1), barbero(-1) {}

int Cliente::obtenerTiempoEntrada() {
    return tiempoEntrada;
}

int Cliente::obtenerTiempoEspera() {
    return tiempoEspera;
}

int Cliente::obtenerTiempoCorte() {
    return tiempoCorte;
}

void Cliente::setearSillaUtilizada(int sillaUtilizada) {
    this->sillaUtilizada = sillaUtilizada;
}

void Cliente::setearSillaBUtilizada(int sillaBUtilizada) {
    this->sillaBUtilizada = sillaBUtilizada;
}

void Cliente::setearBarbero(int setearBarbero) {
    this->barbero = setearBarbero;
}

int Cliente::obtenerSillaUtilizada() {
    return sillaUtilizada;
}

int Cliente::obtenerSillaBUtilizada() {
    return sillaBUtilizada;
}

int Cliente::obtenerBarbero() {
    return barbero;
}